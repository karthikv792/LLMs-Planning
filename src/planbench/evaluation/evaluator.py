"""Unified ResponseEvaluator for PlanBench.

Merges evaluation methods from both codebases:
- evaluate_plan       — Text-based plan validation (t1, t2, t4, t5, t6, t8_x)
- evaluate_plan_pddl  — PDDL-format plan validation (t1_pddl, t1_zero_pddl)
- evaluate_plan_parallel — Parallel plan extraction with LLM translator
- extract_plans       — Per-instance LLM-based plan extraction
- evaluate_state      — State comparison (t7)
- evaluate_verification — Structured verification output parsing (t3, t3_1)
- parse_output        — Parse LLM verification responses
"""

from __future__ import annotations

import concurrent.futures
import json
import os
from typing import Any

import yaml
from tarski.io import PDDLReader
from tqdm import tqdm

from planbench.executor import Executor
from planbench.evaluation.validator import validate_plan, validate_plan_unsolvable
from planbench.model_parser.writer import ModelWriter
from planbench.translation import (
    text_to_plan,
    text_to_plan_with_llm,
    text_to_state,
)
from planbench.utils.helpers import get_cost_gpt_3, save_gpt3_response


class ResponseEvaluator:
    """Evaluates LLM responses against ground truth for PlanBench tasks."""

    def __init__(
        self,
        config_file: str,
        engine: str,
        specified_instances: list[int] | None = None,
        verbose: bool = False,
        ignore_existing: bool = False,
        translator_engine: str = "gpt-4o",
    ) -> None:
        self.engine = engine
        self.verbose = verbose
        self.ignore_existing = ignore_existing
        self.specified_instances = list(specified_instances or [])
        self.translator_engine = translator_engine
        self.data = self.read_config(config_file)
        self.instance_dir = self.data['instance_dir']
        self.domain_pddl = f'./instances/{self.data["domain_file"]}'
        self.llm_plan_file = 'llm_plan'
        self._set_task_params()

    def read_config(self, config_file: str) -> dict:
        with open(config_file, 'r') as file:
            return yaml.safe_load(file)

    def _set_task_params(self, instance_dir: str | None = None) -> None:
        if instance_dir is None:
            instance_dir = self.instance_dir
        self.instance_folder = f'./instances/{instance_dir}/'
        self.instance = f'./instances/{instance_dir}/{self.data["instances_template"]}'
        self.n_files = min(self.data['n_instances'], len(os.listdir(self.instance_folder)))
        self.i_start = self.data['start']
        self.i_end = self.data['end']

    def get_problem(self, instance: str, domain: str) -> Any:
        reader = PDDLReader(raise_on_error=True)
        reader.parse_domain(domain)
        return reader.parse_instance(instance)

    def get_executor(self, instance: str, domain: str, ground: bool = False) -> Executor:
        return Executor(domain, instance, ground=ground)

    def write_new_instance(self, new_model: Any) -> None:
        writer = ModelWriter(new_model)
        writer.write_files('pr-new-domain.pddl', 'pr-new-problem.pddl')

    def load_json(self, task_name: str) -> dict:
        response_dir = f"responses/{self.data['domain_name']}/{self.engine}/"
        output_dir = f"results/{self.data['domain_name']}/{self.engine}/"
        if not self.ignore_existing and os.path.exists(output_dir + f"{task_name}.json"):
            load_dir = output_dir
        else:
            assert os.path.exists(response_dir + f"{task_name}.json")
            load_dir = response_dir
        with open(load_dir + f"{task_name}.json", 'r') as file:
            return json.load(file)

    def save_json(self, structured_output: dict, task_name: str) -> None:
        output_dir = f"results/{self.data['domain_name']}/{self.engine}/"
        os.makedirs(output_dir, exist_ok=True)
        with open(output_dir + f"{task_name}.json", 'w') as file:
            json.dump(structured_output, file, indent=4)

    def _should_skip(self, instance_dict: dict) -> bool:
        """Check if this instance should be skipped."""
        if "llm_raw_response" not in instance_dict:
            return True
        if not instance_dict["llm_raw_response"]:
            if self.verbose:
                print(f"Instance {instance_dict['instance_id']} response not generated")
            return True
        if self.specified_instances:
            if instance_dict['instance_id'] not in self.specified_instances:
                return True
        return False

    # ── evaluate_plan: text-based plan validation ────────────────────────

    def evaluate_plan(self, task_name: str) -> None:
        """Evaluate plans from text responses (t1, t1_zero, t1_cot, t2, t4-t6, t8_x)."""
        structured_output = self.load_json(task_name)
        total_correct = 0
        total_instances = 0
        if 'plan_generalization' in task_name:
            self._set_task_params(instance_dir=self.data['generalized_instance_dir'])

        for instance_dict in tqdm(structured_output["instances"]):
            if self._should_skip(instance_dict):
                continue
            if self.verbose:
                print(f"Evaluating instance {instance_dict['instance_id']}")

            llm_response = instance_dict["llm_raw_response"]
            inst_id = instance_dict["instance_id"]
            cur_instance = self.instance.format(inst_id)
            problem = self.get_problem(cur_instance, self.domain_pddl)
            plan_executor = self.get_executor(cur_instance, self.domain_pddl)

            try:
                llm_plan, _ = text_to_plan(
                    llm_response, problem.actions, self.llm_plan_file, self.data,
                )
                instance_dict["extracted_llm_plan"] = llm_plan
                if 'new_instance' not in instance_dict:
                    correct = int(validate_plan(self.domain_pddl, cur_instance, self.llm_plan_file))
                else:
                    self.write_new_instance(instance_dict['new_instance'])
                    correct = int(validate_plan(
                        'pr-new-domain.pddl', 'pr-new-problem.pddl', self.llm_plan_file,
                    ))
                    del instance_dict['new_instance']
                if 'optimality' in task_name:
                    if correct:
                        cost = get_cost_gpt_3(llm_response)
                        plan_list = [len(pl) > 0 for pl in llm_plan.split('\n')]
                        actual_cost_llm = sum(plan_list)
                        instance_dict["actual_cost_of_llm_plan"] = actual_cost_llm
                        instance_dict["cost_by_llm"] = cost
                        correct = int(actual_cost_llm == plan_executor.cost)
            except Exception as e:
                correct = 0
                print(f"Warning: Plan extraction failed for instance {inst_id}: {e}")

            if self.verbose:
                print(f"Correct: {bool(correct)}")
            instance_dict["llm_correct"] = bool(correct)
            total_correct += correct
            total_instances += 1
            self.save_json(structured_output, task_name)

        if total_instances > 0:
            print(f"Total correct: {total_correct}")
            print(f"Total instances: {total_instances}")
            print(f"Accuracy: {total_correct / total_instances}")

    # ── evaluate_plan_pddl: PDDL-format plan validation ─────────────────

    def evaluate_plan_pddl(self, task_name: str) -> None:
        """Evaluate plans from PDDL-format responses (t1_pddl, t1_zero_pddl)."""
        structured_output = self.load_json(task_name)
        total_correct = 0
        total_instances = 0
        if 'plan_generalization' in task_name:
            self._set_task_params(instance_dir=self.data['generalized_instance_dir'])

        for instance_dict in tqdm(structured_output["instances"]):
            if self._should_skip(instance_dict):
                continue
            if self.verbose:
                print(f"Evaluating instance {instance_dict['instance_id']}")

            llm_response = instance_dict["llm_raw_response"]
            inst_id = instance_dict["instance_id"]
            llm_plan = save_gpt3_response(llm_response, self.llm_plan_file)
            instance_dict["extracted_llm_plan"] = llm_plan

            if "unsolvable" in self.data["domain_name"]:
                correct = int(validate_plan_unsolvable(llm_plan))
            else:
                correct = int(validate_plan(
                    self.domain_pddl, self.instance.format(inst_id), self.llm_plan_file,
                ))

            if self.verbose:
                print(f"Correct: {bool(correct)}")
            instance_dict["llm_correct"] = bool(correct)
            total_correct += correct
            total_instances += 1
            self.save_json(structured_output, task_name)

        if total_instances > 0:
            print(f"Total correct: {total_correct}")
            print(f"Total instances: {total_instances}")
            print(f"Accuracy: {total_correct / total_instances}")

    # ── extract_plans: LLM-based plan extraction ─────────────────────────

    def extract_plans(
        self, instance_dict: dict,
    ) -> tuple[str | None, str | None, int]:
        """Extract plans from a single instance using LLM translator."""
        inst_id = instance_dict["instance_id"]
        print(f"Extracting plans for instance {inst_id}")

        if "llm_raw_response" not in instance_dict:
            return None, None, inst_id

        # If already translated and not ignoring existing
        if instance_dict.get("raw_translation") is not None and not self.ignore_existing:
            llm_plan = text_to_plan_with_llm(
                instance_dict["raw_translation"], self.data, instance_dict,
                translator_engine=self.translator_engine,
            )
            return llm_plan, instance_dict["raw_translation"], inst_id

        if not instance_dict["llm_raw_response"]:
            if self.verbose:
                print(f"Instance {inst_id} response not generated")
            return None, None, inst_id

        if self.specified_instances and inst_id not in self.specified_instances:
            return None, None, inst_id

        if self.verbose:
            print(f"Evaluating instance {inst_id}")

        llm_response = instance_dict["llm_raw_response"]
        llm_plan, raw_translation = text_to_plan_with_llm(
            llm_response, self.data, instance_dict,
            translator_engine=self.translator_engine,
        )
        return llm_plan, raw_translation, inst_id

    # ── evaluate_plan_parallel: parallel LLM-based extraction + validation

    def evaluate_plan_parallel(self, task_name: str) -> None:
        """Evaluate plans with parallel LLM-based extraction (t1, t1_zero, t1_cot)."""
        print(f"Task name: {task_name}")
        structured_output = self.load_json(task_name)
        if 'plan_generalization' in task_name:
            self._set_task_params(instance_dir=self.data['generalized_instance_dir'])

        max_workers = 100
        results: list[tuple] = []
        total_correct = 0
        total_instances = 0

        with concurrent.futures.ThreadPoolExecutor(max_workers=max_workers) as executor:
            futures = {
                executor.submit(self.extract_plans, inst_dict): inst_dict
                for inst_dict in structured_output["instances"]
            }
            for future in concurrent.futures.as_completed(futures):
                try:
                    llm_plan, raw_translation, inst_id = future.result()
                except TypeError as e:
                    print(e, future.result())
                    raise ValueError("Error in extracting plans")
                results.append((llm_plan, raw_translation, inst_id))

        for llm_plan, raw_translation, inst_id in results:
            if llm_plan is not None:
                for instance_dict in structured_output["instances"]:
                    if instance_dict["instance_id"] == inst_id:
                        instance_dict["extracted_llm_plan"] = llm_plan
                        instance_dict["raw_translation"] = raw_translation
                        with open(self.llm_plan_file, 'w') as f:
                            f.write(llm_plan)
                        if "unsolvable" in self.data["domain_name"]:
                            correct = int(validate_plan_unsolvable(llm_plan))
                        else:
                            correct = int(validate_plan(
                                self.domain_pddl, self.instance.format(inst_id),
                                self.llm_plan_file,
                            ))
                        instance_dict["llm_correct"] = bool(correct)
                        total_correct += correct
                        total_instances += 1
                        self.save_json(structured_output, task_name)

        if total_instances > 0:
            print(f"Total correct: {total_correct}")
            print(f"Total instances: {total_instances}")
            print(f"Accuracy: {total_correct / total_instances}")

    # ── evaluate_state: state comparison ──────────────────────────────────

    def evaluate_state(self, task_name: str) -> None:
        """Evaluate state predictions (t7: plan execution)."""
        structured_output = self.load_json(task_name)
        total_correct = 0
        total_instances = 0

        for instance_dict in tqdm(structured_output["instances"]):
            if self._should_skip(instance_dict):
                continue
            if self.verbose:
                print(f"Evaluating instance {instance_dict['instance_id']}")

            llm_response = instance_dict["llm_raw_response"]
            ground_state = instance_dict["ground_truth_plan"]
            llm_state = text_to_state(llm_response, self.data)

            correct = sorted(ground_state) == sorted(llm_state)
            instance_dict["extracted_llm_plan"] = llm_state
            instance_dict["llm_correct"] = bool(correct)
            total_correct += int(correct)
            total_instances += 1
            if self.verbose:
                print(f"Correct: {correct}")
            self.save_json(structured_output, task_name)

        if total_instances > 0:
            print(f"Total correct: {total_correct}")
            print(f"Total instances: {total_instances}")
            print(f"Accuracy: {total_correct / total_instances}")

    # ── parse_output: structured verification response parsing ───────────

    def parse_output(self, action_set: Any, output: str) -> dict:
        """Parse LLM verification response into structured dict."""
        output_dict: dict[str, Any] = {}
        goal_cond = False
        precond_act = False
        precond_act_flag = False
        precond_pred = False

        for line in output.split('\n'):
            if '[STATEMENT]' in line:
                break
            if line.strip() == "":
                continue
            if goal_cond:
                output_dict['unmet_goal'] = text_to_state(line.strip(), self.data)
                goal_cond = False
                continue
            if precond_act:
                _, action = text_to_plan(line.strip(), action_set, self.llm_plan_file, self.data)
                output_dict['unmet_precondition']['action'] = action
                precond_act = False
                precond_act_flag = True
                continue
            if precond_act_flag and precond_pred:
                output_dict['unmet_precondition']['predicate'] = text_to_state(line.strip(), self.data)
                precond_pred = False
                precond_act_flag = False

            if 'plan is valid' in line:
                if 'valid' not in output_dict:
                    output_dict['valid'] = True
                break
            elif 'plan is invalid' in line:
                output_dict['valid'] = False
            if 'unmet goal' in line and 'unmet precondition' in line:
                break
            if 'unmet goal' in line:
                output_dict['unmet_goal'] = ''
                goal_cond = True
            elif 'unmet precondition' in line:
                if 'action' in line:
                    output_dict['unmet_precondition'] = {}
                    precond_act = True
                else:
                    precond_pred = True
            elif 'Unmet precondition:' in line:
                if precond_act_flag:
                    output_dict['unmet_precondition']['predicate'] = text_to_state(
                        line.strip(), self.data,
                    )
                    precond_act_flag = False

        return output_dict

    # ── evaluate_verification: structured verification evaluation ────────

    def evaluate_verification(self, task_name: str) -> None:
        """Evaluate verification responses (t3, t3_1)."""
        structured_output = self.load_json(task_name)
        total_correct_binary = 0
        total_correct_w_type = 0
        total_correct_w_expl = 0
        total_instances = 0

        for instance_dict in structured_output["instances"]:
            if self._should_skip(instance_dict):
                continue
            if self.verbose:
                print(f"Evaluating instance {instance_dict['instance_id']}")

            inst_id = instance_dict["instance_id"]
            cur_instance = self.instance.format(inst_id)
            problem = self.get_problem(cur_instance, self.domain_pddl)
            llm_response = instance_dict["llm_raw_response"]
            ground_truth_response = instance_dict["ground_truth_plan"]

            correct_binary = False
            correct_w_type = False
            correct_w_expl = False

            parsed_llm = self.parse_output(problem.actions, llm_response)
            parsed_gt = self.parse_output(problem.actions, ground_truth_response)
            instance_dict["extracted_llm_plan"] = parsed_llm
            instance_dict["parsed_ground_truth_plan"] = parsed_gt

            if parsed_llm.get('valid') == parsed_gt.get('valid'):
                correct_binary = True
                if not parsed_llm.get('valid'):
                    if sorted(parsed_llm.keys()) == sorted(parsed_gt.keys()):
                        correct_w_type = True
                        if 'unmet_goal' in parsed_llm:
                            if any(
                                pred in parsed_gt['unmet_goal']
                                for pred in parsed_llm['unmet_goal']
                            ):
                                correct_w_expl = True
                        if 'unmet_precondition' in parsed_llm:
                            try:
                                if parsed_llm['unmet_precondition']['action'] == parsed_gt['unmet_precondition']['action']:
                                    if any(
                                        pred in parsed_gt['unmet_precondition']['predicate']
                                        for pred in parsed_llm['unmet_precondition']['predicate']
                                    ):
                                        correct_w_expl = True
                            except KeyError:
                                print(f"For Instance {inst_id}")
                                print(parsed_llm)
                                print(parsed_gt)
                else:
                    correct_w_type = True
                    correct_w_expl = True

            instance_dict['llm_correct_binary'] = correct_binary
            instance_dict['llm_correct_w_type'] = correct_w_type
            instance_dict['llm_correct_w_expl'] = correct_w_expl
            total_correct_binary += int(correct_binary)
            total_correct_w_type += int(correct_w_type)
            total_correct_w_expl += int(correct_w_expl)
            total_instances += 1

            if self.verbose:
                print(f"Correct binary: {correct_binary}")
                print(f"Correct w type: {correct_w_type}")
                print(f"Correct w expl: {correct_w_expl}")
            self.save_json(structured_output, task_name)

        if total_instances > 0:
            print(f"Total correct binary: {total_correct_binary}")
            print(f"Total correct w type: {total_correct_w_type}")
            print(f"Total correct w expl: {total_correct_w_expl}")
            print(f"Total instances: {total_instances}")
            print(f"Accuracy binary: {total_correct_binary / total_instances}")
            print(f"Accuracy w type: {total_correct_w_type / total_instances}")
            print(f"Accuracy w expl: {total_correct_w_expl / total_instances}")

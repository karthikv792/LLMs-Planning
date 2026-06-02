"""Parallel BackPrompter for PlanBench.

Thread-safe parallel plan refinement using ThreadPoolExecutor.
Supports both text and PDDL plan formats, LLM and sound verifiers/critics.

From llm_planning_analysis/back_prompting_parallel.py.

NOTE: This module depends on ``planbench.executor.full_validator`` which
provides ``get_llm_verifier_json``, ``get_validation_message_for_llm``,
and ``get_all_errors``.  If that module is not yet migrated, the
full_validator-dependent code paths (LLM verifier) will raise ImportError
at runtime.
"""

from __future__ import annotations

import concurrent.futures
import json
import os
import random
import string
import time
from threading import Lock
from typing import Any

from tqdm import tqdm

from planbench.translation import (
    fill_template,
    instance_to_text,
    text_to_plan,
)
from planbench.task_utils import (
    plan_verification_zero_shot,
    plan_verification_zero_shot_val_form,
    get_val_feedback,
    get_validation_message,
)
from planbench.utils.helpers import save_gpt3_response
from .base import BackPrompterBase


class ParallelBackPrompter(BackPrompterBase):
    """Parallel backprompting with ThreadPoolExecutor."""

    def __init__(
        self,
        engine: str,
        verbose: bool = False,
        ignore_existing: bool = False,
    ) -> None:
        super().__init__(engine, verbose, ignore_existing)
        self.gpt3_plan_file = "gpt_sas_plan_{}"

    def save_json_backprompting(self, output_file: str, structured_output: dict) -> None:
        os.makedirs(f"results_backprompting/{self.data['domain_name']}/{self.engine}/", exist_ok=True)
        path = f"results_backprompting/{self.data['domain_name']}/{self.engine}/{output_file}.json"
        with open(path, "w") as f:
            json.dump(structured_output, f, indent=4)

    def save_jsonl(self, output_file: str, entry: dict) -> None:
        os.makedirs(f"results_backprompting/{self.data['domain_name']}/{self.engine}/", exist_ok=True)
        path = f"results_backprompting/{self.data['domain_name']}/{self.engine}/{output_file}.jsonl"
        with open(path, "a") as f:
            f.write(json.dumps(entry) + "\n")

    def load_json_backprompting(self, output_file: str) -> dict | None:
        path = f"results_backprompting/{self.data['domain_name']}/{self.engine}/{output_file}.json"
        if os.path.exists(path) and not self.ignore_existing:
            with open(path, "r") as f:
                return json.load(f)
        return None

    def get_info_data(
        self, info_file: str, instance_folder: str, domain_pddl: str,
    ) -> dict:
        """Precompute plans for all instances."""
        info_data: dict = {}
        for instance_name in tqdm(os.listdir(instance_folder), desc="Preprocessing plans"):
            info_data[instance_name] = self._compute_plan_optimal(
                domain_pddl, os.path.join(instance_folder, instance_name),
            )
            with open(info_file, 'w') as f:
                json.dump(info_data, f, indent=4)
        return info_data

    def get_llm_feedback(
        self,
        domain_pddl: str,
        llm_plan: list[str],
        cur_instance: str,
        use_llm_feedback: dict,
        temperatures: dict,
        messages: list[dict] | None = None,
    ) -> tuple[str, list[dict]]:
        """Get LLM-based plan verification feedback."""
        messages = messages or []
        query = self.data["domain_intro"]
        problem = self.get_problem(cur_instance, domain_pddl)

        if not use_llm_feedback.get('zero_shot'):
            raise NotImplementedError("Few-shot not implemented for parallel backprompter")
        else:
            INIT, GOAL, PLAN, _ = instance_to_text(problem, True, self.data, plan=llm_plan)
            if not use_llm_feedback.get('val_form'):
                text = plan_verification_zero_shot(
                    INIT, PLAN, GOAL, cot=use_llm_feedback.get('cot', False),
                )
            else:
                text = plan_verification_zero_shot_val_form(
                    INIT, PLAN, GOAL, cot=use_llm_feedback.get('cot', False),
                )
        query += text

        resp = self._client.query_with_history(
            query, messages, self.max_gpt_response_length,
            temperature=temperatures.get('verifier', 0.0),
        )
        if resp.null_response:
            print("[-] Received null response from LLM")
            return "", resp.messages or []
        return resp.text, resp.messages or []

    def verification(
        self,
        domain_pddl: str,
        llm_plan: str,
        cur_instance: str,
        use_llm_feedback: dict,
        temperatures: dict,
        step_dict: dict,
        steps: int | str,
        start: int,
        feedback_type: int = 1,
        pddl: bool = False,
    ) -> tuple[str, list, bool, bool, dict]:
        """Run verification and produce feedback query."""
        feedback_messages: list = []
        could_not_extract = False
        query = ""
        steps = str(steps)
        verifier_critique = {
            'verifier': use_llm_feedback.get('verifier'),
            'critique': use_llm_feedback.get('critique'),
        }

        if verifier_critique['verifier'] == 'llm' or verifier_critique['critique'] == 'llm':
            # Lazy import for full_validator (may not be migrated yet)
            from planbench.executor.full_validator import (
                get_llm_verifier_json,
                get_validation_message_for_llm,
                get_all_errors,
            )
            query, feedback_messages = self.get_llm_feedback(
                domain_pddl, llm_plan.strip().split("\n"),
                cur_instance, use_llm_feedback, temperatures,
            )
            step_dict[steps]['llm_feedback'] = query
            llm_verifier_json = get_llm_verifier_json(query, use_llm_feedback.get('val_form'))
            if llm_verifier_json is None:
                print("ERROR: Could not extract llm verifier json: ", query)
                could_not_extract = True
                return query, feedback_messages, False, could_not_extract, step_dict

            try:
                llm_verification, llm_critique = get_validation_message_for_llm(llm_verifier_json)
            except Exception as e:
                print(f"[-] Error: {e}")
                could_not_extract = True
                return query, feedback_messages, False, could_not_extract, step_dict

            try:
                val_validator = feedback_type != 2
                sound_verifier_json = (
                    get_val_feedback(domain_pddl, cur_instance, self.gpt3_plan_file.format(start))
                    if val_validator
                    else get_all_errors(domain_pddl, cur_instance, self.gpt3_plan_file.format(start))
                )
            except Exception:
                print("ERROR: Could not extract plan")
                could_not_extract = True
                return query, feedback_messages, False, could_not_extract, step_dict

            if not sound_verifier_json['validation_info']['is_valid_plan']:
                try:
                    sound_verification, sound_critique = get_validation_message(
                        sound_verifier_json, self.data,
                        feedback_type=feedback_type,
                        val_validator=val_validator,
                        pddl=pddl,
                    )
                    if sound_critique is None:
                        sound_critique = ""
                except Exception:
                    print("ERROR: Sound Verifier Dict Extraction error")
                    could_not_extract = True
                    return query, feedback_messages, False, could_not_extract, step_dict
            else:
                sound_verification, sound_critique = "The plan is valid.", ""

            if verifier_critique['verifier'] == 'llm':
                query = llm_verification
            elif verifier_critique['verifier'] == 'sound':
                query = sound_verification
            else:
                query = ''
            if verifier_critique['critique'] == 'llm':
                query += " " + llm_critique
            elif verifier_critique['critique'] == 'sound':
                query += " " + sound_critique

            step_dict[steps]['llm_verifier_json'] = llm_verifier_json
            step_dict[steps]['sound_verifier_json'] = sound_verifier_json
            correct = (
                llm_verifier_json["valid"]
                if verifier_critique['verifier'] == 'llm'
                else sound_verifier_json["validation_info"]["is_valid_plan"]
            )
        else:
            val_validator = feedback_type != 2
            try:
                sound_verifier_json = (
                    get_val_feedback(domain_pddl, cur_instance, self.gpt3_plan_file.format(start))
                    if val_validator
                    else None
                )
                if sound_verifier_json is None:
                    from planbench.executor.full_validator import get_all_errors
                    sound_verifier_json = get_all_errors(
                        domain_pddl, cur_instance, self.gpt3_plan_file.format(start),
                    )
                step_dict[steps]['llm_plan'] = llm_plan
                step_dict[steps]['sound_verifier_json'] = sound_verifier_json
            except Exception as e:
                print(f"[-] Error: could not extract plan: {e}")
                could_not_extract = True
                return query, feedback_messages, False, could_not_extract, step_dict

            correct = int(sound_verifier_json["validation_info"]["is_valid_plan"])
            if not correct:
                try:
                    sound_verification, sound_critique = get_validation_message(
                        sound_verifier_json, self.data,
                        feedback_type=feedback_type,
                        val_validator=val_validator,
                        pddl=pddl,
                    )
                    query = sound_verification
                    try:
                        query = query.strip() + " " + sound_critique
                    except Exception as e:
                        print(f"ERROR: Validation dict missing error: {e}")
                        query += " " + sound_verifier_json.get('validation_message', '')
                except Exception as e:
                    print(f"[-] Error: could not extract: {e}")
                    could_not_extract = True
                    return query, feedback_messages, False, could_not_extract, step_dict

        if not pddl:
            query = query.strip() + " Please provide the plan between [PLAN] and [PLAN END] tags."
        else:
            query = (
                query.strip()
                + "\nProvide your revised plan as a sequence of actions in PDDL format. "
                "An action ACTION which acts on two objects OBJ1 and OBJ2 would be written "
                "(ACTION OBJ1 OBJ2). Do not provide anything else in your answer.\n\n[PLAN]"
            )

        return query, feedback_messages, correct, could_not_extract, step_dict

    def per_instance_output(
        self,
        task_id: Any,
        progress: Any,
        lock: Lock,
        start: int,
        instance: str,
        domain_pddl: str,
        info_data: dict,
        completed_instances: list[int],
        prev_success_results: dict,
        failed_instances: int,
        use_llm_feedback: dict,
        random_example: bool,
        temperatures: dict,
        val_feedback_type: int,
        iterations: int,
    ) -> tuple[dict | None, int]:
        """Process a single instance (text format)."""
        query = self.data["domain_intro"]
        instance_structured_output: dict[str, Any] = {}
        examples: list[int] = []

        for i in range(start, start + self.n_examples + 1):
            last_plan = (i == start + self.n_examples)
            get_plan = not last_plan
            if last_plan:
                cur_instance = instance.format(i)
            else:
                if random_example:
                    new_i = random.choice([ln for ln in range(1, self.n_files) if ln != i])
                    cur_instance = instance.format(new_i)
                    examples.append(new_i)
                else:
                    cur_instance = instance.format(i)
            try:
                cur_plan = info_data[f"instance-{i}.pddl"]["plan"].split('\n')
            except (KeyError, TypeError):
                raise ValueError(f"Plan not found for instance {i}")
            if self.verbose:
                print(f"Instance {cur_instance}")
            problem = self.get_problem(cur_instance, domain_pddl)
            query += fill_template(
                *instance_to_text(problem, get_plan, self.data, plan=cur_plan),
                o4=last_plan,
            )
            if get_plan:
                examples.append(i)
            else:
                instance_structured_output["instance_id"] = i

        instance_structured_output["example_instance_ids"] = examples
        if instance_structured_output["instance_id"] in completed_instances:
            print(f"Instance {instance_structured_output['instance_id']} already completed")
            if not prev_success_results.get(instance_structured_output["instance_id"]):
                failed_instances += 1
            return None, failed_instances

        (
            messages, llm_plan, verifier_states_correct,
            act_correct, steps, context_window_hit,
            could_not_extract, feedback_messages, llm_plans,
        ) = self._get_repeated_verification_impl(
            task_id, progress, lock, self.engine, query,
            domain_pddl, problem, cur_instance, use_llm_feedback,
            start, temperatures, val_feedback_type, iterations,
            pddl=False,
        )

        instance_structured_output["messages"] = messages
        instance_structured_output["steps"] = steps
        instance_structured_output["verifier_states_correct"] = bool(verifier_states_correct)
        instance_structured_output["act_correct"] = bool(act_correct)
        instance_structured_output["step_wise_dict"] = llm_plans
        instance_structured_output["context_window_hit"] = bool(context_window_hit)
        instance_structured_output["could_not_extract"] = bool(could_not_extract)
        instance_structured_output["feedback_messages"] = feedback_messages
        return instance_structured_output, failed_instances

    def per_instance_output_pddl(
        self,
        task_name: str,
        task_id: Any,
        progress: Any,
        lock: Lock,
        start: int,
        instance: str,
        domain_pddl: str,
        info_data: dict,
        completed_instances: list[int],
        prev_success_results: dict,
        failed_instances: int,
        use_llm_feedback: dict,
        random_example: bool,
        temperatures: dict,
        val_feedback_type: int,
        iterations: int,
    ) -> tuple[dict | None, int]:
        """Process a single instance (PDDL format)."""
        alphanum = ''.join(random.choices(string.ascii_lowercase + string.digits, k=6))
        with open(domain_pddl, 'r') as file:
            domain_intro = file.read()
        query = (
            "The following is a PDDL specification of a planning problem. "
            "The first part, under the heading [DOMAIN], is the domain file. "
            "The second part, under the heading [QUERY PROBLEM], is the problem file. "
            "Using this information, which is correct, and no further assumptions, "
            "find a plan which, when run from the specified initial state, satisfies "
            "the specified goal. Provide your answer as a sequence of actions in PDDL "
            "format. An action ACTION which acts on two objects OBJ1 and OBJ2 would be "
            "written (ACTION OBJ1 OBJ2). Do not provide anything else in your answer.\n"
        )
        query += "[DOMAIN]\n" + domain_intro.strip() + "\n\n"
        instance_structured_output: dict[str, Any] = {}
        cur_instance = instance.format(start)
        with open(cur_instance, 'r') as file:
            problem_text = file.read()
        problem_text = "(define" + problem_text.split("(define")[1][:-1].strip() + "\n)"
        query += "[QUERY PROBLEM]\n" + problem_text.strip() + "\n\n[PLAN]"
        query = query.replace("obfuscated_randomized_blocksworld", alphanum)

        if self.verbose:
            print(f"Instance {cur_instance}")
        problem = self.get_problem(cur_instance, domain_pddl)

        try:
            if start in completed_instances:
                print(f"Instance {start} already completed")
                if not prev_success_results.get(start):
                    failed_instances += 1
                return None, failed_instances
        except Exception as e:
            print(f"[-] Error at checking existing instance: {e}")

        (
            messages, llm_plan, verifier_states_correct,
            act_correct, steps, context_window_hit,
            could_not_extract, feedback_messages, llm_plans,
        ) = self._get_repeated_verification_impl(
            task_id, progress, lock, self.engine, query,
            domain_pddl, problem, cur_instance, use_llm_feedback,
            start, temperatures, val_feedback_type, iterations,
            pddl=True,
        )

        instance_structured_output["instance_id"] = start
        instance_structured_output["messages"] = messages
        instance_structured_output["steps"] = steps
        instance_structured_output["verifier_states_correct"] = bool(verifier_states_correct)
        instance_structured_output["act_correct"] = bool(act_correct)
        instance_structured_output["step_wise_dict"] = llm_plans
        instance_structured_output["context_window_hit"] = bool(context_window_hit)
        instance_structured_output["could_not_extract"] = bool(could_not_extract)
        instance_structured_output["feedback_messages"] = feedback_messages
        return instance_structured_output, failed_instances

    def _get_repeated_verification_impl(
        self,
        task_id: Any,
        progress: Any,
        lock: Lock,
        engine: str,
        original_query: str,
        domain_pddl: str,
        problem: Any,
        cur_instance: str,
        use_llm_feedback: dict,
        start: int,
        temperatures: dict,
        feedback_type: int = 1,
        iterations: int = 15,
        pddl: bool = False,
    ) -> tuple:
        """Core verification loop shared between text and PDDL modes."""
        correct = 0
        steps = 0
        query = original_query
        verifier_critique = {
            'verifier': use_llm_feedback.get('verifier'),
            'critique': use_llm_feedback.get('critique'),
        }
        messages: list[dict] = []
        step_dict: dict = {}
        could_not_extract = False
        context_window_hit = False
        feedback_messages: list = []
        readable_plan = ""
        llm_response = ""

        while steps < iterations:
            step_dict[steps] = {}
            with lock:
                if progress is not None:
                    progress.update(task_id, advance=1)

            history = use_llm_feedback.get('history', -1)
            resp = self._client.query_with_history(
                query, messages, self.max_gpt_response_length,
                temperature=temperatures.get('generator', 0.0),
                history=history,
            )
            llm_response = resp.text
            messages = resp.messages or []
            context_window_hit = resp.context_window_exceeded

            step_dict[steps]['time_taken'] = resp.time_taken

            if context_window_hit or resp.null_response:
                break
            if resp.raw_response is not None:
                try:
                    if isinstance(resp.raw_response, dict):
                        step_dict[steps]['response_object'] = resp.raw_response
                    else:
                        step_dict[steps]['response_object'] = json.loads(
                            resp.raw_response.model_dump_json(),
                        )
                except (AttributeError, TypeError):
                    step_dict[steps]['response_object'] = None

            try:
                if not pddl:
                    with lock:
                        llm_plan, readable_plan = text_to_plan(
                            llm_response, problem.actions,
                            self.gpt3_plan_file.format(start), self.data,
                        )
                else:
                    with lock:
                        llm_plan = save_gpt3_response(
                            llm_response, self.gpt3_plan_file.format(start),
                        )
                        readable_plan = llm_plan
                step_dict[steps]['llm_plan'] = llm_plan
            except Exception:
                could_not_extract = True
                break

            query, feedback_messages, correct, could_not_extract, step_dict = self.verification(
                domain_pddl, llm_plan, cur_instance, use_llm_feedback,
                temperatures, step_dict, steps, start,
                feedback_type=feedback_type, pddl=pddl,
            )
            if could_not_extract:
                break
            if not use_llm_feedback.get('ignore_verifier'):
                if correct:
                    break
            steps += 1

        # Final ground-truth verification
        if verifier_critique['verifier'] == 'llm':
            try:
                if not pddl:
                    with lock:
                        _, readable_plan = text_to_plan(
                            llm_response, problem.actions,
                            self.gpt3_plan_file.format(start), self.data,
                        )
                    feedback_dict = get_val_feedback(
                        domain_pddl, cur_instance, self.gpt3_plan_file.format(start),
                    )
                else:
                    with lock:
                        llm_plan = llm_response.strip('```').strip()
                        with open(self.gpt3_plan_file.format(start), 'w') as f:
                            f.write(llm_plan)
                    readable_plan = llm_plan
                    feedback_dict = get_val_feedback(
                        domain_pddl, cur_instance, self.gpt3_plan_file.format(start),
                    )
            except Exception:
                print("WARNING: final plan could not be verified")
                feedback_dict = {"validation_info": {"is_valid_plan": False}}
            act_correct = int(feedback_dict["validation_info"]["is_valid_plan"])
        else:
            act_correct = correct

        return (
            messages, readable_plan, correct, act_correct,
            steps, context_window_hit, could_not_extract,
            feedback_messages, step_dict,
        )

    def task_1_plan_generation_backprompting(
        self,
        config_file: str,
        use_llm_feedback: dict,
        history: int = -1,
        specified_instances: list[int] | None = None,
        random_example: bool = False,
        max_workers: int = 1,
        temperatures: dict | None = None,
        val_feedback_type: int = 1,
        iterations: int = 15,
    ) -> dict:
        """Run parallel backprompting for text-format plan generation."""
        specified_instances = specified_instances or []
        temperatures = temperatures or {'generator': 0, 'verifier': 0}
        self.read_config(config_file)
        task_name = "task_1_plan_generation_backprompting"
        used_vc = f"_{use_llm_feedback.get('verifier')}_verifier_{use_llm_feedback.get('critique')}_critique"
        zshot = "_zero_shot" if use_llm_feedback.get('zero_shot') else ""
        val_form = "_val_form" if use_llm_feedback.get('val_form') else ""
        no_hist = "_history" if history == -1 else f"_{history}_history"
        cot = "_cot" if use_llm_feedback.get('cot') else ""
        add_ft = f"_feedback_type_{val_feedback_type}" if use_llm_feedback.get('critique') == 'sound' else ""
        temps = f"_tv{temperatures['verifier']}_tg{temperatures['generator']}"
        task_name = task_name + used_vc + zshot + val_form + no_hist + cot + add_ft + temps
        print(task_name)

        instance_dir = self.data['instance_dir']
        domain_pddl = f'./instances/{self.data["domain_file"]}'
        instance_folder = f'./instances/{instance_dir}/'
        instance = f'./instances/{instance_dir}/{self.data["instances_template"]}'

        i_start = self.data['start']
        i_end = self.data['end']

        structured_output = self.load_json_backprompting(task_name)
        if structured_output is None:
            structured_output = {
                "task": task_name,
                "engine": self.engine,
                "prompt_type": "oneshot",
                "additional_task_info": f"backprompting_with_feedback_type_{val_feedback_type}",
                "domain": self.data["domain_name"],
                "temperatures": temperatures,
                "instances": [],
            }

        completed_instances, prev_success_results = self._parse_completed(
            structured_output, iterations,
        )

        if specified_instances:
            range_list = [si - self.n_examples for si in specified_instances]
        else:
            range_list = list(range(i_start, i_end + 2 - self.n_examples))

        # Preprocess plans
        info_file = f"./instances/{self.data['domain_name']}/info.json"
        if not os.path.exists(info_file):
            info_data = self.get_info_data(info_file, instance_folder, domain_pddl)
        else:
            with open(info_file, 'r') as f:
                info_data = json.load(f)

        lock = Lock()
        failed_instances = 0

        with concurrent.futures.ThreadPoolExecutor(max_workers=max_workers) as executor:
            futures = [
                executor.submit(
                    self.per_instance_output, None, None, lock,
                    i, instance, domain_pddl, info_data,
                    completed_instances, prev_success_results,
                    failed_instances, use_llm_feedback,
                    random_example, temperatures,
                    val_feedback_type, iterations,
                )
                for i in range_list
            ]
            for future in concurrent.futures.as_completed(futures):
                try:
                    iso, fi = future.result()
                except Exception as e:
                    print(f"[-] Error: {e}")
                    iso = None
                if iso is not None:
                    print(f"Instance {iso['instance_id']} completed")
                    structured_output["instances"].append(iso)
                    self.save_json_backprompting(task_name, structured_output)

        return structured_output

    def task_1_plan_generation_backprompting_zero_shot_pddl(
        self,
        config_file: str,
        use_llm_feedback: dict,
        history: int = -1,
        specified_instances: list[int] | None = None,
        random_example: bool = False,
        max_workers: int = 1,
        temperatures: dict | None = None,
        val_feedback_type: int = 1,
        iterations: int = 15,
    ) -> dict:
        """Run parallel backprompting for zero-shot PDDL plan generation."""
        specified_instances = specified_instances or []
        temperatures = temperatures or {'generator': 0, 'verifier': 0}
        self.read_config(config_file)
        task_name = "task_1_plan_generation_backprompting_zero_shot_pddl"
        used_vc = f"_{use_llm_feedback.get('verifier')}_verifier_{use_llm_feedback.get('critique')}_critique"
        zshot = "_zero_shot" if use_llm_feedback.get('zero_shot') else ""
        val_form = "_val_form" if use_llm_feedback.get('val_form') else ""
        no_hist = "_history" if history == -1 else f"_{history}_history"
        cot = "_cot" if use_llm_feedback.get('cot') else ""
        add_ft = (
            f"_feedback_type_{val_feedback_type}"
            if all(i == "sound" for i in [use_llm_feedback.get('verifier'), use_llm_feedback.get('critique')])
            else ""
        )
        temps = f"_tv{temperatures['verifier']}_tg{temperatures['generator']}"
        task_name = task_name + used_vc + zshot + val_form + no_hist + cot + add_ft + temps
        print(task_name)

        instance_dir = self.data['instance_dir']
        domain_pddl = f'./instances/{self.data["domain_file"]}'
        instance_folder = f'./instances/{instance_dir}/'
        instance = f'./instances/{instance_dir}/{self.data["instances_template"]}'

        i_start = self.data['start']
        i_end = self.data['end']

        structured_output = self.load_json_backprompting(task_name)
        if structured_output is None:
            structured_output = {
                "task": task_name,
                "engine": self.engine,
                "prompt_type": "oneshot",
                "additional_task_info": f"backprompting_with_feedback_type_{val_feedback_type}",
                "domain": self.data["domain_name"],
                "temperatures": temperatures,
                "instances": [],
            }

        completed_instances, prev_success_results = self._parse_completed(
            structured_output, iterations,
        )

        if specified_instances:
            range_list = [si - self.n_examples for si in specified_instances]
        else:
            range_list = list(range(i_start, i_end + 2 - self.n_examples))

        lock = Lock()
        failed_instances = 0

        with concurrent.futures.ThreadPoolExecutor(max_workers=max_workers) as executor:
            futures = [
                executor.submit(
                    self.per_instance_output_pddl, task_name,
                    None, None, lock,
                    i, instance, domain_pddl, {},
                    completed_instances, prev_success_results,
                    failed_instances, use_llm_feedback,
                    random_example, temperatures,
                    val_feedback_type, iterations,
                )
                for i in range_list
            ]
            for future in concurrent.futures.as_completed(futures):
                try:
                    iso, fi = future.result()
                except Exception as e:
                    print(f"[-] Error: {e}")
                    iso = None
                if iso is not None:
                    print(f"Instance {iso['instance_id']} completed")
                    # Upsert
                    existing_ids = {
                        inst["instance_id"] for inst in structured_output["instances"]
                    }
                    if iso["instance_id"] not in existing_ids:
                        structured_output["instances"].append(iso)
                    else:
                        for idx, inst in enumerate(structured_output["instances"]):
                            if inst["instance_id"] == iso["instance_id"]:
                                structured_output["instances"][idx] = iso
                                break
                    with lock:
                        self.save_json_backprompting(task_name, structured_output)

        return structured_output

    @staticmethod
    def _parse_completed(
        structured_output: dict, iterations: int,
    ) -> tuple[list[int], dict[int, bool]]:
        """Extract completed instance IDs and their success status."""
        completed: list[int] = []
        prev_success: dict[int, bool] = {}
        for inst in structured_output.get("instances", []):
            if not inst.get("could_not_extract"):
                if "act_correct" in inst:
                    if inst["act_correct"] or inst.get("steps", 0) >= iterations:
                        completed.append(inst["instance_id"])
                prev_success[inst["instance_id"]] = inst.get("act_correct", False)
            else:
                assistant_msgs = [
                    msg["content"] for msg in inst.get("messages", [])
                    if msg["role"] == "assistant"
                ]
                if assistant_msgs and assistant_msgs[-1]:
                    completed.append(inst["instance_id"])
                    prev_success[inst["instance_id"]] = inst.get("act_correct", False)
        return completed, prev_success

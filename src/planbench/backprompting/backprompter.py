"""Sequential BackPrompter for PlanBench.

Iteratively refines LLM-generated plans using verifier feedback.
Supports both sound (VAL) and LLM-based verification.

From llm_planning_analysis/back_prompting.py.
"""

from __future__ import annotations

import os
import random
import time
from typing import Any

from planbench.translation import (
    fill_template,
    get_plan_as_text,
    instance_to_text,
    text_to_plan,
)
from planbench.task_utils import (
    plan_verification,
    plan_verification_zero_shot,
    plan_verification_zero_shot_val_form,
    get_val_feedback,
    get_validation_message,
)
from .base import BackPrompterBase


class SequentialBackPrompter(BackPrompterBase):
    """Sequential backprompting: single-threaded iterative refinement."""

    def __init__(
        self,
        engine: str,
        verbose: bool = False,
        ignore_existing: bool = False,
    ) -> None:
        super().__init__(engine, verbose, ignore_existing)
        self.gpt3_plan_file = "gpt_sas_plan"

    def is_already_correct(self, instance_id: int) -> bool:
        try:
            old_json = self.load_json("task1_reasoning")
            if old_json:
                for instance in old_json["instances"]:
                    if instance["instance_id"] == instance_id:
                        return instance["correct"]
        except Exception as e:
            print(f"Error: {e}")
        return False

    def get_llm_feedback(
        self,
        domain_pddl: str,
        llm_plan: list[str],
        cur_instance: str,
        use_llm_feedback: dict,
        messages: list[dict] | None = None,
    ) -> tuple[str, list[dict]]:
        """Get LLM-based plan verification feedback."""
        messages = messages or []
        query = self.data["domain_intro"]
        instance_dir = self.data['instance_dir']
        instance_format = f'./instances/{instance_dir}/{self.data["instances_template"]}'
        instance_folder = f'./instances/{instance_dir}/'
        n_files = min(self.data['n_instances'], len(os.listdir(instance_folder)))
        instance_id = int(cur_instance.split('/')[-1].split('.')[0].split('-')[-1])

        if not use_llm_feedback['zero_shot']:
            example_instances = random.choices(
                [ln for ln in range(1, n_files) if ln != instance_id], k=3,
            )
            example_type = [-1, 0, 1]
            random.shuffle(example_type)
            for example, etype in zip(example_instances, example_type):
                example_instance = instance_format.format(example)
                plan_exec = self.get_executor(example_instance, domain_pddl)
                text, _ = plan_verification(
                    plan_exec, self.data, True, give_response=True, example_type=etype,
                )
                query += text
            plan_exec = self.get_executor(cur_instance, domain_pddl)
            text, _ = plan_verification(plan_exec, self.data, False, llm_plan=llm_plan)
        else:
            plan_exec = self.get_executor(cur_instance, domain_pddl)
            if not use_llm_feedback.get('val_form'):
                text = plan_verification_zero_shot(plan_exec, self.data, llm_plan=llm_plan)
            else:
                text = plan_verification_zero_shot_val_form(plan_exec, self.data, llm_plan=llm_plan)
        query += text

        resp = self._client.query_with_history(
            query, messages,
            self.max_gpt_response_length,
        )
        return resp.text, resp.messages or []

    def get_repeated_verification(
        self,
        engine: str,
        original_query: str,
        domain_pddl: str,
        problem: Any,
        cur_instance: str,
        use_llm_feedback: dict,
        threshold_feedback_amount: int = 15,
    ) -> tuple[list, str, bool, bool, int, bool, bool, list]:
        """Iteratively query and verify plans until correct or limit reached."""
        correct = 0
        steps = 0
        query = original_query
        messages: list[dict] = []
        could_not_extract = False
        context_window_hit = False
        feedback_messages: list = []
        readable_plan = ""
        verifier_states_correct = False

        print(f"Sending query to LLM: Instance {cur_instance}")
        while correct == 0 and steps < threshold_feedback_amount:
            resp = self._client.query_with_history(query, messages, self.max_gpt_response_length)
            llm_response = resp.text
            messages = resp.messages or []
            context_window_hit = resp.context_window_exceeded

            if context_window_hit:
                break
            if resp.null_response:
                print("Rate limit or null response. Waiting 60s.")
                time.sleep(60)
                continue

            feedback_messages = []

            if use_llm_feedback.get('use_llm'):
                try:
                    llm_plan, readable_plan = text_to_plan(
                        llm_response, problem.actions, self.gpt3_plan_file,
                        self.data, ground_flag=True,
                    )
                except Exception:
                    could_not_extract = True
                    break
                query, feedback_messages = self.get_llm_feedback(
                    domain_pddl, llm_plan.strip().split("\n"),
                    cur_instance, use_llm_feedback,
                )
                verifier_states_correct = False
                for line in query.split('\n'):
                    if 'plan is valid' in line.lower():
                        correct = True
                        break
            else:
                try:
                    _, readable_plan = text_to_plan(
                        llm_response, problem.actions, self.gpt3_plan_file, self.data,
                    )
                    feedback_dict = get_val_feedback(domain_pddl, cur_instance, self.gpt3_plan_file)
                except Exception:
                    could_not_extract = True
                    break
                verifier_states_correct = int(feedback_dict["validation_info"]["is_valid_plan"])
                query = get_validation_message(feedback_dict, self.data)
            steps += 1

        # Final correctness check (sound verifier for ground truth)
        if use_llm_feedback.get('use_llm'):
            try:
                _, readable_plan = text_to_plan(
                    llm_response, problem.actions, self.gpt3_plan_file, self.data,
                )
                feedback_dict = get_val_feedback(domain_pddl, cur_instance, self.gpt3_plan_file)
            except Exception:
                print("WARNING: final plan could not be verified")
            act_correct = int(feedback_dict["validation_info"]["is_valid_plan"])
        else:
            act_correct = verifier_states_correct

        return (
            messages, readable_plan, verifier_states_correct,
            act_correct, steps, context_window_hit,
            could_not_extract, feedback_messages,
        )

    def task_1_plan_generation_backprompting(
        self,
        config_file: str,
        use_llm_feedback: dict,
        specified_instances: list[int] | None = None,
        random_example: bool = False,
    ) -> dict:
        """Run backprompting for plan generation task."""
        specified_instances = specified_instances or []
        self.read_config(config_file)

        if use_llm_feedback.get('use_llm'):
            if use_llm_feedback.get('zero_shot'):
                task_name = "task_1_plan_generation_backprompting_llm_feedback_zero_shot"
                if use_llm_feedback.get('val_form'):
                    task_name += "_val_form"
            else:
                task_name = "task_1_plan_generation_backprompting_llm_feedback"
        else:
            task_name = "task_1_plan_generation_backprompting"

        instance_dir = self.data['instance_dir']
        domain_pddl = f'./instances/{self.data["domain_file"]}'
        instance_folder = f'./instances/{instance_dir}/'
        instance = f'./instances/{instance_dir}/{self.data["instances_template"]}'
        n_files = min(self.data['n_instances'], len(os.listdir(instance_folder)))

        i_start = self.data['start']
        i_end = self.data['end']
        n_files = i_end - i_start + 1
        final_output = ""

        failed_instances = 0
        structured_output = self.load_json(task_name)
        if structured_output is None:
            structured_output = {
                "task": task_name,
                "engine": self.engine,
                "prompt_type": "oneshot",
                "domain": self.data["domain_name"],
                "instances": [],
            }

        completed_instances: list[int] = []
        prev_success_results: dict[int, bool] = {}
        for inst in structured_output["instances"]:
            if not inst.get("could_not_extract"):
                completed_instances.append(inst["instance_id"])
                prev_success_results[inst["instance_id"]] = inst.get("act_correct", False)
            else:
                assistant_msgs = [
                    msg["content"] for msg in inst.get("messages", [])
                    if msg["role"] == "assistant"
                ]
                if assistant_msgs and assistant_msgs[-1]:
                    completed_instances.append(inst["instance_id"])
                    prev_success_results[inst["instance_id"]] = inst.get("act_correct", False)

        if specified_instances:
            range_list = [si - self.n_examples for si in specified_instances]
        else:
            range_list = list(range(i_start, i_end + 2 - self.n_examples))

        for start in range_list:
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
                        new_i = random.choice([ln for ln in range(1, n_files) if ln != i])
                        cur_instance = instance.format(new_i)
                        examples.append(new_i)
                    else:
                        cur_instance = instance.format(i)

                final_output += f"\n Instance {cur_instance}\n"
                if self.verbose:
                    print(f"Instance {cur_instance}")
                problem = self.get_problem(cur_instance, domain_pddl)
                gt_plan = self.compute_plan(domain_pddl, cur_instance)
                plan = get_plan_as_text(self.data)
                query += fill_template(*instance_to_text(problem, get_plan, self.data))

                if get_plan:
                    examples.append(i)
                else:
                    instance_structured_output["instance_id"] = i

            instance_structured_output["example_instance_ids"] = examples

            if instance_structured_output["instance_id"] in completed_instances:
                print(f"Instance {instance_structured_output['instance_id']} already completed")
                if not prev_success_results.get(instance_structured_output["instance_id"]):
                    failed_instances += 1
                continue

            (
                messages, llm_plan, verifier_states_correct,
                act_correct, steps, context_window_hit,
                could_not_extract, feedback_messages,
            ) = self.get_repeated_verification(
                self.engine, query, domain_pddl, problem,
                cur_instance, use_llm_feedback,
            )

            instance_structured_output["messages"] = messages
            instance_structured_output["steps"] = steps
            instance_structured_output["verifier_states_correct"] = bool(verifier_states_correct)
            instance_structured_output["act_correct"] = bool(act_correct)
            instance_structured_output["extracted_llm_plan"] = llm_plan
            instance_structured_output["context_window_hit"] = bool(context_window_hit)
            instance_structured_output["could_not_extract"] = bool(could_not_extract)
            instance_structured_output["feedback_messages"] = feedback_messages

            if not bool(act_correct):
                failed_instances += 1

            structured_output["instances"].append(instance_structured_output)
            self.save_json(task_name, structured_output)

        try:
            os.remove(self.plan_file)
            os.remove(self.gpt3_plan_file)
        except OSError:
            pass

        structured_output["failed_instances"] = failed_instances
        self.save_json(task_name, structured_output)
        final_output += (
            f"[+]: The number of correct plans is "
            f"{n_files - failed_instances}/{n_files}="
            f"{(n_files - failed_instances) / n_files * 100}%"
        )
        print(final_output)
        return structured_output

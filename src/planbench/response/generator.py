"""Unified ResponseGenerator for PlanBench.

Sends prompts to LLMs and collects responses. Supports:
- All 16 task types
- ThreadPoolExecutor parallelization (from llm_planning_analysis)
- Cost tracking for supported engines
- Retry-until-completion mode
- Uses planbench.llm abstraction layer (no direct SDK imports)
"""

from __future__ import annotations

import concurrent.futures
import json
import os
import time
from typing import Any

import yaml
from tqdm import tqdm

from planbench.llm import get_client, LLMClient
from planbench.utils.helpers import caesar_encode


# All canonical task names for both codebases
TASK_DICT: dict[str, str] = {
    # llm_planning_analysis tasks
    't1': 'task_1_plan_generation',
    't1_zero': 'task_1_plan_generation_zero_shot',
    't1_cot': 'task_1_plan_generation_state_tracking',
    't1_pddl': 'task_1_plan_generation_pddl',
    't1_zero_pddl': 'task_1_plan_generation_zero_shot_pddl',
    # plan-bench tasks
    't2': 'task_2_plan_optimality',
    't3': 'task_3_plan_verification',
    't3_1': 'task_3_plan_verification_with_llm_plans',
    't4': 'task_4_plan_reuse',
    't5': 'task_5_plan_generalization',
    't6': 'task_6_replanning',
    't7': 'task_7_plan_execution',
    't8_1': 'task_8_1_goal_shuffling',
    't8_2': 'task_8_2_full_to_partial',
    't8_3': 'task_8_3_partial_to_full',
}

# Cost per million tokens for supported engines
_COST_TABLE: dict[str, dict[str, float]] = {
    'o1-preview_chat': {'input': 15, 'output': 60},
}


class ResponseGenerator:
    """Generates LLM responses for PlanBench tasks."""

    def __init__(
        self,
        config_file: str,
        engine: str,
        verbose: bool = False,
        ignore_existing: bool = False,
    ) -> None:
        self.engine = engine
        self.verbose = verbose
        self.ignore_existing = ignore_existing
        self.max_gpt_response_length = 512
        self.data = self.read_config(config_file)

        # Handle finetuned engine format: "finetuned:<model_id>"
        self._model_id: str | None = None
        if 'finetuned' in self.engine:
            parts = self.engine.split(':')
            assert len(parts) >= 2, "finetuned engine format: finetuned:<model_id>"
            self._model_id = ':'.join(parts[1:])
            self.engine = 'finetuned'

        # Create the LLM client (lazy; no SDK loaded yet)
        kwargs: dict[str, Any] = {}
        if self._model_id:
            kwargs['model_id'] = self._model_id
        self._client: LLMClient = get_client(self.engine, **kwargs)

    def read_config(self, config_file: str) -> dict:
        with open(config_file, 'r') as file:
            return yaml.safe_load(file)

    def get_costs(self, input_tokens: int, output_tokens: int) -> tuple[float, float]:
        """Calculate input/output costs for the current engine."""
        if self.engine not in _COST_TABLE:
            return 0.0, 0.0
        rates = _COST_TABLE[self.engine]
        return (
            rates['input'] * input_tokens / 1e6,
            rates['output'] * output_tokens / 1e6,
        )

    def _query_instance(
        self,
        structured_output: dict,
        instance_id: int,
        specified_instances: list[int],
    ) -> tuple[str, int, float, Any]:
        """Send a single instance query to the LLM. Thread-safe."""
        instance = None
        for j in structured_output["instances"]:
            if j["instance_id"] == instance_id:
                instance = j
                break
        if instance is None:
            print(f"Instance {instance_id} not found in prompts")
            return "", instance_id, 0.0, None

        if "llm_raw_response" in instance:
            if instance["llm_raw_response"] and not self.ignore_existing:
                return "", instance_id, 0.0, None
        else:
            print(f"Getting response for instance {instance['instance_id']}")

        if len(specified_instances) > 0:
            if instance['instance_id'] not in specified_instances:
                return "", instance_id, 0.0, None

        if self.verbose:
            print(f"Sending query to LLM: Instance {instance['instance_id']}")

        query = instance["query"]
        stop_statement = "[STATEMENT]"
        if 'caesar' in self.data['domain_name']:
            stop_statement = caesar_encode(stop_statement)

        resp = self._client.query(
            query,
            self.max_gpt_response_length,
            stop=stop_statement,
        )

        if resp.null_response or not resp.text:
            print(f"Failed instance: {instance['instance_id']}")
            return "", instance_id, 0.0, None

        if self.verbose:
            print(f"LLM response: {resp.text}")

        return resp.text, instance_id, resp.time_taken, resp.raw_response

    def get_responses(
        self,
        task_name: str,
        max_workers: int = 1,
        specified_instances: list[int] | None = None,
        run_till_completion: bool = False,
    ) -> None:
        """Generate responses for all instances, optionally in parallel."""
        specified_instances = specified_instances or []
        output_dir = f"responses/{self.data['domain_name']}/{self.engine}/"
        os.makedirs(output_dir, exist_ok=True)
        output_json = output_dir + f"{task_name}.json"
        print(output_json)

        while True:
            # Load existing responses or prompts
            if os.path.exists(output_json):
                with open(output_json, 'r') as file:
                    structured_output = json.load(file)
                # Merge in any missing instances from prompts
                prompt_dir = f"prompts/{self.data['domain_name']}/"
                prompt_file = prompt_dir + f"{task_name}.json"
                assert os.path.exists(prompt_file), f"Prompt file {prompt_file} does not exist"
                with open(prompt_file, 'r') as file:
                    prompts = json.load(file)
                prompt_ids = {inst['instance_id'] for inst in prompts['instances']}
                output_ids = {inst['instance_id'] for inst in structured_output['instances']}
                missing = prompt_ids - output_ids
                structured_output['instances'] += [
                    inst for inst in prompts['instances'] if inst['instance_id'] in missing
                ]
            else:
                prompt_dir = f"prompts/{self.data['domain_name']}/"
                prompt_file = prompt_dir + f"{task_name}.json"
                assert os.path.exists(prompt_file), f"Prompt file {prompt_file} does not exist"
                with open(prompt_file, 'r') as file:
                    structured_output = json.load(file)
                structured_output['engine'] = self.engine

            failed_instances: list[int] = []
            start = self.data['start']
            end = self.data['end']

            if max_workers <= 1:
                # Sequential mode
                for instance in tqdm(structured_output["instances"]):
                    if "llm_raw_response" in instance:
                        if instance["llm_raw_response"] and not self.ignore_existing:
                            if self.verbose:
                                print(f"Instance {instance['instance_id']} already completed")
                            continue
                    if specified_instances and instance['instance_id'] not in specified_instances:
                        continue

                    if self.verbose:
                        print(f"Sending query to LLM: Instance {instance['instance_id']}")

                    query = instance["query"]
                    stop_statement = "[STATEMENT]"
                    if 'caesar' in self.data['domain_name']:
                        stop_statement = caesar_encode(stop_statement)

                    resp = self._client.query(
                        query,
                        self.max_gpt_response_length,
                        stop=stop_statement,
                    )

                    if resp.null_response or not resp.text:
                        failed_instances.append(instance['instance_id'])
                        print(f"Failed instance: {instance['instance_id']}")
                        continue
                    if self.verbose:
                        print(f"LLM response: {resp.text}")
                    instance["llm_raw_response"] = resp.text
                    with open(output_json, 'w') as file:
                        json.dump(structured_output, file, indent=4)
            else:
                # Parallel mode with ThreadPoolExecutor
                results: list[tuple[str, int, float, Any]] = []
                for i in range(start, end + 2, max_workers):
                    with concurrent.futures.ThreadPoolExecutor(max_workers=max_workers) as executor:
                        futures = [
                            executor.submit(
                                self._query_instance, structured_output, j, specified_instances,
                            )
                            for j in range(i, min(i + max_workers, end + 2))
                        ]
                        for future in concurrent.futures.as_completed(futures):
                            results.append(future.result())

                    for llm_response, inst_id, time_taken, resp_object in results:
                        instance = None
                        for j in structured_output["instances"]:
                            if j["instance_id"] == inst_id:
                                instance = j
                                break
                        if instance is None:
                            continue
                        if "llm_raw_response" in instance:
                            if instance["llm_raw_response"] and not self.ignore_existing:
                                continue
                        if llm_response == "":
                            continue

                        # Store response object
                        if isinstance(resp_object, dict):
                            instance["response_object"] = resp_object
                        elif isinstance(resp_object, str):
                            instance["response_object"] = {"response": resp_object}
                        elif resp_object is not None:
                            try:
                                instance["response_object"] = json.loads(resp_object.model_dump_json())
                            except (AttributeError, TypeError):
                                instance["response_object"] = {}
                        else:
                            instance["response_object"] = {}

                        instance["llm_raw_response"] = llm_response
                        instance["response_object"]["time_taken"] = time_taken

                        # Calculate costs
                        try:
                            usage = instance["response_object"].get("usage", {})
                            ic, oc = self.get_costs(
                                usage.get("prompt_tokens", 0),
                                usage.get("completion_tokens", 0),
                            )
                        except (KeyError, TypeError):
                            ic, oc = 0.0, 0.0
                        instance["response_object"]["costs"] = {
                            'input': ic, 'output': oc, 'total': ic + oc,
                        }
                        with open(output_json, 'w') as file:
                            json.dump(structured_output, file, indent=4)
                    results = []

            if run_till_completion:
                if len(failed_instances) == 0:
                    break
                print(f"Retrying failed instances: {failed_instances}")
                time.sleep(5)
            else:
                break

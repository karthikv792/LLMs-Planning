"""Shared base class for backprompting modules.

Contains utilities used by both sequential and parallel backprompters:
config I/O, PDDL helpers, plan computation, JSON persistence.
"""

from __future__ import annotations

import json
import os
import subprocess
from pathlib import Path
from typing import Any

import yaml
from tarski.io import PDDLReader

from planbench.executor import Executor
from planbench.llm import get_client, LLMClient


class BackPrompterBase:
    """Shared base for sequential and parallel backprompters."""

    def __init__(
        self,
        engine: str,
        verbose: bool = False,
        ignore_existing: bool = False,
    ) -> None:
        self.engine = engine
        self.verbose = verbose
        self.n_examples = 1
        self.ignore_existing = ignore_existing
        self.max_gpt_response_length = 500
        self.plan_file = "sas_plan"
        self.gpt3_plan_file = "gpt_sas_plan"

        # Create the LLM client lazily
        self._client: LLMClient = get_client(engine)
        self.model: Any | None = None
        self.data: dict = {}

    def compute_plan(self, domain: str, instance: str, timeout: int = 30) -> str:
        """Run Fast Downward to compute an optimal plan."""
        fast_downward_path = os.getenv("FAST_DOWNWARD")
        assert os.path.exists(f"{fast_downward_path}/fast-downward.py")
        cmd = f'timeout {timeout}s {fast_downward_path}/fast-downward.py {domain} {instance} --search "astar(lmcut())" > /dev/null 2>&1'
        os.system(cmd)
        if not os.path.exists(self.plan_file):
            return ""
        return Path(self.plan_file).read_text()

    def _compute_plan_optimal(self, domain: str, instance: str) -> dict[str, Any]:
        """Run Fast Downward and capture search statistics."""
        fast_downward_path = os.getenv("FAST_DOWNWARD")
        assert os.path.exists(f"{fast_downward_path}/fast-downward.py")
        cmd = f'{fast_downward_path}/fast-downward.py {domain} {instance} --search "astar(lmcut())"'
        process = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
        out, _err = process.communicate()
        dict_states: dict[str, Any] = {
            'expanded': None,
            'evaluated': None,
            'generated': None,
            'search_time (in secs)': None,
        }
        for out_line in out.decode("utf-8").split('\n'):
            if "Evaluated" in out_line:
                dict_states['evaluated'] = int(out_line.split(" ")[-2])
            elif "Expanded" in out_line:
                dict_states['expanded'] = int(out_line.split(" ")[-2])
            elif "Generated" in out_line:
                dict_states['generated'] = int(out_line.split(" ")[-2])
            elif "Search time" in out_line:
                dict_states['search_time (in secs)'] = float(out_line.split(" ")[-1][:-1])
        try:
            with open("sas_plan") as f:
                plan = [line.rstrip() for line in f][:-1]
                plan_length = len(plan)
        except FileNotFoundError:
            plan = []
            plan_length = 0
        return {
            "plan": '\n'.join(plan),
            "length": plan_length,
            "states_info": dict_states,
        }

    def read_config(self, config_file: str) -> None:
        with open(config_file, 'r') as file:
            self.data = yaml.safe_load(file)

    def get_problem(self, instance: str, domain: str) -> Any:
        reader = PDDLReader(raise_on_error=True)
        reader.parse_domain(domain)
        return reader.parse_instance(instance)

    def get_executor(self, instance: str, domain: str, ground: bool = True) -> Executor:
        return Executor(domain, instance, ground=ground)

    def save_json(self, output_file: str, structured_output: dict) -> None:
        os.makedirs(f"results/{self.data['domain_name']}/{self.engine}/json/", exist_ok=True)
        path = f"results/{self.data['domain_name']}/{self.engine}/json/{output_file}.json"
        with open(path, "w") as f:
            json.dump(structured_output, f, indent=4)

    def load_json(self, output_file: str) -> dict | None:
        path = f"results/{self.data['domain_name']}/{self.engine}/json/{output_file}.json"
        if os.path.exists(path) and not self.ignore_existing:
            with open(path, "r") as f:
                return json.load(f)
        return None

"""Shared base classes for PDDL instance generators.

Contains hash-based deduplication, PDDL validity checking via tarski,
and plan-length validation via Fast Downward.

From llm_planning_analysis/problem_generators.py.
"""

from __future__ import annotations

import hashlib
import json
import os
import random
from typing import Any

import yaml
from tarski.io import PDDLReader
from tarski.syntax.formulas import Tautology, Atom


class InstanceGeneratorBase:
    """Shared base for domain-specific instance generators."""

    def __init__(self, config_file: str, seed: int = 10) -> None:
        self.data: dict[str, Any] = {}
        self.read_config(config_file)
        self.instances_template = (
            f"./instances/{self.data['instance_dir']}/{self.data['instances_template']}"
        )
        self.label_json = f"./instances/{self.data['domain_name']}_all_labels.json"
        self.hashset: set[str] = set()
        self.plan_hashset: set[str] = set()
        if os.path.exists(self.label_json):
            with open(self.label_json, "r") as file:
                self.all_labels: dict = json.load(file)
        else:
            self.all_labels = {}
        random.seed(seed)
        self.seed = seed
        os.makedirs(f"./instances/{self.data['instance_dir']}/", exist_ok=True)

    def read_config(self, config_file: str) -> None:
        with open(config_file, "r") as file:
            self.data = yaml.safe_load(file)

    def instance_ok(self, domain: str, instance: str) -> bool:
        """Check that a PDDL instance has a non-trivial goal."""
        reader = PDDLReader(raise_on_error=True)
        reader.parse_domain(domain)
        reader.parse_instance(instance)
        if isinstance(reader.problem.goal, Tautology):
            return False
        elif isinstance(reader.problem.goal, Atom):
            if reader.problem.goal in reader.problem.init.as_atoms():
                return False
        else:
            if all(
                i in reader.problem.init.as_atoms()
                for i in reader.problem.goal.subformulas
            ):
                return False
        return True

    def convert_pddl(self, pddl: str) -> str:
        """Hash a PDDL instance by its sorted init+goal predicates."""
        init: list[str] = []
        goal: list[str] = []
        init_check = False
        goal_check = False
        for line in pddl.split("\n"):
            if "init" in line:
                init_check = True
                continue
            elif "goal" in line:
                goal_check = True
                init_check = False
                continue
            to_append = line.replace("(", "").replace(")", "")
            if to_append and "and" not in to_append:
                if init_check:
                    init.append(to_append)
                elif goal_check:
                    goal.append(to_append)
        pddl_to_hash = ",".join(sorted(init) + sorted(goal))
        return hashlib.md5(pddl_to_hash.encode("utf-8")).hexdigest()

    def add_existing_files_to_hash_set(self) -> tuple[int, list[int]]:
        """Populate hashset from existing dataset and instance files."""
        em: list[int] = []
        count = 0
        try:
            for _ in os.listdir(
                f"./dataset/{self.data['domain_name']}/{self.data['domain']}/"
            ):
                try:
                    f = open(
                        f"./dataset/{self.data['domain_name']}/{self.data['domain']}/instance-{count}.pddl",
                        "r",
                    )
                except Exception:
                    em.append(count)
                    count += 1
                    continue
                pddl = f.read()
                if pddl:
                    to_add = self.convert_pddl(pddl)
                    self.hashset.add(to_add)
                else:
                    em.append(count)
                count += 1
        except Exception:
            pass

        length = len(self.hashset)
        try:
            for i in os.listdir(f"./instances/{self.data['instance_dir']}/"):
                f = open(f"./instances/{self.data['instance_dir']}/{i}", "r")
                pddl = f.read()
                to_add = self.convert_pddl(pddl)
                if to_add in self.hashset:
                    print("OOPS")
                self.hashset.add(to_add)
        except FileNotFoundError:
            pass
        return length, em

    def plan_length_validity(
        self, domain: str, instance: str
    ) -> tuple[bool, str]:
        """Run Fast Downward and check that the plan has >= 3 steps."""
        fast_downward_path = os.getenv("FAST_DOWNWARD")
        assert os.path.exists(f"{fast_downward_path}/fast-downward.py")
        cmd = (
            f'timeout 20s {fast_downward_path}/fast-downward.py '
            f'{domain} {instance} --search "astar(lmcut())" > /dev/null'
        )
        os.system(cmd)
        plan_file = "sas_plan"
        try:
            with open(plan_file) as f:
                plan = [line.rstrip() for line in f][:-1]
                readable_plan = "\n".join(plan)
            os.remove(plan_file)
            print(len(plan))
            return (True, readable_plan) if len(plan) >= 3 else (False, "")
        except FileNotFoundError:
            print("No plan found")
            return (False, "")

    def gen_goal_directed_instances(
        self, n_instances: int, max_objs: int = 5
    ) -> None:
        """Dispatch to the domain-specific generator (override in subclass)."""
        raise NotImplementedError(
            "Subclasses must implement gen_goal_directed_instances"
        )


class GeneralizationGeneratorBase:
    """Shared base for task-5 generalization instance generators."""

    def __init__(self, config_file: str) -> None:
        random.seed(10)
        self.data = self.read_config(config_file)
        self.instances_template_t5 = (
            f"./instances/{self.data['generalized_instance_dir']}"
            f"/{self.data['instances_template']}"
        )
        self.hashset: set[str] = set()
        self.instances: list = []
        os.makedirs(
            f"./instances/{self.data['generalized_instance_dir']}/", exist_ok=True
        )

    def read_config(self, config_file: str) -> dict:
        with open(config_file, "r") as file:
            return yaml.safe_load(file)

    def add_existing_files_to_hash_set(
        self, instance_dir: str | None = None
    ) -> int:
        for i in os.listdir(f"./instances/{instance_dir}/"):
            f = open(f"./instances/{instance_dir}/{i}", "r")
            pddl = f.read()
            self.hashset.add(hashlib.md5(pddl.encode("utf-8")).hexdigest())
        return len(self.hashset)

    def instance_ok(self, domain: str, instance: str) -> bool:
        reader = PDDLReader(raise_on_error=True)
        reader.parse_domain(domain)
        reader.parse_instance(instance)
        if isinstance(reader.problem.goal, Tautology):
            return False
        elif isinstance(reader.problem.goal, Atom):
            if reader.problem.goal in reader.problem.init.as_atoms():
                return False
        else:
            if all(
                i in reader.problem.init.as_atoms()
                for i in reader.problem.goal.subformulas
            ):
                return False
        return True

    def t5_gen_generalization_instances(self, n_instances: int = 0) -> None:
        raise NotImplementedError(
            "Subclasses must implement t5_gen_generalization_instances"
        )

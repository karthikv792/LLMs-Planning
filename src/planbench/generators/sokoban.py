"""Sokoban instance generator.

From llm_planning_analysis/problem_generators.py.
"""

from __future__ import annotations

import os
import random

from .base import InstanceGeneratorBase


class SokobanInstanceGenerator(InstanceGeneratorBase):
    """Generate goal-directed sokoban instances."""

    def gen_goal_directed_instances(
        self, n_instances: int, max_objs: int = 5
    ) -> None:
        CMD = (
            "timeout 2s ./pddlgenerators/sokoban/random/sokoban-generator-typed"
            " -n {} -b {} -w {} -s" + f" {self.seed}"
        )
        start, missing = self.add_existing_files_to_hash_set()
        c = missing.pop() if missing else start
        if n_instances:
            n = n_instances
        else:
            n = self.data["n_instances"] + 1
        while True:
            if c > n:
                break
            n_grid = random.randint(4, 10)
            n_boxes = random.randint(1, 4)
            n_walls = random.randint(1, 4)
            cmd_exec = CMD.format(n_grid, n_boxes, n_walls)
            pddl = os.popen(cmd_exec).read()
            if "(define" not in pddl:
                continue
            pddl = [i for i in pddl.split("\n") if not i.startswith(";")]
            pddl_str = "\n".join(pddl).strip()
            hash_of_instance = self.convert_pddl(pddl_str)
            if hash_of_instance in self.hashset:
                continue
            self.hashset.add(hash_of_instance)
            with open(self.instances_template.format(c), "w+") as fd:
                fd.write(pddl_str)
            c += 1
            print(f"[+]: Instance created. Total instances: {c}")

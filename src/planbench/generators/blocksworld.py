"""Blocksworld instance generators.

From llm_planning_analysis/problem_generators.py.
"""

from __future__ import annotations

import hashlib
import os
import random

from .base import InstanceGeneratorBase, GeneralizationGeneratorBase


class BlocksworldInstanceGenerator(InstanceGeneratorBase):
    """Generate goal-directed blocksworld instances."""

    def gen_goal_directed_instances(
        self, n_instances: int, max_objs: int = 5
    ) -> None:
        if n_instances:
            n = n_instances
        else:
            n = self.data["n_instances"] + 1
        n_objs = range(3, max_objs + 1)
        CWD = os.getcwd()
        CMD = "./blocksworld 4 {}"
        start, missing = self.add_existing_files_to_hash_set()

        os.chdir("pddlgenerators/blocksworld/")
        instance_file = f"{CWD}/{self.instances_template}"
        domain = f"{CWD}/instances/{self.data['domain_file']}"
        print(missing)
        c = missing.pop() if missing else start
        for obj in n_objs:
            print(f"==================== Number of blocks {obj} ====================")
            count = 0
            cmd_exec = CMD.format(obj)
            if c > n:
                break
            while count < 50:
                with open(instance_file.format(c), "w+") as fd:
                    pddl = os.popen(cmd_exec).read()
                    hash_of_instance = self.convert_pddl(pddl)
                    if hash_of_instance in self.hashset:
                        count += 1
                        continue
                    count = 0
                    self.hashset.add(hash_of_instance)
                    fd.write(pddl)

                inst_to_parse = instance_file.format(c)
                if self.instance_ok(domain, inst_to_parse):
                    if missing:
                        c = missing.pop()
                    else:
                        if c < start:
                            c = start
                        else:
                            c += 1
                    print(f"[+]: Instance created. Total instances: {c}")
                else:
                    self.hashset.remove(hash_of_instance)
                    os.remove(inst_to_parse)
                    continue

        print(f"[+]: A total of {c} instances have been generated")
        os.chdir(CWD)


class BlocksworldGeneralizationGenerator(GeneralizationGeneratorBase):
    """Generate task-5 generalization instances for blocksworld."""

    def t5_gen_generalization_instances(self, n_instances: int = 0) -> None:
        def gen_instance(objs: list[str]) -> str:
            text = "(define (problem BW-generalization-4)\n(:domain blocksworld-4ops)"
            text += "(:objects " + " ".join(objs) + ")\n"
            text += "(:init \n(handempty)\n"
            for obj in objs:
                text += f"(ontable {obj})\n"
            for obj in objs:
                text += f"(clear {obj})\n"
            text += ")\n(:goal\n(and\n"
            obj_tuples = list(zip(objs, objs[1:]))
            for i in obj_tuples:
                text += f"(on {i[0]} {i[1]})\n"
            text += ")))"
            return text

        if n_instances:
            n = n_instances
        else:
            n = self.data["n_instances"] + 2
        objs = self.data["encoded_objects"]
        encoded_objs = list(objs.keys())
        start = self.add_existing_files_to_hash_set(
            self.data["generalized_instance_dir"]
        )

        print("[+]: Making generalization instances for blocksworld")
        for c in range(start, n):
            n_objs = random.randint(3, len(objs))
            random.shuffle(encoded_objs)
            objs_instance = encoded_objs[:n_objs]
            instance = gen_instance(objs_instance)

            if hashlib.md5(instance.encode("utf-8")).hexdigest() in self.hashset:
                print("INSTANCE ALREADY IN SET, SKIPPING")
                continue

            with open(self.instances_template_t5.format(c), "w+") as fd:
                fd.write(instance)

"""Logistics instance generators.

From llm_planning_analysis/problem_generators.py.
"""

from __future__ import annotations

import hashlib
import json
import os
import random

from .base import InstanceGeneratorBase, GeneralizationGeneratorBase


class LogisticsInstanceGenerator(InstanceGeneratorBase):
    """Generate goal-directed logistics instances."""

    def add_objects_logistics(
        self,
        cities: int,
        airplanes: int,
        packages: int,
        city_size: int,
    ) -> tuple[int, int, int, int]:
        if airplanes + 1 > cities:
            airplanes = 1
            if packages + 1 > city_size * cities:
                packages = 1
                if city_size + 1 > 3:
                    city_size = 1
                    if cities + 1 > 5:
                        cities = 1
                    else:
                        cities += 1
                else:
                    city_size += 1
            else:
                packages += 1
        else:
            airplanes += 1
        return cities, airplanes, packages, city_size

    def gen_goal_directed_instances(
        self, n_instances: int, max_objs: int = 5
    ) -> None:
        CWD = os.getcwd()
        CMD = "./pddlgenerators/logistics/logistics -a {} -c {} -s {} -p {}"
        all_instances: list[str] = []

        instance_file = f"{CWD}/{self.instances_template}"
        domain = f"{CWD}/instances/{self.data['domain_name']}/{self.data['domain_file']}"
        start, missing = self.add_existing_files_to_hash_set()
        c = missing.pop() if missing else start

        if n_instances:
            n = n_instances
        else:
            n = self.data["n_instances"] + 1
        cities = 2
        airplanes = 1
        city_size = 1
        packages = 1
        while True:
            if c > n:
                break
            instance_label = f"{cities}-{airplanes}-{city_size}-{packages}"
            if instance_label in all_instances:
                continue
            print("[INFO]: Instance label: ", instance_label)
            cmd_exec = CMD.format(airplanes, cities, city_size, packages)
            count = 0
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
                print(self.plan_length_validity(domain, inst_to_parse))
                isvalid, plan = self.plan_length_validity(domain, inst_to_parse)
                if isvalid:
                    plan_hash = plan
                    if plan_hash in self.plan_hashset:
                        print("[-]: Same plan, skipping...")
                        self.hashset.remove(hash_of_instance)
                        os.remove(inst_to_parse)
                        continue
                    self.plan_hashset.add(plan_hash)
                    if instance_label in self.all_labels:
                        self.all_labels[instance_label].append(c)
                    else:
                        self.all_labels[instance_label] = [c]
                    if missing:
                        c = missing.pop()
                    else:
                        if c < start:
                            c = start
                        else:
                            c += 1
                else:
                    self.hashset.remove(hash_of_instance)
                    os.remove(inst_to_parse)
                    continue

            all_instances.append(instance_label)
            with open(f"{self.data['domain_name']}_all_labels.json", "w") as file:
                json.dump(self.all_labels, file)
            cities, airplanes, packages, city_size = self.add_objects_logistics(
                cities, airplanes, packages, city_size
            )

        print(f"[+]: A total of {c} instances have been generated")
        os.chdir(CWD)


class LogisticsGeneralizationGenerator(GeneralizationGeneratorBase):
    """Generate task-5 generalization instances for logistics."""

    def t5_gen_generalization_instances(self, n_instances: int = 0) -> None:
        def gen_instance(
            init: list[str], goal: list[str], objs: list[str]
        ) -> str:
            text = "(define (problem LG-generalization)\n(:domain logistics-strips)"
            text += "(:objects " + " ".join(objs) + ")\n"
            text += "(:init \n"
            text += "\n".join(init)
            text += "\n"
            text += ")\n(:goal\n(and\n"
            text += "\n".join(goal)
            text += "\n"
            text += ")))"
            return text

        if n_instances:
            n = n_instances
        else:
            n = self.data["n_instances"] + 1
        start = (
            self.add_existing_files_to_hash_set(self.data["generalized_instance_dir"])
            + 1
        )
        print("[+]: Making generalization instances for logistics")
        c = start
        while c < n:
            cities = list(range(random.randint(1, 3)))
            locations = list(range(random.randint(3, 10)))
            packages = list(range(random.randint(2, len(locations))))
            random.shuffle(cities)
            random.shuffle(locations)
            random.shuffle(packages)
            init: list[str] = []
            goal: list[str] = []
            objs: list[str] = []
            airports: dict[int, tuple[int, int]] = {}
            for city in cities:
                init.append(f"(CITY c{city})")
                init.append(f"(TRUCK t{city})")
                init.append(f"(AIRPLANE a{city})")
                objs += [f"c{city}", f"t{city}", f"a{city}"]
                pack_done = 0
                for location in locations:
                    init.append(f"(LOCATION l{city}-{location})")
                    init.append(f"(in-city l{city}-{location} c{city})")
                    objs.append(f"l{city}-{location}")
                    if pack_done < len(packages):
                        to_mul = city * len(packages)
                        init.append(f"(OBJ p{to_mul + packages[pack_done]})")
                        objs.append(f"p{to_mul + packages[pack_done]}")
                        if pack_done == 0:
                            init.append(
                                f"(at p{to_mul + packages[pack_done]} l{city}-{location})"
                            )
                            init.append(f"(at t{city} l{city}-{location})")
                        else:
                            init.append(
                                f"(at p{to_mul + packages[pack_done]} l{city}-{location})"
                            )
                            goal.append(
                                f"(at p{to_mul + packages[pack_done - 1]} l{city}-{location})"
                            )
                        pack_done += 1
                    airports[city] = (location, packages[pack_done - 1])
            for city, v in airports.items():
                location, package = v
                init.append(f"(AIRPORT l{city}-{location})")
                init.append(f"(at a{city} l{city}-{location})")
                if len(cities) > 1:
                    fly_to = random.choice(list(airports.keys()))
                    while fly_to == city:
                        fly_to = random.choice(list(airports.keys()))
                    to_mul = city * len(packages)
                    goal.append(
                        f"(at p{to_mul + package} l{fly_to}-{airports[fly_to][0]})"
                    )

            instance = gen_instance(init, goal, objs)

            if hashlib.md5(instance.encode("utf-8")).hexdigest() in self.hashset:
                print("[-] INSTANCE ALREADY IN SET, SKIPPING")
                continue

            with open(self.instances_template_t5.format(c), "w+") as fd:
                fd.write(instance)
            c += 1

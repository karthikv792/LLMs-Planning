import hashlib
import os
os.chdir("instances/generated_basic/")
instances = sorted(os.listdir("./"))
hashset  = set()
for i in instances:
    print(i)
    f=open(i,"r")
    pddl = f.read()
    hash_inst = hashlib.md5(pddl.encode('utf-8')).hexdigest()
    if hash_inst in hashset:
        print("ALREADY THERE")
    hashset.add(hash_inst)
print("DONE")
# from tarski.io import PDDLReader
# from tarski.syntax.formulas import *
# from utils import *
# reader = PDDLReader(raise_on_error=True)
# domain = "./pr-domain.pddl"
# reader.parse_domain(domain)
# instance = "./pr-problem.pddl"
# reader.parse_instance(instance)
# if isinstance(reader.problem.goal, Tautology):
#     print("T")
# elif isinstance(reader.problem.goal, Atom):
#     if reader.problem.goal in reader.problem.init.as_atoms():
#         print("Goal satisfied")
# else:
#     print(all([i in reader.problem.init.as_atoms() for i in reader.problem.goal.subformulas]))
#
# from model_parser.parser_new import parse_model
# from model_parser.writer_new import ModelWriter
# from model_parser.constants import *
#
# model = parse_model(domain, instance)
# writer = ModelWriter(model)
# writer.write_files("new_pr_domain.pddl", "new_pr_problem.pddl")
#
# import os
# from pathlib import Path
#
# def compute_plan(domain, instance, timeout=30):
#     plan_file = "./sas_plan"
#     fast_downward_path = os.getenv("FAST_DOWNWARD")
#     cmd = f"timeout {timeout}s {fast_downward_path}/fast-downward.py {domain} {instance} --search \"astar(lmcut())\" > /dev/null 2>&1"
#     os.system(cmd)
#
#     if not os.path.exists(plan_file):
#         return ""
#     return Path(plan_file).read_text()
#
# print(compute_plan("./new_pr_domain.pddl", "./new_pr_problem.pddl"))
# print(validate_plan("./new_pr_domain.pddl", "./new_pr_problem.pddl", "./sas_plan"))

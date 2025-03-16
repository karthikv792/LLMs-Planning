import os
import random

import yaml
from Executor import Executor
from utils import *
from pathlib import Path
from tarski.io import PDDLReader
import argparse
import time
import json
import string
import subprocess
from tqdm import tqdm
from rich import print
"""
TODO: Mystery Generalized Instances
TODO: plan 
"""

class PromptGenerator:
    def __init__(self,config_file, verbose, ignore_existing, seed) -> None:
        self.n_examples = 1
        self.output_dir = "prompts"
        self.verbose = verbose
        self.ignore_existing = ignore_existing
        self.plan_file = "sas_plan"
        self.data = self.read_config(config_file)
        self.instance_dir = self.data['instance_dir']
        self.domain_pddl = f'./instances/{self.data["domain_file"]}'
        self._set_task_params()
        self._set_seed(seed)

    def _set_seed(self, seed):
        np.random.seed(seed)
        random.seed(seed)

    def _set_task_params(self, instance_dir=None):
        if instance_dir is None:
            instance_dir = self.instance_dir
        else:
            self.instance_dir = instance_dir
        self.instance_folder = f'./instances/{instance_dir}/'
        self.instance = f'./instances/{instance_dir}/{self.data["instances_template"]}'
        self.n_files = min(self.data['n_instances'], len(os.listdir(self.instance_folder)))

        self.i_start = self.data['start']
        self.i_end = self.data['end']
    
    def compute_plan(self, domain, instance):
        fast_downward_path = os.getenv("FAST_DOWNWARD")
        # Remove > /dev/null to see the output of fast-downward
        assert os.path.exists(f"{fast_downward_path}/fast-downward.py")
        cmd = f"{fast_downward_path}/fast-downward.py {domain} {instance} --search \"astar(lmcut())\" > /dev/null 2>&1"
        os.system(cmd)

        if not os.path.exists(self.plan_file):
            return ""
        return Path(self.plan_file).read_text()
    
    def _compute_plan_optimal(self,domain, instance):
        fast_downward_path = os.getenv("FAST_DOWNWARD")
        # Remove > /dev/null to see the output of fast-downward
        assert os.path.exists(f"{fast_downward_path}/fast-downward.py")
        cmd = f"{fast_downward_path}/fast-downward.py {domain} {instance} --search \"astar(lmcut())\""
        # cmd = f"{fast_downward_path}/fast-downward.py --alias seq-sat-lama-2011 {domain} {instance}> /dev/null 2>&1"
        # print(cmd)
        process = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
        # Wait till the process is complete
        out, err = process.communicate()
        dict_states ={
            'expanded': None,
            'evaluated': None,
            'generated': None,
            'search_time (in secs)': None
        }
        #[t=0.619228s, 11116 KB] Evaluated 12278 state(s).
        #[t=0.527384s, 11116 KB] Search time: 0.520411s
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
        dict_plan = {
            "plan": '\n'.join(plan),
            "length": plan_length,
            "states_info": dict_states
        }
        return dict_plan

    def read_config(self, config_file):
        with open(config_file, 'r') as file:
            return yaml.safe_load(file)

    def get_problem(self, instance, domain):
        reader = PDDLReader(raise_on_error=True)
        reader.parse_domain(domain)
        return reader.parse_instance(instance)

    def get_executor(self, instance, domain, ground=False):
        plan_executor = Executor(domain, instance, ground=ground)
        return plan_executor

    def save_json(self, output_file, structured_output):
        os.makedirs(f"{self.output_dir}/{self.data['domain_name']}/", exist_ok=True)
        with open(f"{self.output_dir}/{self.data['domain_name']}/" + output_file + ".json", "w") as f:
            json.dump(structured_output, f, indent=4)
    
    def load_json(self, output_file, output_dir=None):
        if output_dir is None:
            output_dir = self.output_dir
        if self.ignore_existing:
            return None
        if os.path.exists(f"{output_dir}/{self.data['domain_name']}/" + output_file + ".json"):
            with open(f"{output_dir}/{self.data['domain_name']}/" + output_file + ".json", "r") as f:
                return json.load(f)
        else:
            return None
    def load_results_json(self, output_file):
        output_dir = "results"
        engine = "gpt-4_chat"
        assert os.path.exists(f"{output_dir}/{self.data['domain_name']}/{engine}/" + output_file + ".json"), "File does not exist"
        with open(f"{output_dir}/{self.data['domain_name']}/{engine}/" + output_file + ".json", "r") as f:
            return json.load(f)
        
    
        # ========================================== TASKS ========================================== #
    def task_1_plan_generation(self, specified_instances=[], random_example=False):
        task_name = f"task_1_plan_generation"
        instance_structured_outputs = []
        structured_output = self.load_json(task_name)
        completed_instances =  []
        
        if structured_output is None:
            structured_output = {
                                "task": task_name,
                                "prompt_type": "oneshot",
                                "domain": self.data['domain_name'],
                                "instances": instance_structured_outputs,
                                }
        for inst in structured_output["instances"]:
            if inst["query"]:
                completed_instances.append(inst["instance_id"])
        if len(specified_instances):
            range_list = []
            for specified_instance in specified_instances:
                range_list.append(specified_instance - self.n_examples)
        else:
            range_list = range(self.i_start, self.i_end + 2 - self.n_examples)
        
        for start in tqdm(range_list):
            query = self.data["domain_intro"]
            instance_structured_output = {}
            examples = []
            for i in range(start, start + self.n_examples + 1):
                last_plan = True if i == start + self.n_examples else False
                get_plan = not last_plan                
                if last_plan:
                    cur_instance = self.instance.format(i)
                    instance_structured_output["instance_id"] = i

                else:
                    if random_example:
                        new_i = random.choice([ln for ln in range(1,self.n_files) if ln != i])
                        cur_instance = self.instance.format(new_i)
                        examples.append(new_i)
                    else:
                        cur_instance = self.instance.format(i)
                        examples.append(i)
                
                if self.verbose:
                    print(f"Instance {cur_instance}")
                # --------------- Read Instance --------------- #
                problem = self.get_problem(cur_instance, self.domain_pddl)
                # --------------------------------------------- #
                # ------------ Put plan and instance into text ------------ #
                gt_plan = self._compute_plan_optimal(self.domain_pddl, cur_instance)
                gt_plan_text = get_plan_as_text(self.data)
                gt_plan["readable_plan"] = gt_plan_text
                query += fill_template(*instance_to_text(problem, get_plan, self.data))
                # --------------------------------------------------------- #
                
            if self.verbose:
                print(query)

            stop_statement = '[STATEMENT]'
            # Querying LLM
            if 'caesar' in self.data['domain_name']:
                query = caesar_encode(query)
                stop_statement = caesar_encode(stop_statement)
            if i in completed_instances:
                continue
            instance_structured_output["example_instance_ids"] = examples
            instance_structured_output["query"] = query
            instance_structured_output["ground_truth_plan"] = gt_plan
            structured_output["instances"].append(instance_structured_output)
            self.save_json(task_name, structured_output)
    
    def task_1_plan_generation_zero_shot(self, specified_instances=[], random_example=False):
        task_name = f"task_1_plan_generation_zero_shot"
        instance_structured_outputs = []
        structured_output = self.load_json(task_name)
        completed_instances =  []
        
        if structured_output is None:
            structured_output = {
                                "task": task_name,
                                "prompt_type": "zeroshot",
                                "domain": self.data['domain_name'],
                                "instances": instance_structured_outputs,
                                }
        for inst in structured_output["instances"]:
            if inst["query"]:
                completed_instances.append(inst["instance_id"])
        if len(specified_instances):
            range_list = specified_instances
        else:
            range_list = range(self.i_start, self.i_end + 1)
        
        for start in tqdm(range_list):
            if "domain_intro_zero_shot" in self.data:
                query = self.data["domain_intro_zero_shot"]
            else:
                query = self.data["domain_intro"]
            get_plan = False  
            instance_structured_output = {}
            cur_instance = self.instance.format(start)
            if start in completed_instances:
                continue
            instance_structured_output["instance_id"] = start        
                
            if self.verbose:
                print(f"Instance {cur_instance}")
            # --------------- Read Instance --------------- #
            problem = self.get_problem(cur_instance, self.domain_pddl)
            # --------------------------------------------- #
            # ------------ Put plan and instance into text ------------ #
            gt_plan = self.compute_plan(self.domain_pddl, cur_instance)
            if "unsolvable" in self.data["domain_name"]:
                gt_plan_text = "unsolvable"
            else:
                gt_plan_text = get_plan_as_text(self.data)
            query += fill_template(*instance_to_text(problem, get_plan, self.data), instruction=True)
            # --------------------------------------------------------- #
            if self.verbose:
                print(query)
            instance_structured_output["query"] = query
            instance_structured_output["ground_truth_plan"] = gt_plan_text
            structured_output["instances"].append(instance_structured_output)
            self.save_json(task_name, structured_output)

   
    def task_1_plan_generation_state_tracking(self, specified_instances=[], random_example=False):
        task_name = f"task_1_plan_generation_state_tracking"
        instance_structured_outputs = []
        structured_output = self.load_json(task_name)
        completed_instances =  []
        
        if structured_output is None:
            structured_output = {
                                "task": task_name,
                                "prompt_type": "oneshot",
                                "domain": self.data['domain_name'],
                                "instances": instance_structured_outputs,
                                }
        for inst in structured_output["instances"]:
            if inst["query"]:
                completed_instances.append(inst["instance_id"])
        if len(specified_instances):
            range_list = []
            for specified_instance in specified_instances:
                range_list.append(specified_instance - self.n_examples)
        else:
            range_list = range(self.i_start, self.i_end + 2 - self.n_examples)
        
        for start in tqdm(range_list):
            query = self.data["domain_intro_state_tracking"]
            instance_structured_output = {}
            examples = []
            for i in range(start, start + self.n_examples + 1):
                last_plan = True if i == start + self.n_examples else False
                get_plan = not last_plan                
                if last_plan:
                    cur_instance = self.instance.format(i)
                    instance_structured_output["instance_id"] = i

                else:
                    if random_example:
                        new_i = random.choice([ln for ln in range(1,self.n_files) if ln != i])
                        cur_instance = self.instance.format(new_i)
                        examples.append(new_i)
                    else:
                        cur_instance = self.instance.format(i)
                        examples.append(i)
                
                if self.verbose:
                    print(f"Instance {cur_instance}")
                # --------------- Read Instance --------------- #
                problem = self.get_problem(cur_instance, self.domain_pddl)
                # --------------------------------------------- #
                plan_executor = self.get_executor(cur_instance, self.domain_pddl)
                # ------------ Put plan and instance into text ------------ #
                gt_plan = self.compute_plan(self.domain_pddl, cur_instance)
                gt_plan_text = get_plan_as_text(self.data)
                instance_text, _ = generate_plan_cot(plan_executor, self.data, get_plan)
                query += instance_text
                # --------------------------------------------------------- #
        
            if self.verbose:
                print(query)
            if i in completed_instances:
                continue
            instance_structured_output["example_instance_ids"] = examples
            instance_structured_output["query"] = query
            instance_structured_output["ground_truth_plan"] = gt_plan_text
            structured_output["instances"].append(instance_structured_output)
            self.save_json(task_name, structured_output)

    def task_1_plan_generation_pddl(self, specified_instances=[], random_example=False):
        task_name = f"task_1_plan_generation_pddl"
        instance_structured_outputs = []
        structured_output = self.load_json(task_name)
        completed_instances =  []
        
        if structured_output is None:
            structured_output = {
                                "task": task_name,
                                "prompt_type": "oneshot",
                                "domain": self.data['domain_name'],
                                "instances": instance_structured_outputs,
                                }
        for inst in structured_output["instances"]:
            if inst["query"]:
                completed_instances.append(inst["instance_id"])
        if len(specified_instances):
            range_list = []
            for specified_instance in specified_instances:
                range_list.append(specified_instance - self.n_examples)
        else:
            range_list = range(self.i_start, self.i_end + 2 - self.n_examples)
        
        for start in tqdm(range_list):
            with open(self.domain_pddl, 'r') as file:
                domain_intro = file.read()
            query = "Here is a pddl domain, an example problem and it's corresponding plan. Provide the plan for the query problem. Provide only the pddl syntax for the plan.\n"
            query += "[DOMAIN]\n" + domain_intro.strip() + "\n\n"
            instance_structured_output = {}
            examples = []
            for i in range(start, start + self.n_examples + 1):
                last_plan = True if i == start + self.n_examples else False
                get_plan = not last_plan                
                if last_plan:
                    cur_instance = self.instance.format(i)
                    instance_structured_output["instance_id"] = i

                else:
                    if random_example:
                        new_i = random.choice([ln for ln in range(1,self.n_files) if ln != i])
                        cur_instance = self.instance.format(new_i)
                        examples.append(new_i)
                    else:
                        cur_instance = self.instance.format(i)
                        examples.append(i)
                
                if self.verbose:
                    print(f"Instance {cur_instance}")
                # --------------- Read Instance --------------- #
                with open(cur_instance, 'r') as file:
                    problem = file.read()
                # --------------------------------------------- #
                # ------------ Put plan and instance into text ------------ #
                plan = self.compute_plan(self.domain_pddl, cur_instance)
                if get_plan:
                    query+="[PROBLEM]\n"+problem.strip()+"\n\n"
                    query+="[PLAN]\n"+plan.strip()+"\n[PLAN_END]\n\n"
                    examples.append(i)
                else:
                    query+="[QUERY PROBLEM]\n"+problem.strip()+"\n\n"
                # --------------------------------------------------------- #
                
            if self.verbose:
                print(query)

            stop_statement = '[STATEMENT]'
            # Querying LLM
            if 'caesar' in self.data['domain_name']:
                query = caesar_encode(query)
                stop_statement = caesar_encode(stop_statement)
            if i in completed_instances:
                continue
            instance_structured_output["example_instance_ids"] = examples
            instance_structured_output["query"] = query
            instance_structured_output["ground_truth_plan"] = plan
            structured_output["instances"].append(instance_structured_output)
            self.save_json(task_name, structured_output)

    def task_1_plan_generation_zero_shot_pddl(self, specified_instances=[], random_example=False):
        task_name = f"task_1_plan_generation_zero_shot_pddl"
        instance_structured_outputs = []
        structured_output = self.load_json(task_name)
        completed_instances =  []
        
        if structured_output is None:
            structured_output = {
                                "task": task_name,
                                "prompt_type": "oneshot",
                                "domain": self.data['domain_name'],
                                "instances": instance_structured_outputs,
                                }
        for inst in structured_output["instances"]:
            if inst["query"]:
                completed_instances.append(inst["instance_id"])
        if len(specified_instances):
            range_list = specified_instances
        else:
            range_list = range(self.i_start, self.i_end + 1)
        
        for start in tqdm(range_list):
            if start in completed_instances:
                continue
            # Create a 6 letter random alphanumeric lowercase string
            alphanum = ''.join(random.choices(string.ascii_lowercase + string.digits, k=6))
            with open(self.domain_pddl, 'r') as file:
                domain_intro = file.read()
            query = "The following is a PDDL specification of a planning problem. The first part, under the heading [DOMAIN], is the domain file. The second part, under the heading [QUERY PROBLEM], is the problem file. Using this information, which is correct, and no further assumptions, find a plan which, when run from the specified initial state, satisfies the specified goal. Provide your answer as a sequence of actions in PDDL format. An action ACTION which acts on two objects OBJ1 and OBJ2 would be written (ACTION OBJ1 OBJ2). Do not provide anything else in your answer.\n"
            query += "[DOMAIN]\n" + domain_intro.strip() + "\n\n"
            instance_structured_output = {}
            cur_instance = self.instance.format(start)
            # --------------- Read Instance --------------- #
            with open(cur_instance, 'r') as file:
                problem = file.read()
            problem = "(define" + problem.split("(define")[1][:-1].strip() + "\n)"
            

            

            # --------------- Get Plan --------------- #
            plan = {}#self._compute_plan_optimal(self.domain_pddl, cur_instance)
            query+="[QUERY PROBLEM]\n"+problem.strip()+"\n\n[PLAN]"
            # HACK TO FIX
            query = query.replace("obfuscated_randomized_blocksworld", alphanum)
            
            if self.verbose:
                print(query)

            stop_statement = '[STATEMENT]'
            # Querying LLM
            if 'caesar' in self.data['domain_name']:
                query = caesar_encode(query)
                stop_statement = caesar_encode(stop_statement)
            instance_structured_output["query"] = query
            instance_structured_output["instance_id"] = start
            instance_structured_output["ground_truth_plan"] = plan
            structured_output["instances"].append(instance_structured_output)
            self.save_json(task_name, structured_output)
if __name__=="__main__":
    random.seed(10)
    parser = argparse.ArgumentParser()
    parser.add_argument('--task', type=str, required=True, help='Task to run \
    \n t1 = Plan Generation\
    \n t1_zero = Zero Shot Plan Generation\
    \n t1_cot = Plan Generation COT\
    \n t1_pddl = Plan Generation PDDL\
    \n t1_zero_pddl = Zero Shot Plan Generation PDDL\
    ')
    parser.add_argument('--verbose', type=str, default="False", help='Verbose')
    #config
    parser.add_argument('--config', type=str, required=True, help='Config file name (no need to add .yaml)')
    parser.add_argument('--specific_instances', nargs='+', type=int, default=[], help='List of instances to run')
    parser.add_argument('--random_example', type=str, default="False", help='Random example')
    parser.add_argument('--ignore_existing', action='store_true', help='Ignore existing output')
    parser.add_argument('--seed', type=int, default=42, help='Random seed')
    args = parser.parse_args()
    task = args.task
    config = args.config
    verbose = eval(args.verbose)
    specified_instances = args.specific_instances
    random_example = eval(args.random_example)
    ignore_existing = args.ignore_existing
    seed = args.seed
    # print(task, config, verbose, specified_instances, random_example)
    config_file = f'./configs/{config}.yaml'
    assert os.path.exists(config_file), f"Config file {config_file} does not exist"
    prompt_generator = PromptGenerator(config_file, verbose, ignore_existing, seed)
    if task == 't1':
        prompt_generator.task_1_plan_generation(specified_instances, random_example)
    elif task == 't1_zero':
        prompt_generator.task_1_plan_generation_zero_shot(specified_instances, random_example)
    elif task == 't1_cot':
        prompt_generator.task_1_plan_generation_state_tracking(specified_instances, random_example)
    elif task == 't1_pddl':
        prompt_generator.task_1_plan_generation_pddl(specified_instances, random_example)
    elif task == 't1_zero_pddl':
        prompt_generator.task_1_plan_generation_zero_shot_pddl(specified_instances, random_example)
    else:
        raise NotImplementedError




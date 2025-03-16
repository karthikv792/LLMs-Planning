import os
import random
import cProfile
import yaml
from Executor import Executor
from utils import *
from pathlib import Path
from tarski.io import PDDLReader
from full_validator import *
import argparse
import time
from transformers import AutoTokenizer, AutoModelForCausalLM, AutoModel
import json
import subprocess
import concurrent.futures
from tqdm import tqdm
from rich import pretty
from rich.progress import Progress, SpinnerColumn, BarColumn, TextColumn
from rich import print
from threading import Lock
pretty.install()
import string
import pstats
class BackPrompter():
    def __init__(self, engine, verbose, ignore_existing):
        self.engine = engine
        self.verbose = verbose
        self.n_examples = 1
        self.ignore_existing = ignore_existing
        self.max_gpt_response_length = 500

        self.plan_file = "sas_plan"
        self.gpt3_plan_file = "gpt_sas_plan_{}"
        if self.engine == 'bloom':
            self.model = self.get_bloom()
        elif 'finetunedgpt3' in self.engine:
            print(self.engine)
            assert self.engine.split(':')[1] is not None
            model = ':'.join(self.engine.split(':')[1:])
            print(model)
            self.engine='finetunedgpt3'
            self.model = {'model':model}
        else:
            self.model = None
    
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
            self.data = yaml.safe_load(file)

    def get_problem(self, instance, domain):
        reader = PDDLReader(raise_on_error=True)
        reader.parse_domain(domain)
        return reader.parse_instance(instance)

    def get_executor(self, instance, domain, ground=True):
        plan_executor = Executor(domain, instance, ground=ground)
        return plan_executor

    def get_bloom(self):
        max_memory_mapping = {0: "0GB", 1: "43GB", 2: "43GB", 3: "43GB", 4: "43GB", 5: "43GB"}
        tokenizer = AutoTokenizer.from_pretrained("bigscience/bloom")
        model = AutoModelForCausalLM.from_pretrained("bigscience/bloom", cache_dir='/data/karthik/LLM_models/bloom/',
                                                     local_files_only=False, load_in_8bit=True, device_map='auto',
                                                     max_memory=max_memory_mapping)
        return {'model': model, 'tokenizer': tokenizer}


    def save_json(self, output_file, structured_output):
        os.makedirs(f"results_backprompting/{self.data['domain_name']}/{self.engine}/", exist_ok=True)
        with open(f"results_backprompting/{self.data['domain_name']}/{self.engine}/" + output_file + ".json", "w") as f:
            json.dump(structured_output, f, indent=4)
    def save_jsonl(self, output_file, entry):
        os.makedirs(f"results_backprompting/{self.data['domain_name']}/{self.engine}/", exist_ok=True)
        with open(f"results_backprompting/{self.data['domain_name']}/{self.engine}/" + output_file + ".jsonl", "a") as f:
            f.write(json.dumps(entry) + "\n")
    
    def load_json(self, output_file):
        if os.path.exists(f"results_backprompting/{self.data['domain_name']}/{self.engine}/" + output_file + ".json") and not self.ignore_existing:
            with open(f"results_backprompting/{self.data['domain_name']}/{self.engine}/" + output_file + ".json", "r") as f:
                return json.load(f)
        else:
            return None
    
    def per_instance_output(self, task_id, progress, lock,start, instance, domain_pddl, info_data, completed_instances, prev_success_results, failed_instances, use_llm_feedback, random_example, temperatures, val_feedback_type, iterations):
        query = self.data["domain_intro"]
        instance_structured_output = {}
        examples = []
        for i in range(start, start + self.n_examples+1):
            last_plan = True if i == start + self.n_examples else False
            get_plan = not last_plan
            if last_plan:
                cur_instance = instance.format(i)
            else:
                if random_example:
                    new_i = random.choice([ln for ln in range(1,self.n_files) if ln != i])
                    cur_instance = instance.format(new_i)
                    examples.append(new_i)
                else:
                    cur_instance = instance.format(i)
            try:
                cur_plan = info_data[f"instance-{i}.pddl"]["plan"].split('\n')
            except:
                raise ValueError(f"Plan not found for instance {i}")
            # --------------- Add to final output --------------- #
            # final_output += f"\n Instance {cur_instance}\n"
            if self.verbose:
                print(f"Instance {cur_instance}")
            problem = self.get_problem(cur_instance, domain_pddl)
            query += fill_template(*instance_to_text(problem, get_plan, self.data, plan=cur_plan), o4=last_plan)
            
            if get_plan:
                examples.append(i)
            else:
                # Store instance id for the actual instance LLM is asked to solve
                instance_structured_output["instance_id"] = i

        instance_structured_output["example_instance_ids"] = examples
        if instance_structured_output["instance_id"] in completed_instances:
            print(f"Instance {instance_structured_output['instance_id']} already completed")
            if not prev_success_results[instance_structured_output["instance_id"]]:
                failed_instances += 1
            return None, failed_instances
        messages, llm_plan, verifier_states_correct, act_correct, steps, context_window_hit, could_not_extract, feedback_messages, llm_plans = \
            self.get_repeated_verification_parallel(task_id, progress, lock,self.engine, query, domain_pddl, problem, cur_instance, use_llm_feedback, start, temperatures, val_feedback_type, iterations)
        
        instance_structured_output["messages"] = messages
        instance_structured_output["steps"] = steps
        instance_structured_output["verifier_states_correct"] = bool(verifier_states_correct)
        instance_structured_output["act_correct"] = bool(act_correct)
        instance_structured_output["step_wise_dict"] = llm_plans
        instance_structured_output["context_window_hit"] = bool(context_window_hit)
        instance_structured_output["could_not_extract"] = bool(could_not_extract)
        instance_structured_output["feedback_messages"] = feedback_messages
        # if not bool(act_correct):
        #     failed_instances += 1
        # print(f"Instance {instance_structured_output['instance_id']} completed")
        # print(instance_structured_output)
        return instance_structured_output, failed_instances

    def get_info_data(self, info_file, instance_folder, domain_pddl):
        info_data = {}
        for instance in tqdm(os.listdir(instance_folder), desc="Preprocessing plans"):
            info_data[instance] = self._compute_plan_optimal(domain_pddl, os.path.join(instance_folder, instance))
            with open(info_file, 'w') as f:
                json.dump(info_data, f, indent=4)
        return info_data
            


    def task_1_plan_generation_backprompting(self, config_file, use_llm_feedback, history, specified_instances=[], random_example=False, max_workers=1, temperatures={'generator': 0, 'verifier': 0}, val_feedback_type=1, iterations=15):
        
        
        self.read_config(config_file)
        task_name = "task_1_plan_generation_backprompting"
        used_verifier_and_critique = f"_{use_llm_feedback['verifier']}_verifier_{use_llm_feedback['critique']}_critique"
        zshot = "_zero_shot" if use_llm_feedback['zero_shot'] else ""
        val_form = "_val_form" if use_llm_feedback['val_form'] else ""
        no_hist = "_history" if history==-1 else f"_{history}_history"
        cot = "_cot" if use_llm_feedback['cot'] else ""
        add_feedback_type = f"_feedback_type_{val_feedback_type}" if use_llm_feedback['critique'] == 'sound' else ""
        temps = f"_tv{temperatures['verifier']}_tg{temperatures['generator']}"
        task_name = task_name + used_verifier_and_critique + zshot + val_form + no_hist + cot + add_feedback_type + temps
        print(task_name)

        instance_dir = self.data['instance_dir']
        domain_pddl = f'./instances/{self.data["domain_file"]}'
        instance_folder = f'./instances/{instance_dir}/'
        instance = f'./instances/{instance_dir}/{self.data["instances_template"]}'
        n_files = min(self.data['n_instances'], len(os.listdir(instance_folder)))

        i_start = self.data['start']
        i_end = self.data['end']
        n_files = i_end - i_start + 1  # min(self.data['n_instances'], len(os.listdir(instance_folder)))
        final_output = ""

        failed_instances = 0
        # print(task_name)
        structured_output = self.load_json(task_name)
        if structured_output is None:
            structured_output = {
                                "task": task_name,
                                "engine": self.engine,
                                "prompt_type": "oneshot",
                                "additional_task_info": f"backprompting_with_feedback_type_{val_feedback_type}",
                                "domain": self.data["domain_name"],
                                "tempertures": temperatures,
                                "instances": [],
                                }
        completed_instances =  []
        prev_success_results = {}
        for inst in structured_output["instances"]:
            if not inst["could_not_extract"]:
                completed_instances.append(inst["instance_id"])
                prev_success_results[inst["instance_id"]] = inst["act_correct"]
            else:
                if [msg["content"] for msg in inst["messages"] if msg["role"] == "assistant"][-1]:
                    completed_instances.append(inst["instance_id"])
                    prev_success_results[inst["instance_id"]] = inst["act_correct"]
        # print(completed_instances)
        if len(specified_instances):
            range_list = []
            for specified_instance in specified_instances:
                range_list.append(specified_instance - self.n_examples)
        else:
            range_list = range(i_start, i_end + 2 - self.n_examples)
        # print(completed_instances)

        #Preprocess plans
        info_file = f"./instances/{self.data['domain_name']}/info.json"
        if not os.path.exists(info_file):
            info_data = self.get_info_data(info_file, instance_folder, domain_pddl)
        else:
            with open(info_file, 'r') as f:
                info_data = json.load(f)
        lock = Lock()


        # for i in range_list:
        #     i_s_o, fi = self.per_instance_output(i, instance, domain_pddl, info_data, completed_instances, prev_success_results, failed_instances, use_llm_feedback, random_example, temperatures)
        
        with Progress(
            SpinnerColumn(spinner_name='earth'),
            TextColumn("[progress.description]{task.description}"),
            BarColumn(),
            TextColumn("[progress.percentage]{task.percentage:>3.1f}%"),
            TextColumn("[progress.percentage]{task.completed}/{task.total}")
        ) as progress:
            task_id = progress.add_task("[cyan]Processing...", total=len(range_list)*iterations)
        
            with concurrent.futures.ThreadPoolExecutor(max_workers=max_workers) as executor:
                results = [executor.submit(self.per_instance_output, task_id, progress, lock,i, instance, domain_pddl, info_data, completed_instances, prev_success_results, failed_instances, use_llm_feedback, random_example, temperatures, val_feedback_type, iterations) for i in range_list]

                for future in concurrent.futures.as_completed(results):
                    try:
                        instance_structured_output, failed_instances = future.result()
                    except Exception as e:
                        print(e)
                    if instance_structured_output is not None:
                        print(f"Instance {instance_structured_output['instance_id']} completed")
                        structured_output["instances"].append(instance_structured_output)
                        self.save_json(task_name, structured_output)
                        

        # --------------- Add to final output --------------- #

        
        # structured_output["failed_instances"] = failed_instances
        # self.save_json(task_name, structured_output)
        # final_output += f"[+]: The number of correct plans is {n_files - failed_instances}/{n_files}={(n_files - failed_instances) / (n_files) * 100}%"
        # print(final_output)

        return structured_output
    def per_instance_output_pddl(self,  task_name,task_id, progress, lock, start, instance, domain_pddl, info_data, completed_instances, prev_success_results, failed_instances, use_llm_feedback, random_example, temperatures, val_feedback_type, iterations):
        alphanum = ''.join(random.choices(string.ascii_lowercase + string.digits, k=6))
        with open(domain_pddl, 'r') as file:
            domain_intro = file.read()
        query = "The following is a PDDL specification of a planning problem. The first part, under the heading [DOMAIN], is the domain file. The second part, under the heading [QUERY PROBLEM], is the problem file. Using this information, which is correct, and no further assumptions, find a plan which, when run from the specified initial state, satisfies the specified goal. Provide your answer as a sequence of actions in PDDL format. An action ACTION which acts on two objects OBJ1 and OBJ2 would be written (ACTION OBJ1 OBJ2). Do not provide anything else in your answer.\n"
        query += "[DOMAIN]\n" + domain_intro.strip() + "\n\n"
        instance_structured_output = {}
        cur_instance = instance.format(start)
        # --------------- Read Instance --------------- #
        with open(cur_instance, 'r') as file:
            problem = file.read()
        problem = "(define" + problem.split("(define")[1][:-1].strip() + "\n)"
        
        query+="[QUERY PROBLEM]\n"+problem.strip()+"\n\n[PLAN]"
        # HACK TO FIX
        query = query.replace("obfuscated_randomized_blocksworld", alphanum)
        # try:
        #     cur_plan = info_data[f"instance-{start}.pddl"]["plan"].split('\n')
        # except:
        #     raise ValueError(f"Plan not found for instance {start}")
        # --------------- Add to final output --------------- #
        # final_output += f"\n Instance {cur_instance}\n"
        if self.verbose:
            print(f"Instance {cur_instance}")
        problem = self.get_problem(cur_instance, domain_pddl)
        try:
            if start in completed_instances:
                print(f"Instance {start} already completed")
                if not prev_success_results[start]:
                    failed_instances += 1
                return None, failed_instances
            else:
                existing_instance = None
                for inst in self.so["instances"]:
                    if inst["instance_id"] == start:
                        existing_instance = inst
                        break
                if existing_instance is not None:
                    instance_structured_output = existing_instance
        except Exception as e:
            print(f"[-] Error at checking existing instance: {e}")
                

        messages, llm_plan, verifier_states_correct, act_correct, steps, context_window_hit, could_not_extract, feedback_messages, llm_plans = \
            self.get_repeated_verification_pddl( task_name,instance_structured_output,task_id, progress, lock,self.engine, query, domain_pddl, problem, cur_instance, use_llm_feedback, start, temperatures, val_feedback_type, iterations,pddl=True)
        # if not existing_instance:
        instance_structured_output["instance_id"] = start
        instance_structured_output["messages"] = messages
        instance_structured_output["steps"] = steps
        instance_structured_output["verifier_states_correct"] = bool(verifier_states_correct)
        instance_structured_output["act_correct"] = bool(act_correct)
        instance_structured_output["step_wise_dict"] = llm_plans
        instance_structured_output["context_window_hit"] = bool(context_window_hit)
        instance_structured_output["could_not_extract"] = bool(could_not_extract)
        instance_structured_output["feedback_messages"] = feedback_messages
        # if not bool(act_correct):
        #     failed_instances += 1
        # print(instance_structured_output)
        return instance_structured_output, failed_instances 

    
    def get_repeated_verification_pddl(self, task_name,instance_structured_output,task_id, progress, lock,engine, original_query, domain_pddl, problem, cur_instance, use_llm_feedback, start, temperatures, feedback_type=1, iterations=15, pddl=False):
        correct = 0
        steps = 0
        query = original_query
        verifier_critique ={
            'verifier': use_llm_feedback['verifier'],
            'critique': use_llm_feedback['critique']
        }
        # print(original_query)
        messages = []
        step_dict = {}
        readable_plan = ""
        # print(f"Sending query to LLM: Instance {cur_instance}")
        if instance_structured_output:
            step_dict = instance_structured_output["step_wise_dict"]
            steps = instance_structured_output["steps"]
            messages = instance_structured_output["messages"]
            llm_plan = step_dict[str(steps-1)]['llm_plan']
            with lock:
                with open(self.gpt3_plan_file.format(start), 'w') as f:
                    f.write(llm_plan)
                    f.close()
            query, feedback_messages, correct, could_not_extract, step_dict = self.verification(domain_pddl, llm_plan, cur_instance, use_llm_feedback, temperatures, step_dict, steps-1, start, feedback_type=feedback_type, pddl=pddl)
            if could_not_extract:
                print("[-] Could not extract plan")
                return messages, llm_plan, correct, correct, steps, False, could_not_extract, feedback_messages, step_dict
            print(f">> Partially completed instance instance-{start}.pddl")
            
        while steps < iterations:  
            # print(messages)
            steps = str(steps)
            step_dict[steps] = {}
            progress.update(task_id, advance=1)
            llm_response, messages, context_window_hit, rate_limit_hit, response_object, time_taken, sending_messages = send_query_with_feedback(query, engine, messages, use_llm_feedback['history'], temperatures['generator'])

            step_dict[steps]['time_taken'] = time_taken 
            step_dict[steps]['sent_messages'] = sending_messages
            if context_window_hit:
                break
            if rate_limit_hit:
                print("Rate limit hit. Waiting for 60 seconds.")
                time.sleep(60)
                continue

            if isinstance(response_object, dict):
                step_dict[steps]['response_object'] = response_object
            else:
                step_dict[steps]['response_object'] = json.loads(response_object.model_dump_json())
            try:
                if not pddl:
                    with lock:
                        llm_plan, readable_plan = text_to_plan(llm_response, problem.actions, self.gpt3_plan_file.format(start), self.data)
                else:
                    with lock:
                        llm_plan = save_gpt3_response(llm_response, self.gpt3_plan_file.format(start))
                        readable_plan = llm_plan
                step_dict[steps]['llm_plan'] = llm_plan
            except:
                could_not_extract = True 
                break
            query, feedback_messages, correct, could_not_extract, step_dict = self.verification(domain_pddl, llm_plan, cur_instance, use_llm_feedback, temperatures, step_dict, steps, start, feedback_type=feedback_type, pddl=pddl)
            if could_not_extract:
                break
            
                
            if not use_llm_feedback['ignore_verifier']:
                if correct:
                    break
            steps = int(steps)+1
        # Determine whether backprompting result is actually correct
        # Since the LLM is not sound, this may not be true when the LLM says it is correct
        # This is always true when VAL says it is correct
        if verifier_critique['verifier'] == 'llm':
            try: 
                if not pddl:
                    with lock:
                        _, readable_plan = text_to_plan(llm_response, problem.actions, self.gpt3_plan_file.format(start), self.data)
                    feedback_dict = get_val_feedback(domain_pddl, cur_instance, self.gpt3_plan_file.format(start))
                else:
                    with lock:
                        llm_plan = llm_response.strip('```').strip()
                        with open(self.gpt3_plan_file.format(start), 'w') as f:
                            f.write(llm_plan)
                            f.close()
                    readable_plan = llm_plan
                    feedback_dict = get_val_feedback(domain_pddl, cur_instance, self.gpt3_plan_file.format(start))
            except:
                print("WARNING: final plan could not be verified")
            act_correct = int(feedback_dict["validation_info"]["is_valid_plan"])
        else:
            act_correct = correct

        # print(f"Final LLM response after {steps} steps")
        return messages, readable_plan, correct, act_correct, steps, context_window_hit, could_not_extract, feedback_messages, step_dict
    
    def verification(self, domain_pddl, llm_plan, cur_instance, use_llm_feedback, temperatures, step_dict, steps, start, feedback_type=1, pddl=False):
        feedback_messages = []
        could_not_extract = False
        query=""
        steps = str(steps)
        if verifier_critique['verifier'] == 'llm' or verifier_critique['critique'] == 'llm':
            query, feedback_messages = self.get_llm_feedback(domain_pddl, llm_plan.strip().split("\n"), cur_instance, use_llm_feedback, temperatures)
            step_dict[steps]['llm_feedback'] = query
            llm_verifier_json = get_llm_verifier_json(query, use_llm_feedback['val_form'])
            if llm_verifier_json == None:
                print("ERROR: Could not extract llm verifier json: ", query)
                could_not_extract = True 
                return query, feedback_messages, 0, could_not_extract, step_dict
                # correct = check_correctness(query, use_llm_feedback['val_form'])
                # query+=f"\nWith this feedback, please try again to generate a valid plan."
            else:
                try:
                    llm_verification, llm_critique = get_validation_message_for_llm(llm_verifier_json)
                except Exception as e:
                    print(f"[-] Error: {e}")
                    print(llm_verifier_json)
                    could_not_extract = True
                    return query, feedback_messages, 0, could_not_extract, step_dict
                # if verifier_critique['verifier'] == 'sound' or verifier_critique['critique'] == 'sound':
                try:
                    sound_verifier_json = get_val_feedback(domain_pddl, cur_instance, self.gpt3_plan_file.format(start)) if feedback_type != 2 else get_all_errors(domain_pddl, cur_instance, self.gpt3_plan_file.format(start))

                    # print("Sound verifier json", sound_verifier_json)
                except:
                    print("ERROR: Could not extract plan")
                    print(sound_verifier_json)
                    could_not_extract = True 
                    return query, feedback_messages, 0, could_not_extract, step_dict
                
                if not sound_verifier_json['validation_info']['is_valid_plan']:
                    try:
                        sound_verification, sound_critique = get_validation_message(sound_verifier_json, self.data, feedback_type=feedback_type, val_validator=feedback_type!=2, pddl=pddl)
                        if sound_critique is None:
                            sound_critique = ""
                    except:
                        print("ERROR: Sound Verifier Dict Extraction error")
                        print(sound_verifier_json)
                        could_not_extract = True
                        return query, feedback_messages, 0, could_not_extract
                else:
                    sound_verification, sound_critique = "The plan is valid.", ""
                try:
                    if verifier_critique['verifier']=='llm':
                        query = llm_verification
                    elif verifier_critique['verifier']=='sound':
                        query = sound_verification
                    else:
                        query = ''
                    if verifier_critique['critique']=='llm':
                        query += " "+llm_critique
                    elif verifier_critique['critique']=='sound':
                        query += " "+sound_critique
                    else:
                        query += ''
                except Exception as e:
                    print(f"[-] Error: {e}")
                    print(llm_verification, llm_critique, sound_verification, sound_critique, sound_verifier_json)
                step_dict[steps]['llm_verifier_json'] = llm_verifier_json
                step_dict[steps]['sound_verifier_json'] = sound_verifier_json
                #Calculate true positives and false positives between LLM and sound verifier with jsons

                
                
                
            correct = llm_verifier_json["valid"] if verifier_critique['verifier'] == 'llm' else sound_verifier_json["validation_info"]["is_valid_plan"]
                    
        else:
            val_validator = feedback_type != 2
            try: 
                
                sound_verifier_json = get_val_feedback(domain_pddl, cur_instance, self.gpt3_plan_file.format(start)) if val_validator else get_all_errors(domain_pddl, cur_instance, self.gpt3_plan_file.format(start))
                step_dict[steps]['llm_plan'] = llm_plan
                step_dict[steps]['sound_verifier_json'] = sound_verifier_json
            except Exception as e:
                print(f"[-] Error: could not extract plan: {e}")
                print(sound_verifier_json)
                could_not_extract = True 
                return query, feedback_messages, 0, could_not_extract, step_dict
            correct = int(sound_verifier_json["validation_info"]["is_valid_plan"])    
            if not correct:
                try:
                    sound_verification, sound_critique = get_validation_message(sound_verifier_json, 
                                                self.data,
                                                feedback_type=feedback_type, val_validator=val_validator, pddl=pddl)
                    query = sound_verification
                    try:
                        query = query.strip()+" " + sound_critique
                    except Exception as e:
                        print(f"ERROR: Validation dict missing error: {e}")
                        print(sound_verifier_json)
                        print(llm_plan.split("\n"))
                        query += " " +sound_verifier_json['validation_message']
                except Exception as e:
                    print(f"[-] Error: could not extract: {e}")
                    could_not_extract = True
                    return query, feedback_messages, 0, could_not_extract, step_dict
        if not pddl:
            query = query.strip() + f" Please provide the plan between [PLAN] and [PLAN END] tags."
        else: 
            query = query.strip() + f"\nProvide your revised plan as a sequence of actions in PDDL format. An action ACTION which acts on two objects OBJ1 and OBJ2 would be written (ACTION OBJ1 OBJ2). Do not provide anything else in your answer.\n\n[PLAN]"
        
        return query, feedback_messages, correct, could_not_extract, step_dict
    
    """
    Step 1: Get current instance_structured_output latest step from jsonl file
    S
    """
    def task_1_plan_generation_backprompting_zero_shot_pddl(self, config_file, use_llm_feedback, history, specified_instances=[], random_example=False, max_workers=1, temperatures={'generator': 0, 'verifier': 0}, val_feedback_type=1, iterations=15):
        self.read_config(config_file)
        task_name = "task_1_plan_generation_backprompting_zero_shot_pddl"
        used_verifier_and_critique = f"_{use_llm_feedback['verifier']}_verifier_{use_llm_feedback['critique']}_critique"
        zshot = "_zero_shot" if use_llm_feedback['zero_shot'] else ""
        val_form = "_val_form" if use_llm_feedback['val_form'] else ""
        no_hist = "_history" if history==-1 else f"_{history}_history"
        cot = "_cot" if use_llm_feedback['cot'] else ""
        add_feedback_type = f"_feedback_type_{val_feedback_type}" if all([i=="sound" for i in [use_llm_feedback['verifier'], use_llm_feedback['critique']]]) else ""
        temps = f"_tv{temperatures['verifier']}_tg{temperatures['generator']}"
        task_name = task_name + used_verifier_and_critique + zshot + val_form + no_hist + cot + add_feedback_type + temps
        print(task_name)

        instance_dir = self.data['instance_dir']
        domain_pddl = f'./instances/{self.data["domain_file"]}'
        instance_folder = f'./instances/{instance_dir}/'
        instance = f'./instances/{instance_dir}/{self.data["instances_template"]}'
        n_files = min(self.data['n_instances'], len(os.listdir(instance_folder)))

        i_start = self.data['start']
        i_end = self.data['end']
        n_files = i_end - i_start + 1  # min(self.data['n_instances'], len(os.listdir(instance_folder)))
        final_output = ""

        failed_instances = 0
        # print(task_name)
        structured_output = self.load_json(task_name)
        if structured_output is None:
            structured_output = {
                                "task": task_name,
                                "engine": self.engine,
                                "prompt_type": "oneshot",
                                "additional_task_info": f"backprompting_with_feedback_type_{val_feedback_type}",
                                "domain": self.data["domain_name"],
                                "tempertures": temperatures,
                                "instances": [],
                                }
        completed_instances =  []
        prev_success_results = {}
        self.so = structured_output
        for inst in structured_output["instances"]:
            if not inst["could_not_extract"]:
                if "act_correct" in inst:
                    if inst["act_correct"] or inst["steps"]>=iterations:
                        completed_instances.append(inst["instance_id"])
                prev_success_results[inst["instance_id"]] = inst["act_correct"]
            else:
                if [msg["content"] for msg in inst["messages"] if msg["role"] == "assistant"][-1]:
                    completed_instances.append(inst["instance_id"])
                    prev_success_results[inst["instance_id"]] = inst["act_correct"]
        # print(completed_instances)
        if len(specified_instances):
            range_list = []
            for specified_instance in specified_instances:
                range_list.append(specified_instance - self.n_examples)
        else:
            range_list = range(i_start, i_end + 2 - self.n_examples)
        # print(completed_instances)

        #Preprocess plans
        # info_file = f"./instances/{self.data['domain_name']}/info.json"
        # if not os.path.exists(info_file):
        #     info_data = self.get_info_data(info_file, instance_folder, domain_pddl)
        # else:
        #     with open(info_file, 'r') as f:
        #         info_data = json.load(f)
        info_data = {}
        lock = Lock()
        # for i in range_list:
        #     i_s_o, fi = self.per_instance_output(i, instance, domain_pddl, info_data, completed_instances, prev_success_results, failed_instances, use_llm_feedback, random_example, temperatures)
        with Progress(
            SpinnerColumn(spinner_name='earth'),
            TextColumn("[progress.description]{task.description}"),
            BarColumn(),
            TextColumn("[progress.percentage]{task.percentage:>3.1f}%"),
            TextColumn("[progress.percentage]{task.completed}/{task.total}")
        ) as progress:
            task_id = progress.add_task("[cyan]Processing...", total=len(range_list)*iterations)
            print(f"\n\n\n>>>> Deploying {max_workers} Masters students to solve {len(range_list)} instances\n\n\n")
            with concurrent.futures.ThreadPoolExecutor(max_workers=max_workers) as executor:
                results = [executor.submit(self.per_instance_output_pddl, task_name, task_id, progress, lock,i, instance, domain_pddl, info_data, completed_instances, prev_success_results, failed_instances, use_llm_feedback, random_example, temperatures, val_feedback_type, iterations) for i in range_list]

                for future in concurrent.futures.as_completed(results):
                    try:
                        instance_structured_output, failed_instances = future.result()
                    except Exception as e:
                        print(f"[-] Error: {e}")
                    if instance_structured_output is not None:
                        print(f"Instance {instance_structured_output['instance_id']} completed")
                        # Check if instance already exists in structured output
                        if not any([inst["instance_id"]==instance_structured_output["instance_id"] for inst in structured_output["instances"]]):
                            structured_output["instances"].append(instance_structured_output)
                            with lock:
                                self.save_json(task_name, structured_output)
                        else:
                            #Replace existing instance with new instance
                            with lock:
                                for i, inst in enumerate(structured_output["instances"]):
                                    if inst["instance_id"]==instance_structured_output["instance_id"]:
                                        structured_output["instances"][i] = instance_structured_output
                                        break
                                
                                self.save_json(task_name, structured_output)
        # --------------- Add to final output --------------- #
        # structured_output["failed_instances"] = failed_instances
        # self.save_json(task_name, structured_output)
        # final_output += f"[+]: The number of correct plans is {n_files - failed_instances}/{n_files}={(n_files - failed_instances) / (n_files) * 100}%"
        # print(final_output)

        return structured_output

    
    def get_llm_feedback(self, domain_pddl, llm_plan, cur_instance, use_llm_feedback, temperatures, messages=[]):
        '''
        Has an LLM correct the plan. Previous messages with the LLM (generation messages)
        can optionally be passed as context to the LLM if there's interest
        in seeing this as a conversation.
        '''
        query = self.data["domain_intro"]
        domain_file = self.data['domain_file']
        instance_dir = self.data['instance_dir']
        instance_format = f'./instances/{instance_dir}/{self.data["instances_template"]}'
        info_file = f"./instances/{self.data['domain_name']}/info.json"
        with open(info_file, 'r') as f:
            info_data = json.load(f)
        instance_folder = f'./instances/{instance_dir}/'
        n_files = min(self.data['n_instances'], len(os.listdir(instance_folder)))
        instance_structured_output = {}
        instance_id = int(cur_instance.split('/')[-1].split('.')[0].split('-')[-1])
        instance_structured_output["instance_id"] = instance_id
        problem = self.get_problem(cur_instance, domain_pddl)
        
        if not use_llm_feedback['zero_shot']:
            # example_instances = random.choices([ln for ln in range(1, n_files) if ln != instance_id], k=3)        
            # example_type = [-1, 0, 1]
            # random.shuffle(example_type)
            # for example, example_type in zip(example_instances, example_type):
            #     example_instance = instance_format.format(example)
            #     plan_executor = self.get_executor(example_instance, domain_pddl)
            #     text,_ = plan_verification(plan_executor, self.data, True, give_response = True, example_type=example_type)
            #     query += text
            # plan_executor = self.get_executor(cur_instance, domain_pddl)
            # text, _ = plan_verification(plan_executor, self.data, False, llm_plan=llm_plan)
            raise NotImplementedError("Few shot not implemented")
        else:
            # plan_executor = self.get_executor(cur_instance, domain_pddl)
            # llm_plan = [i.replace('_', ' ') for i in llm_plan]
            INIT, GOAL, PLAN, _ = instance_to_text(problem, True, self.data, plan=llm_plan)
            
            if not use_llm_feedback['val_form']:
                text = plan_verification_zero_shot(INIT, PLAN, GOAL,cot=use_llm_feedback['cot'])
            else:
                text = plan_verification_zero_shot_val_form(INIT, PLAN, GOAL,cot=use_llm_feedback['cot'])
        query += text

        response, messages, _, _,_,_,_ = send_query_with_feedback(query, self.engine, messages, temp=temperatures['verifier'])

        return response, messages

    def get_repeated_verification_parallel(self, task_id, progress, lock,engine, original_query, domain_pddl, problem, cur_instance, use_llm_feedback, start, temperatures, feedback_type=1, iterations=15, pddl=False):
        correct = 0
        steps = 0
        query = original_query
        verifier_critique ={
            'verifier': use_llm_feedback['verifier'],
            'critique': use_llm_feedback['critique']
        }
        # print(original_query)
        messages = []
        step_dict = {}
        could_not_extract = False
        # print(f"Sending query to LLM: Instance {cur_instance}")
        
        while steps < iterations:  
            # print(messages)
            step_dict[steps] = {}
            with lock:
                progress.update(task_id, advance=1)
            llm_response, messages, context_window_hit, rate_limit_hit, response_object, time_taken, sending_messages = send_query_with_feedback(query, engine, messages, use_llm_feedback['history'], temperatures['generator'])

            step_dict[steps]['time_taken'] = time_taken
            step_dict[steps]['messages'] = sending_messages
#             llm_response = """
# unstack the red block from on top of the blue block
# put down the red block
# unstack the blue block from on top of the yellow block
# put down the blue block
# unstack the yellow block from on top of the orange block
# pick up the red block
# stack the red block on top of the yellow block
# [PLAN END]
#             """
#             context_window_hit, rate_limit_hit = False, False
            if context_window_hit:
                break
            if rate_limit_hit:
                print("Rate limit hit. Waiting for 60 seconds.")
                time.sleep(60)
                continue

            feedback_messages = []
            if isinstance(response_object, dict):
                step_dict[steps]['response_object'] = response_object
            elif response_object is None:
                step_dict[steps]['response_object'] = None
            else:
                try:
                    step_dict[steps]['response_object'] = json.loads(response_object.model_dump_json())
                except:
                    step_dict[steps]['response_object'] = None
            if verifier_critique['verifier'] == 'llm' or verifier_critique['critique'] == 'llm':
                try:
                    if not pddl:
                        with lock:
                            llm_plan, readable_plan = text_to_plan(llm_response, problem.actions, self.gpt3_plan_file.format(start), self.data)
                    else:
                        with lock:
                            llm_plan = llm_response.strip('```').strip()
                            with open(self.gpt3_plan_file.format(start), 'w') as f:
                                f.write(llm_plan)
                                f.close()
                            readable_plan = llm_plan
                    step_dict[steps]['llm_plan'] = llm_plan
                except:
                    could_not_extract = True 
                    break
                query, feedback_messages = self.get_llm_feedback(domain_pddl, llm_plan.strip().split("\n"), cur_instance, use_llm_feedback, temperatures)
                step_dict[steps]['llm_feedback'] = query
                llm_verifier_json = get_llm_verifier_json(query, use_llm_feedback['val_form'])
                if llm_verifier_json == None:
                    print("ERROR: Could not extract llm verifier json: ", query)
                    could_not_extract = True 
                    break
                    # correct = check_correctness(query, use_llm_feedback['val_form'])
                    # query+=f"\nWith this feedback, please try again to generate a valid plan."
                else:
                    try:
                        llm_verification, llm_critique = get_validation_message_for_llm(llm_verifier_json)
                    except Exception as e:
                        print(f"[-] Error: {e}")
                        print(llm_verifier_json)
                        could_not_extract = True
                        break
                    # if verifier_critique['verifier'] == 'sound' or verifier_critique['critique'] == 'sound':
                    try:
                        sound_verifier_json = get_val_feedback(domain_pddl, cur_instance, self.gpt3_plan_file.format(start)) if feedback_type != 2 else get_all_errors(domain_pddl, cur_instance, self.gpt3_plan_file.format(start))

                        # print("Sound verifier json", sound_verifier_json)
                    except:
                        print("ERROR: Could not extract plan")
                        print(sound_verifier_json)
                        could_not_extract = True 
                        break
                    
                    if not sound_verifier_json['validation_info']['is_valid_plan']:
                        try:
                            sound_verification, sound_critique = get_validation_message(sound_verifier_json, self.data, feedback_type=feedback_type, val_validator=feedback_type!=2, pddl=pddl)
                            if sound_critique is None:
                                sound_critique = ""
                        except:
                            print("ERROR: Sound Verifier Dict Extraction error")
                            print(sound_verifier_json)
                            could_not_extract = True
                            break
                    else:
                        sound_verification, sound_critique = "The plan is valid.", ""
                    try:
                        if verifier_critique['verifier']=='llm':
                            query = llm_verification
                        elif verifier_critique['verifier']=='sound':
                            query = sound_verification
                        else:
                            query = ''
                        if verifier_critique['critique']=='llm':
                            query += " "+llm_critique
                        elif verifier_critique['critique']=='sound':
                            query += " "+sound_critique
                        else:
                            query += ''
                    except Exception as e:
                        print(f"[-] Error: {e}")
                        print(llm_verification, llm_critique, sound_verification, sound_critique, sound_verifier_json)
                    step_dict[steps]['llm_verifier_json'] = llm_verifier_json
                    step_dict[steps]['sound_verifier_json'] = sound_verifier_json
                    #Calculate true positives and false positives between LLM and sound verifier with jsons

                    
                    
                    
                correct = llm_verifier_json["valid"] if verifier_critique['verifier'] == 'llm' else sound_verifier_json["validation_info"]["is_valid_plan"]
                        
            else:
                val_validator = feedback_type != 2
                try: 
                    if not pddl:
                        with lock:
                            llm_plan, readable_plan = text_to_plan(llm_response, problem.actions, self.gpt3_plan_file.format(start), self.data)
                    else:
                        with lock:
                            llm_plan = llm_response.strip('```').strip()
                            with open(self.gpt3_plan_file.format(start), 'w') as f:
                                f.write(llm_plan)
                                f.close()
                        readable_plan = llm_plan
                    sound_verifier_json = get_val_feedback(domain_pddl, cur_instance, self.gpt3_plan_file.format(start)) if val_validator else get_all_errors(domain_pddl, cur_instance, self.gpt3_plan_file.format(start))
                    step_dict[steps]['llm_plan'] = llm_plan
                    step_dict[steps]['sound_verifier_json'] = sound_verifier_json
                except:
                    print("ERROR: Could not extract plan")
                    print(sound_verifier_json)
                    could_not_extract = True 
                    break
                correct = int(sound_verifier_json["validation_info"]["is_valid_plan"])    
                if not correct:
                    try:
                        sound_verification, sound_critique = get_validation_message(sound_verifier_json, 
                                                    self.data,
                                                    feedback_type=feedback_type, val_validator=val_validator, pddl=pddl)
                        query = sound_verification + " " + sound_critique
                    except Exception as e:
                        print(f"ERROR: Validation dict missing error: {e}")
                        print(sound_verifier_json)
                        could_not_extract = True
                        break
            if not pddl:
                query = query.strip() + f" Please provide the plan between [PLAN] and [PLAN END] tags."
                
            if not use_llm_feedback['ignore_verifier']:
                if correct:
                    break
            steps += 1
        # Determine whether backprompting result is actually correct
        # Since the LLM is not sound, this may not be true when the LLM says it is correct
        # This is always true when VAL says it is correct
        if verifier_critique['verifier'] == 'llm':
            try: 
                if not pddl:
                    with lock:
                        _, readable_plan = text_to_plan(llm_response, problem.actions, self.gpt3_plan_file.format(start), self.data)
                    feedback_dict = get_val_feedback(domain_pddl, cur_instance, self.gpt3_plan_file.format(start))
                else:
                    with lock:
                        llm_plan = llm_response.strip('```').strip()
                        with open(self.gpt3_plan_file.format(start), 'w') as f:
                            f.write(llm_plan)
                            f.close()
                    readable_plan = llm_plan
                    feedback_dict = get_val_feedback(domain_pddl, cur_instance, self.gpt3_plan_file.format(start))
            except:
                print("WARNING: final plan could not be verified")
            act_correct = int(feedback_dict["validation_info"]["is_valid_plan"])
        else:
            act_correct = correct

        # print(f"Final LLM response after {steps} steps")
        return messages, readable_plan, correct, act_correct, steps, context_window_hit, could_not_extract, feedback_messages, step_dict

if __name__=="__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--verbose', type=str, default="False", help='Verbose')
    #config
    parser.add_argument('--config', type=str, required=True, help='Config file name (no need to add .yaml)')
    parser.add_argument('--engine', type=str, required=True, help='Engine to use \
                        \n gpt-4_chat = GPT-4 \
                        \n bloom = Bloom \
                        \n gpt-3.5-turbo_chat = GPT-3.5 Turbo \
                        \n davinci = GPT-3 Davinci \
                        \n curie = GPT-3 Curie \
                        \n babbage = GPT-3 Babbage \
                        \n ada = GPT-3 Ada \
                        ')
    parser.add_argument('--specific_instances', nargs='+', type=int, default=[], help='List of instances to run')
    parser.add_argument('--random_example', type=str, default="False", help='Random example')
    parser.add_argument('--ignore_existing', action='store_true', help='Ignore existing output')
    parser.add_argument('--seed', type=int, default=42, help='Random seed')


    parser.add_argument('--llm_verifier', action='store_true', help='Use LLM for verifier')
    parser.add_argument('--sound_verifier', action='store_true', help='Use sound verifier')
    parser.add_argument('--llm_critique', action='store_true', help='Use LLM for critique')
    parser.add_argument('--sound_critique', action='store_true', help='Use sound critique')


    parser.add_argument('--history', type=int ,default=-1, help='History')
    parser.add_argument('--cot', action='store_true', help='COT')
    # parser.add_argument('--val_form', action='store_true', help='val_form')

    parser.add_argument('--max_workers', type=int, default=1, help='Max workers')
    
    parser.add_argument('--t_verifier', type=float, default=0.0, help='Verifier temperature')
    parser.add_argument('--t_generator', type=float, default=0.0, help='Generator temperature')
    
    parser.add_argument('--feedback_type', type=int, default=1, help='Changes the amount of feedback provided to LLM in backprompt.\n \
                        \n 0 = no error \
                        \n 1 = one errors \
                        \n 2 = all errors \
                        ')
    parser.add_argument('--ignore_verifier', action='store_true', help='Ignore verifier')
    parser.add_argument('--iterations', type=int, default=15, help='Number of iterations')
    parser.add_argument('--pddl', action='store_true', help='PDDL')
    args = parser.parse_args()
    config = args.config
    engine = args.engine
    verbose = eval(args.verbose)
    specified_instances = args.specific_instances
    random_example = eval(args.random_example)
    ignore_existing = args.ignore_existing
    seed = args.seed
    cot = args.cot
    history = args.history
    max_workers = args.max_workers
    verifier_critique ={
        'verifier': None,
        'critique': None
    }
    if args.llm_verifier:
        verifier_critique['verifier'] = 'llm'
    elif args.sound_verifier:
        verifier_critique['verifier'] = 'sound'
    if args.llm_critique:
        verifier_critique['critique'] = 'llm'
    elif args.sound_critique:
        verifier_critique['critique'] = 'sound'
    temperatures = {'verifier': args.t_verifier, 'generator': args.t_generator}
    val_feedback_type = args.feedback_type
    iterations = args.iterations
    # if not args.llm_validation and history:
    #     raise ValueError("No history can only be used with LLM validation")
    random.seed(seed)
    # print(task, config, verbose, specified_instances, random_example)
    use_llm_feedback = {
        'verifier': verifier_critique['verifier'],
        'critique': verifier_critique['critique'],
        'zero_shot': True,
        'val_form': True,
        'history': history,
        'cot': cot,
        'ignore_verifier': args.ignore_verifier,
        
    }
    
    config_file = f'./configs/{config}.yaml'
    backprompter = BackPrompter(engine, verbose=verbose, ignore_existing=ignore_existing)
    if not args.pddl:
        backprompter.task_1_plan_generation_backprompting(config_file, use_llm_feedback, history, specified_instances=specified_instances, random_example=random_example, max_workers=max_workers, temperatures=temperatures, val_feedback_type=val_feedback_type, iterations=iterations)
    else:
        backprompter.task_1_plan_generation_backprompting_zero_shot_pddl(config_file, use_llm_feedback, history, specified_instances=specified_instances, random_example=random_example, max_workers=max_workers, temperatures=temperatures, val_feedback_type=val_feedback_type, iterations=iterations)
    # Get current directory
    for i in os.listdir(os.getcwd()):
        if 'gpt_sas_plan' in i:
            os.remove(i)
    



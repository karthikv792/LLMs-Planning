import os
import random
import openai
import numpy as np
import hashlib
from tarski.io import PDDLReader
from tarski.syntax.formulas import *
from transformers import StoppingCriteriaList, StoppingCriteria

openai.api_key = os.environ["OPENAI_API_KEY"]
random.seed(10)


class Callbacks():
    def __init__(self, data):
        self.data = data
        self.instances_template = f"./instances/{data['domain']}/{data['instances_template']}"

        self.hashset = set()
        os.makedirs(f"./instances/{data['domain']}/", exist_ok=True)

    def instance_ok(self, domain, instance):
        reader = PDDLReader(raise_on_error=True)
        reader.parse_domain(domain)
        reader.parse_instance(instance)
        if isinstance(reader.problem.goal, Tautology):
            return False
        elif isinstance(reader.problem.goal, Atom):
            if reader.problem.goal in reader.problem.init.as_atoms():
                return False
        else:
            if (all([i in reader.problem.init.as_atoms() for i in reader.problem.goal.subformulas])):
                return False
        return True

    def add_existing_files_to_hash_set(self):
        for i in os.listdir(f"./instances/{self.data['domain']}/"):
            f = open(f"./instances/{self.data['domain']}/" + i, "r")
            pddl = f.read()
            self.hashset.add(hashlib.md5(pddl.encode('utf-8')).hexdigest())
        return len(self.hashset)

    def t1_gen_goal_directed_instances(self):
        n = self.data['n_instances'] + 2
        n_objs = range(4, len(self.data["encoded_objects"]) + 1)
        ORIG = os.getcwd()
        CMD = "./blocksworld 4 {}"
        start = self.add_existing_files_to_hash_set()

        os.chdir("pddlgenerators/blocksworld/")
        instance_file = f"{ORIG}/{self.instances_template}"
        domain = f"{ORIG}/instances/{self.data['domain_file']}"
        c = start
        for obj in n_objs:
            cmd_exec = CMD.format(obj)
            for i in range(1, n):
                with open(instance_file.format(c), "w+") as fd:
                    pddl = os.popen(cmd_exec).read()
                    hash_of_instance = hashlib.md5(pddl.encode('utf-8')).hexdigest()
                    if hash_of_instance in self.hashset:
                        print("[+]: Same instance, skipping...")
                        continue
                    self.hashset.add(hash_of_instance)
                    fd.write(pddl)

                inst_to_parse = instance_file.format(c)
                if self.instance_ok(domain, inst_to_parse):
                    c += 1
                else:
                    print("[-]: Instance not valid")
                    self.hashset.remove(hash_of_instance)
                    os.remove(inst_to_parse)
                    continue
                if c == n:
                    break
            if c == n:
                break

        print(f"[+]: A total of {c} instances have been generated")
        os.chdir(ORIG)

    def t4_gen_generalization_instances(self):
        def gen_instance(objs):
            text = "(define (problem BW-generalization-4)\n(:domain blocksworld-4ops)"
            text += "(:objects " + " ".join(objs) + ")\n"
            text += "(:init \n(handempty)\n"

            for obj in objs:
                text += f"(ontable {obj})\n"

            for obj in objs:
                text += f"(clear {obj})\n"

            text += ")\n(:goal\n(and\n"

            obj_tuples = list(zip(objs, objs[1:]))
            # obj_tuples.reverse() # TODO: this improves considerably Davinci t4

            for i in obj_tuples:
                text += f"(on {i[0]} {i[1]})\n"

            text += ")))"
            return text

        n = self.data['n_instances'] + 2
        objs = self.data['encoded_objects']
        encoded_objs = list(objs.keys())
        start = self.add_existing_files_to_hash_set()

        print("[+]: Making generalization instances for blocksworld")
        for c in range(start, n):
            n_objs = random.randint(3, len(objs))
            random.shuffle(encoded_objs)
            objs_instance = encoded_objs[:n_objs]
            instance = gen_instance(objs_instance)

            if hashlib.md5(instance.encode('utf-8')).hexdigest() in self.hashset:
                print("INSTANCE ALREADY IN SET, SKIPPING")
                continue

            with open(self.instances_template.format(c), "w+") as fd:
                fd.write(instance)

def generate_from_bloom(model, tokenizer, query, max_tokens):
    encoded_input = tokenizer(query, return_tensors='pt')
    stop = tokenizer("[PLAN END]", return_tensors='pt')
    stoplist = StoppingCriteriaList([stop])
    output_sequences = model.generate(input_ids=encoded_input['input_ids'].cuda(), max_new_tokens=max_tokens, temperature=0,top_p=1)
    return tokenizer.decode(output_sequences[0], skip_special_tokes=True)

def send_query(query, engine, max_tokens, model=None, stop="[STATEMENT]"):
    max_token_err_flag = False
    if engine=='bloom':

        if model:
            response = generate_from_bloom(model['model'], model['tokenizer'], query, max_tokens)
            response = response.replace(query, '')
            resp_string = ""
            for line in response.split('\n'):
                if '[PLAN END]' in line:
                    break
                else:
                    resp_string+=f'{line}\n'
            return resp_string
        else:
            assert model is not None
    else:
        try:
            response = openai.Completion.create(
                model=engine,
                prompt=query,
                temperature=0,
                max_tokens=max_tokens,
                top_p=1,
                frequency_penalty=0,
                presence_penalty=0,
                stop=stop)
        except Exception as e:
            max_token_err_flag = True
            print("[-]: Failed GPT3 query execution: {}".format(e))

        text_response = response["choices"][0]["text"] if not max_token_err_flag else ""
        return text_response.strip()


def treat_on(letters_dict, atom):
    terms = atom.subterms
    return f"the {letters_dict[terms[0].name]} block on top of the {letters_dict[terms[1].name]} block"

def get_sorted(init_atoms):
    return sorted(init_atoms, key=lambda x: x.symbol.name+" "+" ".join([subterm.name for subterm in x.subterms]))

def parse_problem(problem, data, shuffle):
    def parse(init_goal_preds, OBJS):
        TEXT = ""
        predicates = []

        init_goal_preds = list(init_goal_preds)
        for atom in init_goal_preds:
            objs = []
            for subterm in atom.subterms:
                objs.append(OBJS[subterm.name])
            predicates.append(data['predicates'][atom.symbol.name].format(*objs))
        if len(predicates) > 1:
            TEXT += ", ".join(predicates[:-1]) + f" and {predicates[-1]}"
        else:
            TEXT += predicates[0]
        return TEXT




    OBJS = data['encoded_objects']

    init_atoms = get_sorted(problem.init.as_atoms())
    goal_preds = get_sorted(problem.goal.subformulas) if hasattr(problem.goal, 'subformulas') else [problem.goal]

    if shuffle:
        random.shuffle(init_atoms)
        random.shuffle(goal_preds)
    # print(shuffle,init_atoms)
    # print(goal_preds)
    # ----------- INIT STATE TO TEXT ----------- #
    INIT = parse(init_atoms, OBJS)

    # ----------- GOAL TO TEXT ----------- #
    GOAL = parse(goal_preds, OBJS)

    return INIT, GOAL


def get_ordered_objects(object_names, line):
    objs = []
    pos = []
    for obj in object_names:
        if obj in line:
            objs.append(obj)
            pos.append(line.index(obj))

    sorted_zipped_lists = sorted(zip(pos, objs))
    return [el for _, el in sorted_zipped_lists]


def validate_plan(domain, instance, plan_file):
    val_path = os.getenv("VAL")
    cmd = f"{val_path}/validate {domain} {instance} {plan_file}"
    response = os.popen(cmd).read()
    if 'Problem in domain' in response:
        raise Exception('Problem in domain: Check PDDL Writer')
    return True if "Plan valid" in response else False


def fill_template(INIT, GOAL, PLAN):
    text = ""
    if INIT != "":
        text += "\n[STATEMENT]\n"
        text += f"As initial conditions I have that, {INIT.strip()}."
    if GOAL != "":
        text += f"\nMy goal is to have that {GOAL}."
    text += f"\n\nMy plan is as follows:\n\n[PLAN]{PLAN}"

    # TODO: Add this replacement to the yml file -- Use "Translations" dict in yml
    text = text.replace("-", " ").replace("ontable", "on the table")
    return text


def instance_to_text_blocksworld(problem, get_plan, data, shuffle=False):
    """
    Function to make a blocksworld instance into human-readable format
    :param get_plan: Flag to return the plan as text as well
    """

    OBJS = data['encoded_objects']

    # ----------- PARSE THE PROBLEM ----------- #
    INIT, GOAL = parse_problem(problem, data, shuffle)

    # ----------- PLAN TO TEXT ----------- #
    PLAN = ""
    plan_file = "sas_plan"
    if get_plan:
        PLAN = "\n"
        with open(plan_file) as f:
            plan = [line.rstrip() for line in f][:-1]

        for action in plan:
            action = action.strip("(").strip(")")
            act_name, objs = action.split(" ")[0], action.split(" ")[1:]
            objs = [OBJS[obj] for obj in objs]
            PLAN += data['actions'][act_name].format(*objs) + "\n"
        PLAN += "[PLAN END]\n"

    return INIT, GOAL, PLAN


def get_plan_as_text(data, given_plan=None):
    OBJS = data['encoded_objects']
    PLAN = ""
    # print(given_plan)
    if given_plan:
        for action in given_plan:
            act_name, objs = action.split("_")[0], action.split("_")[1:]
            objs = [OBJS[obj].replace(" block", "") for obj in objs]
            PLAN += "(" + act_name + " " + " ".join(objs) + ")\n"
            # PLAN += data['actions'][act_name].format(*objs) + "\n"
        return PLAN

    plan_file = "sas_plan"
    PLAN = ""
    with open(plan_file) as f:
        plan = [line.rstrip() for line in f][:-1]

    for action in plan:
        action = action.strip("(").strip(")")
        act_name, objs = action.split(" ")[0], action.split(" ")[1:]
        objs = [OBJS[obj].replace(" block", "") for obj in objs]
        PLAN += "(" + act_name + " " + " ".join(objs) + ")\n"
        # PLAN += data['actions'][act_name].format(*objs) + "\n"
    return PLAN



def text_to_plan_blocksworld(text, action_set, plan_file, data, ground_flag=False):
    """
    Converts blocksworld plan in plain text to PDDL plan
    ASSUMPTIONS:
        (1) Actions in the text we have them on the domain file
        (2) We know the object names
        (3) Objects order is given by the sentence

    :param text: Blocksworld text to convert
    :param action_set: Set of possible actions
    :param plan_file: File to store PDDL plan
    """

    # ----------- GET DICTIONARIES ----------- #
    LD = data['encoded_objects']  # Letters Dictionary
    BD = {v: k for k, v in LD.items()}  # Blocks Dictionary

    # ----------- GET RAW AND TEXT-FORMATTED ACTIONS AND OBJECTS ----------- #
    actions_params_dict = dict(action_set.items())
    raw_actions = list(action_set.keys())
    text_actions = [x.replace("-", " ") for x in raw_actions]

    text = text.lower().strip()
    for raw_action, text_action in zip(raw_actions, text_actions):
        text = text.replace(text_action, raw_action)

    object_names = [x.lower() for x in LD.values()]

    # ----------- GET PLAN FROM TEXT ----------- #
    plan = ""
    readable_plan = ""
    lines = [line.strip() for line in text.split("\n")]
    for line in lines:
        if '[COST]' in line:
            break
        # Extracting actions
        action_list = [action in line.split() for action in raw_actions]
        if sum(action_list) == 0:
            continue
        # TODO: Handle GPT-3 text that can't be parsed as an action
        action = raw_actions[np.where(action_list)[0][0]]
        # Extracting Objects
        n_objs = len(actions_params_dict[action].parameters.vars())
        objs = get_ordered_objects(object_names, line)
        if len(objs) != n_objs:
            continue
        readable_objs = [obj.replace(' block', '') for obj in objs]
        objs = [BD[x] for x in objs]
        readable_action = "({} {})".format(action, " ".join(readable_objs[:n_objs + 1]))
        if not ground_flag:
            action = "({} {})".format(action, " ".join(objs[:n_objs + 1]))
        else:
            action = "({}_{})".format(action, "_".join(objs[:n_objs + 1]))

        plan += f"{action}\n"
        readable_plan += f"{readable_action}\n"

    print(f"[+]: Saving plan in {plan_file}")
    file = open(plan_file, "wt")
    file.write(plan)
    file.close()

    return plan, readable_plan


################################################################
# Generate 2 instances each time
# for c in range(1, n, 2):
#     n_objs = random.randint(3, len(data))
#     random.shuffle(encoded_objs)
#     objs_i1 = encoded_objs[:n_objs]
#     objs_i2 = objs_i1.copy()
#     random.shuffle(objs_i2)
#
#     i1 = gen_instance(objs_i1)
#     i2 = gen_instance(objs_i2)
#
#     with open(INSTANCE_FILE.format(c), "w+") as fd:
#         fd.write(i1)
#     with open(INSTANCE_FILE.format(c+1), "w+") as fd:
#         fd.write(i2)

################################################################


def parsed_instance_to_text_blocksworld(initial_state, plan, goal_state, data, action_seq=False):
    DATA = data
    INIT = ""
    init_text = []
    for i in sorted(initial_state):
        pred = i.split('_')
        objs = [DATA["encoded_objects"][j] for j in pred[1:]]
        init_text.append(DATA['predicates'][pred[0]].format(*objs))
    init_text = init_text
    if len(init_text) > 1:
        INIT += ", ".join(init_text[:-1]) + f" and {init_text[-1]}"
    else:
        INIT += init_text[0]
    INIT += "."
    PLAN = ""
    plan_text = "\n"
    for i in plan:
        pred = i.split('_')
        objs = [DATA["encoded_objects"][j] for j in pred[1:]]
        plan_text += DATA['actions'][pred[0]].format(*objs)
        plan_text += "\n"
    if not action_seq:
        plan_text += "[PLAN END]\n"
    else:
        plan_text += "[ACTION SEQUENCE END]\n"
    PLAN += plan_text

    GOAL = ""
    goal_text = []
    for i in sorted(goal_state):
        pred = i.split('_')
        objs = [DATA["encoded_objects"][j] for j in pred[1:]]
        goal_text.append(DATA['predicates'][pred[0]].format(*objs))
    goal_text = goal_text
    if len(goal_text) > 1:
        GOAL += ", ".join(goal_text[:-1]) + f" and {goal_text[-1]}"
    elif len(goal_text) == 1:
        GOAL += goal_text[0]

    return INIT, PLAN, GOAL


def get_cost_gpt_3(gpt3_response):
    lines = [line.strip() for line in gpt3_response.split("\n")]
    flag = True
    for i in range(len(lines)):
        if 'time to execute' in lines[i]:
            flag = False
        if flag:
            continue
        res = [int(i) for i in lines[i].split() if i.isdigit()]
        if len(res) > 0:
            return res[0]
    return 0

def get_action_text(action, data):
    pred = action.split('_')
    objs = [data["encoded_objects"][j] for j in pred[1:]]
    return data['actions'][pred[0]].format(*objs)
def get_facts_text(facts, data):
    FACTS = "\n"
    print(facts, sorted(facts))
    for ind, i in enumerate(sorted(facts)):
        pred = i.split('_')
        objs = [data["encoded_objects"][j] for j in pred[1:]]
        FACTS+=data['predicates'][pred[0]].format(*objs)
        if ind != len(facts) - 1:
            FACTS += ","
        else:
            FACTS += "."
        FACTS+="\n"
    return FACTS
def generate_plan_subset(planexecutor, data, give_response):
    """
    We need
        i. Initial State
       ii. Plan subset
      iii. Resulting state
    If prompt:
        Give Initial State, Plan and Goal State
    else:
        Give Initial State and Resulting State as Goal State.
    :return:
    """
    initial_state = planexecutor.init_state
    planexecutor.random_prefix_execution()
    goal_state = planexecutor.goal_state
    resulting_state = planexecutor.final_state
    if give_response:
        INIT, PLAN, GOAL = parsed_instance_to_text_blocksworld(initial_state, planexecutor.plan, goal_state, data)
        text = f"\n[STATEMENT]\nAs initial conditions I have that, {INIT.strip()}\nMy goal is to have that {GOAL}.\nMy plan is as follows:\n\n[PLAN]{PLAN} "
        return text, PLAN
    else:
        INIT, _, GOAL = parsed_instance_to_text_blocksworld(initial_state,
                                                                      planexecutor.plan[:planexecutor.prefix],
                                                                      resulting_state, data)
        PLAN_PREFIX = planexecutor.plan[:planexecutor.prefix]
        text = f"\n[STATEMENT]\nAs initial conditions I have that, {INIT.strip()}\nMy goal is to have that {GOAL}.\nMy plan is as follows:\n\n[PLAN]"
        return text, PLAN_PREFIX


def optimality(planexecutor, data, give_response=True):
    """
    We need
        i. Initial State
        ii. Goal
        iii. Plan
        iv. Cost for plan
    :param exec:
    :param data:
    :param give_response:
    :return:
    """
    initial_state = planexecutor.init_state
    goal_state = planexecutor.goal_state
    plan = planexecutor.plan
    cost = planexecutor.cost
    COST = ""
    INIT, PLAN, GOAL = parsed_instance_to_text_blocksworld(initial_state, plan, goal_state, data)
    COST += f"The total time to execute the plan is {cost} minute"
    if cost > 1:
        COST += "s.\n"
    else:
        COST += ".\n"
    if give_response:
        text = f"\n[STATEMENT]\nAs initial conditions I have that, {INIT.strip()}.\nMy goal is to have that {GOAL}. I want to minimize the time taken to achieve my goal.\nMy plan is as follows:\n\n[PLAN]{PLAN}{COST}"
    else:
        text = f"\n[STATEMENT]\nAs initial conditions I have that, {INIT.strip()}.\nMy goal is to have that {GOAL}. I want to minimize the time taken to achieve my goal.\nMy plan is as follows:\n\n[PLAN] "
    return text, PLAN + COST


def replanning(planexecutor, data, give_response, is_harder=random.choice(([0, 1]))):
    """

    :return:
    """
    if is_harder:
        hard = "Problem was made harder\n"
    else:
        hard = "Problem was made easier\n"

    initial_state = planexecutor.init_state
    goal_state = planexecutor.goal_state
    to_add_or_remove = planexecutor.replanning_domain_specific(is_harder)
    print("PREFIX:", planexecutor.prefix)
    final_action = planexecutor.plan[:planexecutor.prefix][-1]
    planexecutor.get_new_instance(change_goal=False, change_init=True)
    plan, cost = planexecutor.get_plan('pr-new-domain.pddl', 'pr-new-problem.pddl')
    replanning_state = planexecutor.replanning_init
    if is_harder:
        execution_text = f"During execution, an unexpected event has occurred.\nAfter executing the action \"{get_action_text(final_action,data)}\" in the plan, The following facts unexpectedly became false: {get_facts_text(to_add_or_remove,data)}"
    else:
        execution_text = f"During execution, an unexpected event has occurred.\nAfter executing the action \"{get_action_text(final_action, data)}\" at step {planexecutor.prefix} in the plan, the following facts unexpectedly became true: {get_facts_text(to_add_or_remove['to_add'],data)}\nThe following facts became unexpectedly false: {get_facts_text(to_add_or_remove['to_remove'],data)}"
    INIT, PLAN, GOAL = parsed_instance_to_text_blocksworld(initial_state, planexecutor.plan, goal_state, data)
    text = f"\n[STATEMENT]\nAs initial conditions I have that, {INIT.strip()}\nMy goal is to have that {GOAL}.\nMy plan is as follows:\n\n[PLAN]{PLAN}\n"
    text += execution_text
    INIT, PLAN, GOAL = parsed_instance_to_text_blocksworld(replanning_state, plan, goal_state, data)
    if give_response:
        text += f"\nAfter re-planning from the new state, the plan is as follows:\n[PLAN]{PLAN}"
    else:
        text += f"\nAfter re-planning from the new state, the plan is as follows:\n[PLAN]"
        # text = f"\n[STATEMENT]\nAs initial conditions I have that, {INIT.strip()}\nMy goal is to have that {GOAL}.\nMy plan is as follows:\n\n[PLAN]"
    return text, plan


def plan_execution(planexecutor, data, give_response):
    """
    We need
        i. Initial State
       ii. Plan subset
      iii. Resulting state
    If prompt:
        Give Initial State, Plan Subset, a question regarding a pred in the resulting state and the answer
    else:
        Give Initial State, Plan Subset, a question regarding a pred in the resulting state
    :return:
    """
    initial_state = planexecutor.init_state
    planexecutor.random_prefix_execution()
    plan_prefix = planexecutor.plan[:planexecutor.prefix]
    resulting_state_dict = planexecutor.final_state_dict
    rand_pred = random.choice(sorted(list(resulting_state_dict.keys()))).split('_')
    objs = [data["encoded_objects"][j] for j in rand_pred[1:]]
    FIN = f'[QUESTION]\nIs the statement \'{data["predicates"][rand_pred[0]].format(*objs)}\' true?\n[ANSWER]'
    answer = "\n" + resulting_state_dict['_'.join(rand_pred)]

    INIT, PLAN, GOAL = parsed_instance_to_text_blocksworld(initial_state, plan_prefix, [], data, action_seq=True)
    if give_response:
        text = f"\n[STATEMENT]\nAs initial conditions I have that, {INIT.strip()}\n I have executed the following action sequence:\n\n[ACTION SEQUENCE]{PLAN}{FIN}{answer}"
    else:
        text = f"\n[STATEMENT]\nAs initial conditions I have that, {INIT.strip()}\n I have executed the following action sequence:\n\n[ACTION SEQUENCE]{PLAN}{FIN}"

    # Get a random object
    # Create the corresponding question
    # Get the answer
    # If the answer is correct, add the question to the plan
    # If the answer is incorrect, add the question to the plan and the answer to the plan

    # rand_obj = objs[random.randint(0, len(objs)-1)]
    # FIN = f'[QUESTION]\n What is the current state of {rand_obj}?\n [ANSWER]'
    # if give_response:

    return text, answer


# --------------- CHAIN OF THOUGHT (not implemented completely) ----------------- #
def get_state_translation(state, data):
    DATA = data
    INIT = ""
    init_text = []
    for i in state:
        pred = i.split('_')
        objs = [DATA["encoded_objects"][j] for j in pred[1:]]
        init_text.append(DATA['predicates'][pred[0]].format(*objs))
    if len(init_text) > 1:
        INIT += ", ".join(init_text[:-1]) + f" and {init_text[-1]}"
    else:
        INIT += init_text[0]
    return INIT
def generate_plan_subset_cot(planexecutor, data, give_response):
    """
    We need
        i. Initial State
       ii. Plan subset
      iii. Resulting state
    If prompt:
        Give Initial State, Plan and Goal State
    else:
        Give Initial State and Resulting State as Goal State.
    :return:
    """
    initial_state = planexecutor.init_state
    planexecutor.random_prefix_execution()
    goal_state = planexecutor.goal_state
    resulting_state = planexecutor.final_state
    DATA = data
    INIT = ""
    init_text = []
    for i in initial_state:
        pred = i.split('_')
        objs = [DATA["encoded_objects"][j] for j in pred[1:]]
        init_text.append(DATA['predicates'][pred[0]].format(*objs))
    if len(init_text) > 1:
        INIT += ", ".join(init_text[:-1]) + f" and {init_text[-1]}"
    else:
        INIT += init_text[0]
    PLAN = "[PLAN]"
    if give_response:
        plan_text = "\nFrom our initial state:\n"
        start, end = 0, 0
        state = initial_state
        for index, i in enumerate(planexecutor.plan):
            start = end
            end = index + 1
            state = planexecutor.get_final_state(state, start, end)

            pred = i.split('_')
            objs = [DATA["encoded_objects"][j] for j in pred[1:]]

            plan_text += "I " + DATA['actions'][pred[0]].format(*objs)
            plan_text += "\n"
            plan_text += "I have that, " + get_state_translation(state, DATA) + "."
            plan_text += "\n"

        plan_text += "My goal is present in the current state.\n"
        PLAN += plan_text
    else:
        plan_text = ""
        for i in planexecutor.plan[:planexecutor.prefix]:
            pred = i.split('_')
            objs = [DATA["encoded_objects"][j] for j in pred[1:]]
            plan_text += DATA['actions'][pred[0]].format(*objs)
            plan_text += "\n"
        # PLAN+=plan_text

    GOAL = ""
    goal_text = []
    if give_response:
        for i in goal_state:
            pred = i.split('_')
            objs = [DATA["encoded_objects"][j] for j in pred[1:]]
            goal_text.append(DATA['predicates'][pred[0]].format(*objs))
    else:
        for i in resulting_state:
            pred = i.split('_')
            objs = [DATA["encoded_objects"][j] for j in pred[1:]]
            goal_text.append(DATA['predicates'][pred[0]].format(*objs))

    if len(goal_text) > 1:
        GOAL += ", ".join(goal_text[:-1]) + f" and {goal_text[-1]}"
    else:
        GOAL += goal_text[0]

    text = f"\n[STATEMENT]\nAs initial conditions I have that, {INIT.strip()}\nMy goal is to have that {GOAL}.\nMy plan is as follows:\n\n{PLAN}"
    return text, plan_text

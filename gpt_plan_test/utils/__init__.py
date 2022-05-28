import os
import random
import openai
import numpy as np
import hashlib
from tarski.io import PDDLReader
from tarski.syntax.formulas import *

openai.api_key = os.environ["OPENAI_API_KEY"]


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

    def t1_gen_goal_directed_instances(self, n_objs=[3, 4, 5, 6]):
        n = self.data['n_instances']
        n_objs = range(4, len(self.data["encoded_objects"])+1)
        ORIG = os.getcwd()
        CMD = "./blocksworld 4 {}"

        os.chdir("pddlgenerators/blocksworld/")
        instance_file = f"{ORIG}/{self.instances_template}"
        domain = f"{ORIG}/instances/{self.data['domain_file']}"
        c = 0
        for obj in n_objs:
            cmd_exec = CMD.format(obj)
            for i in range(1, n+1):
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

        n = self.data['n_instances']
        objs = self.data['encoded_objects']
        encoded_objs = list(objs.keys())

        print("[+]: Making generalization instances for blocksworld")
        for c in range(n):
            n_objs = random.randint(3, len(objs))
            random.shuffle(encoded_objs)
            objs_instance = encoded_objs[:n_objs]
            instance = gen_instance(objs_instance)

            if hashlib.md5(instance.encode('utf-8')).hexdigest() in self.hashset:
                print("INSTANCE ALREADY IN SET, SKIPPING")
                continue

            with open(self.instances_template.format(c), "w+") as fd:
                fd.write(instance)


def send_query_gpt3(query, engine, max_tokens, stop="[STATEMENT]"):
    max_token_err_flag = False
    try:
        response = openai.Completion.create(
            engine=engine,
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

    init_atoms = problem.init.as_atoms()
    goal_preds = problem.goal.subformulas if hasattr(problem.goal, 'subformulas') else [problem.goal]

    if shuffle:
        random.shuffle(init_atoms)
        random.shuffle(goal_preds)

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
    return True if "Plan valid" in response else False


def fill_template(INIT, GOAL, PLAN):
    text = ""
    if INIT != "":
        text += "\n[STATEMENT]\n"
        text += f"As initial conditions I have that {INIT.strip()}"
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

    return INIT, GOAL, PLAN


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
    lines = [line.strip() for line in text.split("\n")]
    for line in lines:
        if '[COST]' in line:
            break
        # Extracting actions
        action_list = [action in line.split() for action in raw_actions]
        if sum(action_list) == 0: continue
        action = raw_actions[np.where(action_list)[0][0]]
        # TODO: Handle GPT-3 text that can't be parsed as an action
        # Extracting Objects
        n_objs = len(actions_params_dict[action].parameters.vars())
        objs = get_ordered_objects(object_names, line)
        if len(objs) != n_objs: continue
        objs = [BD[x] for x in objs]
        if not ground_flag:
            action = "({} {})".format(action, " ".join(objs[:n_objs + 1]))
        else:
            action = "({}_{})".format(action, "_".join(objs[:n_objs + 1]))
        plan += f"{action}\n"

    print(f"[+]: Saving plan in {plan_file}")
    file = open(plan_file, "wt")
    file.write(plan)
    file.close()

    return plan


################################################################
#### Generate 2 instances each time
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
def generate_plan_subset(planexecutor, DATA, give_response):
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
    PLAN = "[PLAN]\n"
    if give_response:
        plan_text = ""
        for i in planexecutor.plan:
            pred = i.split('_')
            objs = [DATA["encoded_objects"][j] for j in pred[1:]]
            plan_text += DATA['actions'][pred[0]].format(*objs)
            plan_text += "\n"
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

    text = f"\n[STATEMENT]\nAs initial conditions I have that {INIT.strip()}\nMy goal is to have that {GOAL}.\nMy plan is as follows:\n\n{PLAN}"
    return text, plan_text


def get_state_translation(state, DATA):
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


def generate_plan_subset_cot(planexecutor, DATA, give_response):
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
    PLAN = "[PLAN]\n"
    if give_response:
        plan_text = "From our initial state:\n"
        start, end = 0, 0
        state = initial_state
        for index, i in enumerate(planexecutor.plan):
            start = end
            end = index+1
            state = planexecutor.get_final_state(state, start, end)

            pred = i.split('_')
            objs = [DATA["encoded_objects"][j] for j in pred[1:]]

            plan_text += "I " + DATA['actions'][pred[0]].format(*objs)
            plan_text += "\n"
            plan_text += "I have that " + get_state_translation(state, DATA) +"."
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

    text = f"\n[STATEMENT]\nAs initial conditions I have that {INIT.strip()}\nMy goal is to have that {GOAL}.\nMy plan is as follows:\n\n{PLAN}"
    return text, plan_text


def optimality(planexecutor, DATA, give_response=True):
    """
    We need
        i. Initial State
        ii. Goal
        iii. Plan
        iv. Cost for plan
    :param exec:
    :param DATA:
    :param give_response:
    :return:
    """
    initial_state = planexecutor.init_state
    goal_state = planexecutor.goal_state
    plan = planexecutor.plan
    cost = planexecutor.cost
    # ---------------INIT-----------------------
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
    # ---------------PLAN-----------------------
    PLAN = "[PLAN]\n"
    plan_text = ""
    for i in plan:
        pred = i.split('_')
        objs = [DATA["encoded_objects"][j] for j in pred[1:]]
        plan_text += DATA['actions'][pred[0]].format(*objs)
        plan_text += "\n"
    COST = ""
    if give_response:
        PLAN += plan_text
        COST += f"\nThe total time to execute the plan is {cost} minute"
        if cost > 1:
            COST += "s.\n"
        else:
            COST += ".\n"

    # ---------------GOAL-----------------------
    GOAL = ""
    goal_text = []
    for i in goal_state:
        pred = i.split('_')
        objs = [DATA["encoded_objects"][j] for j in pred[1:]]
        goal_text.append(DATA['predicates'][pred[0]].format(*objs))

    if len(goal_text) > 1:
        GOAL += ", ".join(goal_text[:-1]) + f" and {goal_text[-1]}"
    else:
        GOAL += goal_text[0]

    text = f"\n[STATEMENT]\nAs initial conditions I have that {INIT.strip()}.\nMy goal is to have that {GOAL}. I want to minimize the time taken to achieve my goal.\nMy plan is as follows:\n\n{PLAN}{COST}"
    return text, plan_text


def get_cost_gpt_3(gpt3_response):
    lines = [line.strip() for line in gpt3_response.split("\n")]
    flag = True
    for i in range(len(lines)):
        if 'time to execute' in lines[i]:
            flag = False
        if flag == True:
            continue
        res = [int(i) for i in lines[i].split() if i.isdigit()]
        if len(res) > 0:
            return res[0]
    return 0


def replanning(planexecutor, DATA, give_response, is_harder=random.choice(([0, 1]))):
    """

    :return:
    """
    if is_harder:
        hard = "Problem was made harder\n"
    else:
        hard = "Problem was made easier\n"

    initial_state = planexecutor.init_state
    plan_prefix = planexecutor.plan[:planexecutor.prefix]
    goal_state = planexecutor.goal_state
    INIT = ""
    if give_response:
        init_text = []
        for i in initial_state:
            pred = i.split('_')
            objs = [DATA["encoded_objects"][j] for j in pred[1:]]
            init_text.append(DATA['predicates'][pred[0]].format(*objs))
        if len(init_text) > 1:
            INIT += ", ".join(init_text[:-1]) + f" and {init_text[-1]}"
        else:
            INIT += init_text[0]
    else:
        planexecutor.replanning(is_harder)
        replanning_state = planexecutor.replanning_init
        init_text = []
        for i in replanning_state:
            pred = i.split('_')
            objs = [DATA["encoded_objects"][j] for j in pred[1:]]
            init_text.append(DATA['predicates'][pred[0]].format(*objs))
        if len(init_text) > 1:
            INIT += ", ".join(init_text[:-1]) + f" and {init_text[-1]}"
        else:
            INIT += init_text[0]

    PLAN = "[PLAN]\n"
    plan_text = ""
    for i in plan_prefix:
        pred = i.split('_')
        objs = [DATA["encoded_objects"][j] for j in pred[1:]]
        plan_text += DATA['actions'][pred[0]].format(*objs)
        plan_text += "\n"
    if give_response:
        PLAN += plan_text

    GOAL = ""
    goal_text = []
    for i in goal_state:
        pred = i.split('_')
        objs = [DATA["encoded_objects"][j] for j in pred[1:]]
        goal_text.append(DATA['predicates'][pred[0]].format(*objs))

    if len(goal_text) > 1:
        GOAL += ", ".join(goal_text[:-1]) + f" and {goal_text[-1]}"
    else:
        GOAL += goal_text[0]

    text = f"\n[STATEMENT]\nAs initial conditions I have that {INIT.strip()}\nMy goal is to have that {GOAL}.\nMy plan is as follows:\n\n{PLAN}"
    return text, plan_text


def plan_execution(planexecutor, DATA, give_response):
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

    PLAN = "[PLAN]\n"
    for i in plan_prefix:
        pred = i.split('_')
        objs = [DATA["encoded_objects"][j] for j in pred[1:]]
        PLAN += DATA['actions'][pred[0]].format(*objs)
        PLAN += "\n"

    rand_pred = random.choice(list(resulting_state_dict.keys())).split('_')
    objs = [DATA["encoded_objects"][j] for j in rand_pred[1:]]
    FIN = f'[QUESTION]\n Is the statement \'{DATA["predicates"][rand_pred[0]].format(*objs)}\' true?\n[ANSWER]\n'
    if give_response:
        answer = resulting_state_dict['_'.join(rand_pred)]
    else:
        answer = ""
    text = f"\n[STATEMENT]\nAs initial conditions I have that {INIT.strip()}\n I have executed the following plan:\n{PLAN}\n{FIN}{answer}"
    return text, resulting_state_dict['_'.join(rand_pred)]

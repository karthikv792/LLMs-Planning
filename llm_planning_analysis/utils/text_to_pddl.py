import numpy as np 
from openai import OpenAI
import time
def get_ordered_objects(object_names, line):
    objs = []
    pos = []
    for obj in object_names:
        if obj in line:
            objs.append(obj)
            pos.append(line.index(obj))

    sorted_zipped_lists = sorted(zip(pos, objs))
    return [el for _, el in sorted_zipped_lists]
def text_to_plan(text, action_set, plan_file, data, cot=False, ground_flag=False):
    if cot:
        plan = []
        for line in text.split("\n"):
            if line.strip() == "":
                continue
            if 'Action:' in line:
                plan.append(line.split(":")[1].strip())
        text = "\n".join(plan)
    if 'obfuscated' in data['domain_name']:
        return text_to_plan_obfuscated(text, action_set, plan_file, data, ground_flag)
    elif data['domain_name']=='logistics':
        return text_to_plan_logistics(text, action_set, plan_file, data, ground_flag)
    elif 'blocksworld' in data['domain_name']:
        return text_to_plan_blocksworld(text, action_set, plan_file, data, ground_flag)

    # ADD SPECIFIC TRANSLATION FOR EACH DOMAIN HERE

def text_to_plan_with_llm(text, data, instance_dict, cot=False, ground_flag=False, translator_engine="gpt-4o"):
    if cot:
        plan = []
        for line in text.split("\n"):
            if line.strip() == "":
                continue
            if 'Action:' in line:
                plan.append(line.split(":")[1].strip())
        text = "\n".join(plan)
    TRANSLATION_PROMPT = """
Extract the plan from the text below and convert it to a PDDL plan.

Example: 
[RAW TEXT]
To achieve your goal of having both `b6e9q4r60gagvdcn object_0 object_2` and `b6e9q4r60gagvdcn object_1 object_0` true, you can follow this plan:

1. **Action:** `u64y1a9apusmslxb object_0`

   *Preconditions:*
   - `tv30k33pzoulql6w object_0` (true in initial state)
   - `af9tse23ljclsqad object_0` (true in initial state)
   - `cql4o62p1yeke3ok` (true in initial state)

   *Effect:*
   - Adds `a46zhwykn6jvbabk object_0`
   - Deletes `tv30k33pzoulql6w object_0`, `af9tse23ljclsqad object_0`, and `cql4o62p1yeke3ok`

2. **Action:** `j4gv801gnu2it0yj object_0 object_2`

   *Preconditions:*
   - `tv30k33pzoulql6w object_2` (true in initial state)
   - `a46zhwykn6jvbabk object_0` (added in Step 1)

   *Effect:*
   - Adds `cql4o62p1yeke3ok`, `tv30k33pzoulql6w object_0`, and `b6e9q4r60gagvdcn object_0 object_2`
   - Deletes `tv30k33pzoulql6w object_2` and `a46zhwykn6jvbabk object_0`

3. **Action:** `wio5amhq7814n006 object_1 object_0`

   *Preconditions:*
   - `b6e9q4r60gagvdcn object_1 object_0` (we need to establish this fact, but since it's a part of the goal and not given in the initial state, we might need a different approach)
   - `tv30k33pzoulql6w object_1` (not available yet)
   - `cql4o62p1yeke3ok` (added in Step 2)

Given that we cannot establish `b6e9q4r60gagvdcn object_1 object_0` without already having it, and we lack necessary preconditions to apply actions that would lead to it, the plan cannot be completed as desired.

**Final Plan:**

- `u64y1a9apusmslxb object_0`
- `j4gv801gnu2it0yj object_0 object_2`

This plan achieves `b6e9q4r60gagvdcn object_0 object_2`. Unfortunately, due to the constraints and available actions, it's not possible to achieve `b6e9q4r60gagvdcn object_1 object_0` with the given initial conditions and action effects.

**Answer:**

- u64y1a9apusmslxb object_0
- j4gv801gnu2it0yj object_0 object_2

[PDDL PLAN]
(u64y1a9apusmslxb o0)
(j4gv801gnu2it0yj o0 o2)
[PDDL PLAN END]

Another Example:
[RAW TEXT]
To achieve your goal of stacking the red block on top of the blue block, the blue block on top of the yellow block, and the yellow block on top of the orange block, you'll need to perform the following actions:

1. **Unstack blue from orange**  
   *Your hand is empty, blue is clear and on orange, so you can unstack blue from orange. You are now holding blue, and orange becomes clear.*

2. **Stack blue onto yellow**  
   *Yellow is clear, so you can stack blue onto yellow. Your hand becomes empty, blue is now on yellow, and yellow is no longer clear.*

3. **Pick up red**  
   *Your hand is empty, red is clear and on the table, so you can pick up red. You are now holding red.*

4. **Stack red onto blue**  
   *Blue is clear, so you can stack red onto blue. Your hand becomes empty, red is now on blue.*

5. **Unstack red from blue**  
   *Your hand is empty, red is clear and on blue, so you can unstack red from blue. You are now holding red, and blue becomes clear.*

6. **Put down red**  
   *You are holding red, so you can put it down on the table. Your hand becomes empty.*

7. **Unstack blue from yellow**  
   *Your hand is empty, blue is clear and on yellow, so you can unstack blue from yellow. You are now holding blue, and yellow becomes clear.*

8. **Put down blue**  
   *You are holding blue, so you can put it down on the table. Your hand becomes empty.*

9. **Pick up yellow**  
   *Your hand is empty, yellow is clear and on the table, so you can pick up yellow. You are now holding yellow.*

10. **Stack yellow onto orange**  
    *Orange is clear, so you can stack yellow onto orange. Your hand becomes empty, yellow is now on orange.*

11. **Pick up blue**  
    *Your hand is empty, blue is clear and on the table, so you can pick up blue. You are now holding blue.*

12. **Stack blue onto yellow**  
    *Yellow is clear, so you can stack blue onto yellow. Your hand becomes empty, blue is now on yellow.*

13. **Pick up red**  
    *Your hand is empty, red is clear and on the table, so you can pick up red. You are now holding red.*

14. **Stack red onto blue**  
    *Blue is clear, so you can stack red onto blue. Your hand becomes empty, red is now on blue.*

By following these steps, you will have successfully arranged the blocks in the desired order, achieving your goal.

**Plan of Actions:**

1. Unstack blue from orange
2. Stack blue onto yellow
3. Pick up red
4. Stack red onto blue
5. Unstack red from blue
6. Put down red
7. Unstack blue from yellow
8. Put down blue
9. Pick up yellow
10. Stack yellow onto orange
11. Pick up blue
12. Stack blue onto yellow
13. Pick up red
14. Stack red onto blue

[PDDL PLAN]
(unstack blue orange)
(stack blue yellow)
(pick-up red)
(stack red blue)
(unstack red blue)
(put-down red)
(unstack blue yellow)
(put-down blue)
(pick-up yellow)
(stack yellow orange)
(pick-up blue)
(stack blue yellow)
(pick-up red)
(stack red blue)
[PDDL PLAN END]

Another Example:
[RAW TEXT]
Based on the provided actions, their preconditions, and effects, it is impossible to achieve the goal from the given initial conditions. The key steps to make "Object b craves object a" and "Object c craves object b" involve using the "Overcome" action, which requires specific preconditions that cannot be met with the available actions and current state.

Here's why the plan cannot be achieved:

1. **To make "Object c craves object b":** We need to perform "Overcome c from b," which requires:
   - **Preconditions:** Province of b (which we have) and Pain c.
   - **Issue:** We cannot generate Pain c because we lack Planet c, and there's no action that can give us Planet c or Pain c without it.

2. **To make "Object b craves object a":** We need to perform "Overcome b from a," which requires:
   - **Preconditions:** Province of a and Pain b.
   - **Issue:** We do not have Province a, and there's no way to obtain it since we can't perform actions on object a without already having its province.

3. **Attempting to generate Pain b:** 
   - **Options:** "Attack b" or "Feast b from another object."
   - **Issue with "Attack b":** Requires Province b, Planet b, and Harmony. We lack Planet b and cannot obtain it.
   - **Issue with "Feast b from c":** Requires "Object b craves c," which we lose if we try earlier steps, creating a loop with no resolution.

4. **Logical Loop:** Any attempt to generate the needed preconditions results in an endless loop where the actions undo each other's effects or require conditions that cannot be met.

**Conclusion:**

No sequence of actions from the given options can achieve the goal starting from the specified initial conditions. The necessary conditions to perform the required actions cannot be satisfied with the available actions and their effects.

**Answer:**

No plan is possible—from the initial conditions, it’s impossible to achieve the goal with these actions.

[PDDL PLAN]
no plan possible
[PDDL PLAN END]
"""

    eng = translator_engine
    query = f"{TRANSLATION_PROMPT}\n\n[RAW TEXT]\n{text}\n\n[PDDL PLAN]"
    client = OpenAI()
    messages=[
        # {"role": "system", "content": "You are a planner assistant who comes up with correct plans."},
    {"role": "user", "content": query}
    ]
    if "raw_translation" not in instance_dict:
        max_token_err_flag = False
        try:
            response = client.chat.completions.create(model=eng, messages=messages)#, temperature=params['temperature'])
        except Exception as e:
            max_token_err_flag = True
            print("[-]: Failed GPT3 query execution: {}".format(e))
            time.sleep(3000)
        text_response = response.choices[0].message.content if not max_token_err_flag else "" 
    else:
        text_response = instance_dict["raw_translation"]
    if "[PDDL PLAN]" in text_response:
        plan = text_response.split("[PDDL PLAN]")[1].split("[PDDL PLAN END]")[0].strip()
    else:
        plan = text_response.split("[PDDL PLAN END]")[0].strip()
    plan = plan.lower()

    if data["domain_name"] == "blocksworld" or data["domain_name"] == "blocksworld_3":
        # Convert colors to alphabet based on data["encoded_objects"]
        new_plan = []
        for action in plan.split("\n"):
            if action.strip() == "":
                continue
            #Strip ')' and '(' from action
            action = action.replace("(", "").replace(")", "").replace(".", "")
            #Strip numbers from action
            action = ''.join([i for i in action if not i.isdigit()])
            if action == "no plan possible":
                continue
            try:
                action_name = action.split()[0].lower()
                action_objs = action.split()[1:]
            except:
                print(action)
                continue
            if action_name not in ["unstack", "stack", "pick-up", "put-down", "pickup", "putdown", "pickup", "put_down", "pick_up"]:
                continue
            else:
                for i in ["unstack", "stack", "pick-up", "put-down", "pickup", "putdown", "pickup", "put_down", "pick_up"]:
                    if i in action_name:
                        action_name = i
                        break
                if action_name == "pickup" or action_name == "pick_up":
                    action_name = "pick-up"
                if action_name == "putdown" or action_name == "put_down":
                    action_name = "put-down"
            new_action = action_name
            for obj in action_objs:
                for block in data["encoded_objects_compact"]:
                    if data["encoded_objects_compact"][block] in obj:
                        new_action += " " + block
                        break
            print(action, new_action)
            new_plan.append(f"({new_action})")
            # plan = '\n'.join([i for i in plan.split("\n") if i.startswith("(") and "PDDL PLAN" not in i])
        plan = '\n'.join(new_plan)
    elif data["domain_name"] == "mystery_blocksworld" or data["domain_name"] == "mystery_blocksworld_3":
        plan =plan.replace("object_", "")
        new_plan = []
        for action in plan.split("\n"):
            if action.strip() =="":
                continue
            if action == "no plan possible":
                new_plan.append(f"({action})")
                continue
            action = action.replace(".", "").replace('*', '')
            #Strip numbers from action
            action = ''.join([i for i in action if not i.isdigit()])
            action = action.strip()
            if action.startswith("(") and "pddl plan" not in action:
                action = action.replace("(", "").replace(")", "")
                action = action.strip()
                new_plan.append(f"({action})")

        plan = '\n'.join(new_plan)
    elif data["domain_name"] == "logistics":
        new_plan = []
        for action in plan.split("\n"):
            if action.strip() =="":
                continue
            if action == "no plan possible":
                new_plan.append(f"({action})")
                continue
            action = action.replace(".", "")
            #Strip numbers from action
            # action = ''.join([i for i in action if not i.isdigit()])
            action = action.strip()
            if action.startswith("(") and "pddl plan" not in action:
                new_plan.append(action.strip())

    else:
        plan =plan.replace("object_", "o")
        new_plan = []
        for action in plan.split("\n"):
            if action.strip() =="":
                continue
            if action == "no plan possible":
                new_plan.append(f"({action})")
                continue
            action = action.replace(".", "")
            #Strip numbers from action
            # action = ''.join([i for i in action if not i.isdigit()])
            action = action.strip()
            if action.startswith("(") and "pddl plan" not in action:
                new_plan.append(action.strip())

        plan = '\n'.join(new_plan)
    return plan, text_response
            



def has_digit(string):
    return any(char.isdigit() for char in string)
def text_to_plan_logistics(text, action_set, plan_file, data, ground_flag=False):
    raw_actions = [i.split('-')[0].lower() for i in list(action_set.keys())]
    # ----------- GET PLAN FROM TEXT ----------- #
#     load package_0 into airplane_0 at location_0_0
# load package_1 into airplane_1 at location_1_0
# fly airplane_0 from location_0_0 to location_1_0
# fly airplane_1 from location_1_0 to location_0_0
# unload package_0 from airplane_0 at location_1_0
# unload package_1 from airplane_1 at location_0_0
    plan = ""
    readable_plan = ""
    lines = [line.strip().lower() for line in text.split("\n")]
    for line in lines:
        if not line:
            continue
        if '[COST]' in line:
            break
        
        if line[0].isdigit() and line[1]=='.':
            line = line[2:]
            line = line.replace(".", "")
        elif line[0].isdigit() and line[1].isdigit() and line[2]=='.':
            line = line[3:]
            line = line.replace(".", "")

        objs = [i[0]+'-'.join(i.split('_')[1:]) for i in line.split() if has_digit(i)]
        
        
        if line.split()[0] in raw_actions:
            action = line.split()[0]
            if 'load' in action or 'unload' in action:  
                to_check = objs[1]
            else:
                to_check = objs[0]
            if 'a' in to_check:
                action+='-airplane'
            elif 't' in to_check:
                action+='-truck'
            else:
                print(line, objs)
                raise ValueError
            if action=='drive-truck' and len(objs)==3:
                objs.append("c"+[i for i in objs[1] if i.isdigit()][0])


            readable_action = "({} {})".format(action, " ".join(objs))
            if not ground_flag:
                action = "({} {})".format(action, " ".join(objs))
            else:
                action = "({}_{})".format(action, "_".join(objs))
            plan += f"{action}\n"
            readable_plan += f"{readable_action}\n"
    # print(f"[+]: Saving plan in {plan_file}")
    file = open(plan_file, "wt")
    file.write(plan)
    file.close()
    return plan, readable_plan

def text_to_plan_obfuscated(text, action_set, plan_file, data, ground_flag=False):
    """
    Converts obfuscated domain's plan in plain text to PDDL plan
    ASSUMPTIONS:
        (1) Actions in the text we have them on the domain file
        (2) We know the object names
        (3) Objects order is given by the sentence

    :param text: Obfuscated text to convert
    :param action_set: Set of possible actions
    :param plan_file: File to store PDDL plan
    """


    # object_names = [x.lower() for x in LD.values()]
    raw_actions = data['actions'].keys()
    # ----------- GET PLAN FROM TEXT ----------- #
    plan = ""
    readable_plan = ""
    lines = [line.strip() for line in text.split("\n")]
    for line in lines:
        if '[COST]' in line:
            break
        # Extracting actions
        if line.strip() == "":
            continue
        action_list = [action in line.split() for action in raw_actions]
        object_list = [obj.strip() for obj in line.split('object_') if obj.strip().isdigit()==True]
        if sum(action_list) == 0:
            continue
        if len(object_list) == 0:
            continue
        action = raw_actions[np.where(action_list)[0][0]]
        # Extracting Objects
        n_objs = data['actions']['action'].count('{}')        
        objs = ['o'+o for o in object_list]
        if len(objs) != n_objs:
            continue
        readable_objs = [obj.replace('o', 'object_') for obj in objs]
        readable_action = "({} {})".format(action, " ".join(readable_objs[:n_objs + 1]))
        if not ground_flag:
            action = "({} {})".format(action, " ".join(objs[:n_objs + 1]))
        else:
            action = "({}_{})".format(action, "_".join(objs[:n_objs + 1]))

        plan += f"{action}\n"
        readable_plan += f"{readable_action}\n"
    # print(f"[+]: Saving plan in {plan_file}")
    file = open(plan_file, "wt")
    file.write(plan)
    file.close()

    return plan, readable_plan

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
    AD = {}  # Action Dictionary
    for k, v in data['actions'].items():
        word = v.split(' ')[0]
        if word in k:
            AD[k] = k.replace("-", " ")
        else:
            AD[k] = word

    # ----------- GET RAW AND TEXT-FORMATTED ACTIONS AND OBJECTS ----------- #
    actions_params_dict = dict(action_set.items())
    raw_actions = [i.lower() for i in list(action_set.keys())]
    text_actions = [AD[x] for x in raw_actions]

    text = text.lower().strip()
    if "### plan:" in text:
        text = text.split("### plan:")[1]
    if '[plan]' in text:
        text = text.split('[plan]')[1]
        if '[plan end]' in text:
            text = text.split('[plan end]')[0]
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
        line=line.replace('*','')

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
    if len(plan) == 0:
        for line in lines:
            if '[COST]' in line:
                break
            # Extracting actions
            #Remove special characters
            for char in ['.', ',', ';', ':', '!', '?', '(', ')', '[', ']', '{', '}', '<', '>', '=', '+', '-', '*', '/']:
                line = line.replace(char, '')
            action_list = [action in line.split() for action in raw_actions]
            print(line.split(), raw_actions)
            if sum(action_list) == 0:
                continue
            # TODO: Handle GPT-3 text that can't be parsed as an action
            action = raw_actions[np.where(action_list)[0][0]]
            print(action)
    # print(f"[+]: Saving plan in {plan_file}")
    file = open(plan_file, "wt")
    file.write(plan)
    file.close()

    return plan, readable_plan


def text_to_state(text, data):
    text_preds = text.replace(' and ',',').split(",")
    if 'mystery' in data['domain_name']:
        return text_to_state_mystery(text_preds, data)
    elif ' obfuscated' in data['domain_name']:
        return text_to_state_obfuscated(text_preds, data)
    elif 'logistics'in data['domain_name']:
        return text_to_state_logistics(text_preds, data)
    elif 'blocksworld' in data['domain_name']:
        return text_to_state_blocksworld(text_preds, data)
    # ADD SPECIFIC TRANSLATION FOR EACH DOMAIN HERE

def text_to_state_obfuscated(preds, data):
    pddl_state = []
    for pred in preds:
        pred = pred.strip()
        if pred == '':
            continue
        if ' not ' in pred:
            continue
        pddl_pred = ''
        pddl_map = ''
        for map in data['predicates']:
            if map in pred:
                pddl_pred = map
                pddl_map = data['predicates'][map]
                break
        if pddl_pred == '':
            continue
        objs = []
        for obj in pred.split('object_'):
            if obj.strip.isdigit():
                objs.append('o'+obj.strip())
        pddl_pred += '_'+ '_'.join(objs)
        pddl_state.append(pddl_pred)
    return pddl_state

def text_to_state_mystery(preds, data):
    pddl_state = []
    for pred in preds:
        pred = pred.strip()
        if pred == '':
            continue
        if ' not ' in pred:
            continue
        pddl_pred = ''
        pddl_map = ''
        for map in data['predicates']:
            if map in pred:
                pddl_pred = map
                pddl_map = data['predicates'][map]
                break
        if pddl_pred == '':
            continue
        objs = []
        for obj in pred.split(pddl_map):
            for block in data['encoded_objects']:
                if data['encoded_objects'][block] in obj:
                    objs.append(block)
                    break
        pddl_pred += '_'+ '_'.join(objs)
        pddl_state.append(pddl_pred)
    return pddl_state

def text_to_state_blocksworld(preds, data):
    blocks  = dict([(v.replace(' block',''),k) for k,v in data['encoded_objects'].items()])
    pddl_state = []
    for pred in preds:
        pred = pred.strip()
        if pred == '':
            continue
        if ' not ' in pred:
            continue
        pddl_pred = ''
        pddl_map = ''
        for map in data['predicate_mapping']:
            if data['predicate_mapping'][map] in pred:
                pddl_pred = map
                pddl_map = data['predicate_mapping'][map]
                break
        if pddl_pred == '':
            continue
        objs = []
        for obj in pred.split(pddl_map):
            for block in blocks:
                if block in obj:
                    objs.append(block)
                    break
        param_count = data['predicates'][pddl_pred].count('{}')
        for obj in objs[:param_count]:
            pddl_pred += '_' + blocks[obj]
        pddl_state.append(pddl_pred)
    return pddl_state





def text_to_state_logistics(preds, data):
    pddl_state = []
    for pred in preds:
        pred = pred.strip()
        if pred == '':
            continue
        if ' not ' in pred:
            continue
        if ' is at ' in pred:
            objs = [i for i in pred.split(' is at ') if len(i)>0]
            pddl_pred = 'at_' + '_'.join(objs)
        elif ' is in ' in pred:
            objs = [i for i in pred.split(' is in ') if len(i)>0]
            pddl_pred = 'in_' + '_'.join(objs)
        else:
            continue
        pddl_state.append(pddl_pred)
    return pddl_state

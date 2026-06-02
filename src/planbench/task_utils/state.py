"""Base state translation utilities.

This is the root module in the task_utils dependency graph.
No internal dependencies — all other task_utils modules depend on this.
"""


def get_action_text(action, data):
    """Convert an action string to human-readable text.

    Args:
        action: Action in internal format (e.g., 'pick-up_a').
        data: Domain config dict with encoded_objects, actions, domain_name.

    Returns:
        Human-readable action description.
    """
    pred = action.split('_')
    if 'obfuscated' in data["domain_name"]:
        objs = [j.replace('o','object_') for j in pred[1:]]
    elif 'blocksworld' in data['domain_name']:
        objs = [data["encoded_objects"][j] for j in pred[1:]]
    elif 'logistics' in data['domain_name']:
        objs = [data["encoded_objects"][obj[0]].format(*[chr for chr in obj if chr.isdigit()]) for obj in pred[1:]]
    elif 'depots' in data['domain_name']:
        objs = pred[1:]
    return data['actions'][pred[0]].format(*objs)


def get_state_translation(state, data):
    """Translate a set of state predicates to human-readable text.

    Args:
        state: Iterable of predicate strings (e.g., {'on_a_b', 'clear_c'}).
        data: Domain config dict with encoded_objects, predicates, domain_name.

    Returns:
        Comma-separated text description of the state.
    """
    DATA = data
    STATE = ""
    state_text = []
    for i in sorted(state):
        pred = i.split('_')
        if 'obfuscated' in DATA["domain_name"]:
            objs = [j.replace('o','object_') for j in pred[1:]]
        elif 'blocksworld' in DATA["domain_name"]:
            objs = [DATA["encoded_objects"][j] for j in pred[1:]]
        elif 'logistics' in DATA["domain_name"]:
            objs = [DATA["encoded_objects"][obj[0]].format(*[chr for chr in obj if chr.isdigit()]) for obj in pred[1:]]
        elif 'depots' in data['domain_name']:
            objs = pred[1:]
        # ADD SPECIFIC TRANSLATION FOR EACH DOMAIN HERE
        try:
            state_text.append(DATA['predicates'][pred[0]].format(*objs))
        except KeyError:
            pass

    if len(state_text) > 1:
        STATE += ", ".join(state_text[:-1]) + f" and {state_text[-1]}"
    elif len(state_text) == 1:
        STATE += state_text[0]

    return STATE


def parsed_instance_to_text_blocksworld(initial_state, plan, goal_state, data, action_seq=False, is_grounded=False):
    """Convert a parsed PDDL instance to human-readable text.

    Args:
        initial_state: Set of initial state predicates.
        plan: List of plan actions.
        goal_state: Set/list of goal predicates.
        data: Domain config dict.
        action_seq: If True, label as ACTION SEQUENCE instead of PLAN.
        is_grounded: Whether the instance is grounded.

    Returns:
        Tuple of (INIT, PLAN, GOAL) text strings.
    """
    INIT = get_state_translation(initial_state, data)
    PLAN = ""
    plan_text = "\n"
    for i in plan:
        action = get_action_text(i, data)
        plan_text += action + "\n"
    if not action_seq:
        plan_text += "[PLAN END]\n"
    else:
        plan_text += "[ACTION SEQUENCE END]\n"
    PLAN += plan_text

    GOAL = get_state_translation(goal_state, data)

    return INIT, PLAN, GOAL

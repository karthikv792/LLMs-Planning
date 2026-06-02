"""Chain-of-thought plan generation and plan execution formatting."""

from .state import get_action_text, get_state_translation, parsed_instance_to_text_blocksworld


def generate_plan_cot(planexecutor, data, give_response):
    """Generate a chain-of-thought plan explanation.

    Produces step-by-step plan text showing current state, action,
    preconditions, and resulting state for each step.

    Args:
        planexecutor: Executor instance with plan and state info.
        data: Domain config dict.
        give_response: If True, include the full CoT trace.

    Returns:
        Tuple of (full_text, plan_text).
    """
    initial_state = planexecutor.init_state
    goal_state = planexecutor.goal_state
    resulting_state = planexecutor.final_state
    DATA = data
    INIT = get_state_translation(initial_state, DATA)
    PLAN = "[PLAN]"
    if give_response:
        plan_text = ""
        start, end = 0, 0
        steps = 1
        state = initial_state
        for index, i in enumerate(planexecutor.plan):
            start = end
            end = index + 1
            plan_text += f"\n{steps}. Current State: {get_state_translation(state, DATA)}\n"
            state = planexecutor.get_final_state(state, planexecutor.plan[start:end])
            preconds = planexecutor.get_action_preconditions(i.upper())
            precondition_text = get_state_translation(preconds, DATA)
            action = get_action_text(i, DATA)
            plan_text += f"   Action: {action}\n"
            plan_text += f"   Reason: The above action is applicable in the current state because its preconditions; {precondition_text}, are satisfied in the current state.\n"

            plan_text += f"   Resulting State: {get_state_translation(state, DATA)}\n"
            steps += 1
        plan_text += f"\nFinal State: {get_state_translation(state, DATA)}\n"
        plan_text += "The goal conditions are satisfied in the final state. Hence, the above plan is valid.\n[PLAN END]\n"
        PLAN += plan_text
    else:
        plan_text = "\n"
        for i in planexecutor.plan[:planexecutor.prefix]:
            action = get_action_text(i, DATA)
            plan_text += action
            plan_text += "\n"

    GOAL = get_state_translation(goal_state, DATA)

    text = f"\n[STATEMENT]\nAs initial conditions I have that, {INIT.strip()}\nMy goal is to have that {GOAL}.\nMy plan is as follows:\n\n{PLAN}"
    return text, plan_text


def plan_execution(planexecutor, data, give_response):
    """Format an executed action sequence with resulting state.

    Args:
        planexecutor: Executor instance.
        data: Domain config dict.
        give_response: If True, include the resulting state.

    Returns:
        Tuple of (text, resulting_state_list).
    """
    initial_state = planexecutor.init_state
    planexecutor.random_prefix_execution()
    plan_prefix = planexecutor.plan[:planexecutor.prefix]
    resulting_state = planexecutor.final_state

    INIT, PLAN, GOAL = parsed_instance_to_text_blocksworld(initial_state, plan_prefix, [], data, action_seq=True)
    if give_response:
        FIN = f'[RESULTING STATE]\n{get_state_translation(resulting_state, data)}\n'
        text = f"\n[STATEMENT]\nAs initial conditions I have that, {INIT.strip()}.\n I have executed the following action sequence:\n\n[ACTION SEQUENCE]{PLAN}{FIN}"
    else:
        FIN = f'[RESULTING STATE]\n'
        text = f"\n[STATEMENT]\nAs initial conditions I have that, {INIT.strip()}.\n I have executed the following action sequence:\n\n[ACTION SEQUENCE]{PLAN}{FIN}"

    return text, list(resulting_state)

"""Optimal planning task utilities."""

from .state import parsed_instance_to_text_blocksworld


def optimality(planexecutor, data, give_response=True):
    """Generate an optimality prompt showing plan with execution cost.

    Args:
        planexecutor: Executor instance with plan and cost.
        data: Domain config dict.
        give_response: If True, include the plan and cost in response.

    Returns:
        Tuple of (text, plan_with_cost).
    """
    initial_state = planexecutor.init_state
    goal_state = planexecutor.goal_state
    plan = planexecutor.plan
    cost = planexecutor.cost
    COST = ""
    INIT, PLAN, GOAL = parsed_instance_to_text_blocksworld(initial_state, plan, goal_state, data, is_grounded=planexecutor.is_pr_grounded)
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

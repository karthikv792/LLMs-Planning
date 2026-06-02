"""Replanning and plan subset generation utilities."""

import random

from .state import get_action_text, get_state_translation, parsed_instance_to_text_blocksworld


def generate_plan_subset(planexecutor, data, give_response):
    """Generate a plan subset prompt for plan reuse tasks.

    Args:
        planexecutor: Executor instance.
        data: Domain config dict.
        give_response: If True, include the full plan.

    Returns:
        Tuple of (text, plan_or_prefix).
    """
    initial_state = planexecutor.init_state
    planexecutor.random_prefix_execution()
    goal_state = planexecutor.goal_state
    resulting_state = planexecutor.final_state

    if give_response:
        INIT, PLAN, GOAL = parsed_instance_to_text_blocksworld(initial_state, planexecutor.plan, goal_state, data, is_grounded=planexecutor.is_pr_grounded)
        text = f"\n[STATEMENT]\nAs initial conditions I have that, {INIT.strip()}.\nMy goal is to have that {GOAL}.\nMy plan is as follows:\n\n[PLAN]{PLAN} "
        return text, PLAN
    else:
        INIT, _, GOAL = parsed_instance_to_text_blocksworld(initial_state,
                                                            planexecutor.plan[:planexecutor.prefix],
                                                            resulting_state, data, is_grounded=planexecutor.is_pr_grounded)
        PLAN_PREFIX = planexecutor.plan[:planexecutor.prefix]
        text = f"\n[STATEMENT]\nAs initial conditions I have that, {INIT.strip()}.\nMy goal is to have that {GOAL}.\nMy plan is as follows:\n\n[PLAN]"
        return text, PLAN_PREFIX


def replanning(planexecutor, data, give_response, is_harder=random.choice(([0, 1]))):
    """Generate a replanning prompt after unexpected state changes.

    Args:
        planexecutor: Executor instance.
        data: Domain config dict.
        give_response: If True, include the new plan.
        is_harder: Whether the replanning makes the problem harder (1) or easier (0).

    Returns:
        Tuple of (text, plan, new_model).
    """
    if is_harder:
        hard = "Problem was made harder\n"
    else:
        hard = "Problem was made easier\n"

    initial_state = planexecutor.init_state
    goal_state = planexecutor.goal_state
    to_add_or_remove = planexecutor.replanning_domain_specific(is_harder, domain=data['domain_name'])
    final_action = planexecutor.plan[:planexecutor.prefix][-1]
    new_model = planexecutor.get_new_instance(change_goal=False, change_init=True)
    plan, cost = planexecutor.get_plan('pr-new-domain.pddl', 'pr-new-problem.pddl')
    replanning_state = planexecutor.replanning_init
    if is_harder:
        execution_text = f"During execution, an unexpected event has occurred.\nAfter executing the action \"{get_action_text(final_action, data)}\" in the plan, The following facts unexpectedly became false: {get_state_translation(to_add_or_remove, data)}"
    else:
        execution_text = f"During execution, an unexpected event has occurred.\nAfter executing the action \"{get_action_text(final_action, data)}\" at step {planexecutor.prefix} in the plan, the following facts unexpectedly became true: {get_state_translation(to_add_or_remove['to_add'], data)}\nThe following facts became unexpectedly false: {get_state_translation(to_add_or_remove['to_remove'], data)}"
    INIT, PLAN, GOAL = parsed_instance_to_text_blocksworld(initial_state, planexecutor.plan, goal_state, data, is_grounded=planexecutor.is_pr_grounded)
    text = f"\n[STATEMENT]\nAs initial conditions I have that, {INIT.strip()}.\nMy goal is to have that {GOAL}.\nMy plan is as follows:\n\n[PLAN]{PLAN}\n"
    text += execution_text
    INIT, PLAN, GOAL = parsed_instance_to_text_blocksworld(replanning_state, plan, goal_state, data, is_grounded=planexecutor.is_pr_grounded)
    if give_response:
        text += f"\nAfter re-planning from the new state, the plan is as follows:\n[PLAN]{PLAN}"
    else:
        text += f"\nAfter re-planning from the new state, the plan is as follows:\n[PLAN]"
    return text, plan, new_model

"""Full plan validator that reports detailed error feedback.

Validates PDDL plans action-by-action, tracking unmet preconditions
and unsatisfied goal conditions.
"""
from planbench.model_parser.constants import *
from planbench.model_parser.parser import parse_model
from planbench.model_parser.writer import ModelWriter
from planbench.executor.executor import Executor


def get_all_errors(domain_file, problem_file, plan_file):
    """Validate a plan and return detailed error feedback.

    Args:
        domain_file: Path to the PDDL domain file.
        problem_file: Path to the PDDL problem file.
        plan_file: Path to the plan file (one action per line).

    Returns:
        Dictionary with 'validation_info' (is_valid_plan bool) and
        'unmet_info' (unmet_precond list, unmet_goal list).
    """
    ground = False # Keep this as the default
    executor = Executor(domain_file, problem_file, ground=ground)
    open_conditions = []
    with open(plan_file, 'r') as f:
        plan = f.readlines()
    plan = [line.strip() for line in plan]
    curr_state = executor.init_state
    for action in plan:
        action = action.replace('(', '').replace(')', '').replace(" ", "_")
        # BUG FIX: Original code called executor.is_action_list_upper() which
        # doesn't exist. Use the is_upper attribute directly instead.
        if executor.is_upper:
            action = action.upper()
        act_open_conditions = set()
        preconds, add_effects, del_effects = executor.ground_strips_action(action)
        # Find the preconds that are not satisfied
        act_open_conditions = set([precond for precond in preconds if precond not in curr_state])
        # Add the add effects to the current state
        curr_state = curr_state.union(add_effects)
        # Remove the delete effects from the current state
        curr_state = curr_state.difference(del_effects)
        open_conditions.append(list(act_open_conditions))

    unsatisfied_goal = executor.goal_state.difference(curr_state)

    precons_unsatisfied = any(len(open_conds) > 0 for open_conds in open_conditions)
    goal_unsatisfied = len(unsatisfied_goal) > 0

    feedback_dict = {
        'validation_info':
            {
                'is_valid_plan': not (precons_unsatisfied or goal_unsatisfied),
            },
        'unmet_info':
            {
                'unmet_precond': list(zip(plan, open_conditions)) if precons_unsatisfied else {} ,
                'unmet_goal': list(unsatisfied_goal) if goal_unsatisfied else []
             }
    }
    return feedback_dict

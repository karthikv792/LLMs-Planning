"""Goal reformulation task utilities."""

import random

from .state import get_state_translation


def paraphrase_goal(exec, data):
    """Generate a paraphrased goal with full state translation.

    Executes the complete plan and returns the goal state length
    and full state translation (for goal shuffling tasks).

    Args:
        exec: Executor instance.
        data: Domain config dict.

    Returns:
        Tuple of (goal_state_length, full_state_translation).
    """
    exec.complete_plan_execution()
    goal_state, full_state = list(exec.goal_state), list(exec.final_state)
    random.shuffle(goal_state)
    return len(goal_state), get_state_translation(full_state, data)

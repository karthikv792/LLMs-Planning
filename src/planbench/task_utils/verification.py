"""Plan verification utilities.

Includes full plan verification with VAL tool feedback,
zero-shot verification, and feedback formatting.
"""

import os
import random

from .state import get_action_text, get_state_translation, parsed_instance_to_text_blocksworld


def plan_verification_zero_shot(planexecutor, data, llm_plan=None):
    """Generate a zero-shot plan verification prompt.

    Args:
        planexecutor: Executor instance.
        data: Domain config dict.
        llm_plan: Optional pre-existing plan to verify.

    Returns:
        Verification prompt text.
    """
    if llm_plan is None:
        example_type = random.choice([-1, 0, 1])
        plan, cost = planexecutor.get_plan(planexecutor.pr_domain, planexecutor.pr_problem)

        if example_type == -1: #Inexecutable
            if len(plan)>2:
                to_del = random.choice(range(1, len(plan)-1))
            else:
                to_del = 1
            plan = plan[:to_del]+plan[to_del+1:]
            random.shuffle(plan)
        elif example_type==0: #Unsatisfied goal
            prefix= random.choice(range(0, len(plan)-1))
            plan = plan[:prefix]
        else:
            pass
    else:
        plan = llm_plan
        plan = [action.replace('(', '').replace(')', '') for action in plan]
        plan = ["_".join(action.split(' ')) for action in plan]

    initial_state = planexecutor.init_state
    goal_state = planexecutor.goal_state
    INIT, PLAN, GOAL = parsed_instance_to_text_blocksworld(initial_state, plan, goal_state, data)
    text = f'\n[STATEMENT]\nAs initial conditions I have that, {INIT.strip()}.\nMy goal is to have that {GOAL}. \nMy plan is as follows:\n\n[PLAN]{PLAN}\nVerify whether the above plan is valid. If it is valid, please say "Plan is valid." and nothing else. If it is invalid, please say "Plan is invalid." and then provide feedback on why the plan fails.'
    return text


def plan_verification_zero_shot_val_form(planexecutor, data, llm_plan=None):
    """Generate a zero-shot plan verification prompt with VAL-format feedback instructions.

    Args:
        planexecutor: Executor instance.
        data: Domain config dict.
        llm_plan: Optional pre-existing plan to verify.

    Returns:
        Verification prompt text with structured feedback format.
    """
    if llm_plan is None:
        example_type = random.choice([-1, 0, 1])
        plan, cost = planexecutor.get_plan(planexecutor.pr_domain, planexecutor.pr_problem)

        if example_type == -1: #Inexecutable
            if len(plan)>2:
                to_del = random.choice(range(1, len(plan)-1))
            else:
                to_del = 1
            plan = plan[:to_del]+plan[to_del+1:]
            random.shuffle(plan)
        elif example_type==0: #Unsatisfied goal
            prefix= random.choice(range(0, len(plan)-1))
            plan = plan[:prefix]
        else:
            pass
    else:
        plan = llm_plan
        plan = [action.replace('(', '').replace(')', '') for action in plan]
        plan = ["_".join(action.split(' ')) for action in plan]

    initial_state = planexecutor.init_state
    goal_state = planexecutor.goal_state
    INIT, PLAN, GOAL = parsed_instance_to_text_blocksworld(initial_state, plan, goal_state, data)
    text = f'\n[STATEMENT]\nAs initial conditions I have that, {INIT.strip()}.\nMy goal is to have that {GOAL}. \nMy plan is as follows:\n\n[PLAN]{PLAN}\nVerify whether the above plan is valid. If it is valid, please say "Plan is valid." and nothing else. If it is invalid, please say "Plan is invalid." and then provide feedback on why the plan fails according to the following format. If the plan is inexecutable, provide the first action that is inexecutable and the unmet preconditions in the following format: The following action [action name] has unmet preconditions [list of preconditions]. If the plan is executable but does not satisfy the goal, provide the unmet goal conditions.'
    return text


def plan_verification(planexecutor, data, run_val, give_response=False, example_type=None, llm_plan=None):
    """Generate a plan verification prompt with optional VAL validation.

    Args:
        planexecutor: Executor instance.
        data: Domain config dict.
        run_val: Whether to run the VAL validator.
        give_response: Whether to include the validation result in the text.
        example_type: Type of example (-1=inexecutable, 0=unsatisfied goal, 1=valid).
        llm_plan: Optional pre-existing plan to verify.

    Returns:
        Tuple of (text, val_message or None).
    """
    if llm_plan is None:
        plan, cost = planexecutor.get_plan(planexecutor.pr_domain, planexecutor.pr_problem)

        if example_type == -1: #Inexecutable
            if len(plan)>2:
                to_del = random.choice(range(1, len(plan)-1))
            else:
                to_del = 1
            plan = plan[:to_del]+plan[to_del+1:]
            random.shuffle(plan)
        elif example_type==0: #Unsatisfied goal
            prefix= random.choice(range(0, len(plan)-1))
            plan = plan[:prefix]
        else:
            pass
        if example_type == 1:
            val_message = "The above plan is valid.\n"
        else:
            with open('sas_plan_ver', 'w') as f:
                if planexecutor.is_pr_grounded:
                    for action in plan:
                        f.write(f'({action})\n')
                else:
                    for action in plan:
                        f.write(f'({" ".join(action.split("_"))})\n')
            if run_val:
                domain = planexecutor.pr_domain
                problem = planexecutor.pr_problem
                val_feedback_dict = get_val_feedback(domain, problem, 'sas_plan_ver')
                val_message = get_validation_message(val_feedback_dict, data)
    else:
        plan = llm_plan
        with open('sas_plan_ver', 'w') as f:
            for action in plan:
                if planexecutor.is_pr_grounded:
                    action = "_".join(action.split(' '))
                f.write(f'{action}\n')
        if run_val:
            domain = planexecutor.pr_domain
            problem = planexecutor.pr_problem
            val_feedback_dict = get_val_feedback(domain, problem, 'sas_plan_ver')
            val_message = get_validation_message(val_feedback_dict, data)
        plan = [action.replace('(', '').replace(')', '') for action in plan]
        plan = ["_".join(action.split(' ')) for action in plan]
    initial_state = planexecutor.init_state
    goal_state = planexecutor.goal_state
    INIT, PLAN, GOAL = parsed_instance_to_text_blocksworld(initial_state, plan, goal_state, data)
    if run_val and give_response:
        text = f"\n[STATEMENT]\nAs initial conditions I have that, {INIT.strip()}.\nMy goal is to have that {GOAL}. \nMy plan is as follows:\n\n[PLAN]{PLAN}\n[VERIFICATION]\n{val_message}"
    else:
        text = f"\n[STATEMENT]\nAs initial conditions I have that, {INIT.strip()}.\nMy goal is to have that {GOAL}. \nMy plan is as follows:\n\n[PLAN]{PLAN}\n[VERIFICATION]"
    return text, val_message if run_val else None


def reformat_feedback(feedback):
    """Parse VAL feedback into structured unmet preconditions and goals.

    Args:
        feedback: List of feedback lines from VAL.

    Returns:
        Tuple of (unmet_precond_lines, unmet_goal_lines).
    """
    unmet_precond = []
    unmet_goal = []
    precond = False
    goal = False
    for line in feedback:
        line = line.strip()
        if 'unsatisfied precondition' in line:
            precond = True
            goal = False
            unmet_precond.append(f'Time step: {line.split(" ")[-1]}\nAction: {line[line.find("("):line.find(")")+1]}\nUnsatisfied Precondition:\n')
            continue
        elif 'goal is not satisfied' in line:
            goal = True
            precond = False
            unmet_goal.append(f'There are unmet goal condition(s):\n')
            continue
        if precond and line:
            is_false = True if 'false' in line else False

            if 'Follow each of:' in line:
                line = line.replace('Follow each of:', 'and')
            elif 'Follow one of:' in line:
                line = line.replace('Follow one of:', 'or')
            if 'and (Set' in line:
                line = line.replace("and (Set ", '').replace(' to true)', '').replace(' to false)', '')
            elif '(Set' in line:
                line = line.replace("(Set ", '').replace(' to true)', '').replace(' to false)', '')
            if is_false:
                line = f'(not {line})'
            unmet_precond.append(line)
        elif goal and line:
            if 'Follow each of:' in line:
                line = line.replace('Follow each of:', 'and')
            elif 'Follow one of:' in line:
                line = line.replace('Follow one of:', 'or')
            if 'and (Set' in line:
                line = line.replace("and (Set ", '').replace(' to true)', '').replace(' to false)', '')
            elif '(Set' in line:
                line = line.replace("(Set ", '').replace(' to true)', '').replace(' to false)', '')
            unmet_goal.append(line)
    return unmet_precond, unmet_goal


def get_val_feedback(domain_file, instance_file, plan_file):
    """Run VAL validator and extract structured feedback.

    Args:
        domain_file: Path to domain PDDL file.
        instance_file: Path to problem PDDL file.
        plan_file: Path to plan file.

    Returns:
        Dictionary with validation_info, validation_message, and unmet_info.
    """
    val = os.environ.get('VAL')
    cmd = f'{val}/validate -v {domain_file} {instance_file} {plan_file}'
    response = os.popen(cmd).read()
    plan_valid = 'Plan valid' in response
    feedback = []
    repair = False
    for line in response.split('\n'):
        if 'Plan Repair' in line:
            repair = True
            continue
        if 'Failed plans' in line:
            repair = False
        if repair and line:
            feedback.append(line)
    unmet_precond, unmet_goal = reformat_feedback(feedback)
    feedback_dict = {
        'validation_info': {'is_valid_plan': plan_valid},
        'validation_message': '\n'.join(unmet_goal) if unmet_goal else '\n'.join(unmet_precond),
        'unmet_info': {'unmet_precond': unmet_precond, 'unmet_goal': unmet_goal}
    }
    return feedback_dict


def get_validation_message(val_message, data):
    """Format VAL validation results into human-readable error message.

    Args:
        val_message: Dictionary from get_val_feedback().
        data: Domain config dict.

    Returns:
        Formatted error message string, or None if plan is valid.
    """
    unmet_precond, unmet_goal = val_message['unmet_info']['unmet_precond'], val_message['unmet_info']['unmet_goal']

    error_message = "The above plan is invalid."

    if unmet_goal:
        is_joint = "and" in unmet_goal[1]
        first_predicate = 2 if is_joint else 1
        last_predicate = len(unmet_goal) - 1 if is_joint else 2
        predicates = unmet_goal[first_predicate:last_predicate]
        error_message += " These are the unmet goal conditions:\n" if len(predicates) > 1 else \
            " This is the unmet goal condition:\n"
    elif unmet_precond:
        timestep = unmet_precond[0].split("\n")[0].split(" ")[-1]
        action = unmet_precond[0].split("(")[1].split(")")[0].replace(" ", "_")
        is_joint = "(and" in unmet_precond[1]
        first_predicate = 2 if is_joint else 1
        last_predicate = len(unmet_precond) - 1 if is_joint else 2
        predicates = unmet_precond[first_predicate:last_predicate]
        error_message += f" The following action at step {timestep} has unmet preconditions:\n" \
            if len(predicates) > 1 else \
            f"The following action at step {timestep} has an unmet precondition:\n"
        error_message += get_action_text(action, data) + "\n"
        error_message += "The unmet preconditions are:\n" \
            if len(predicates) > 1 else \
            "The unmet precondition is:\n"

    else:
        return None
    error_message += get_state_translation(map(lambda pddl: pddl.strip("()").replace(" ", "_"), predicates), data)

    return error_message

"""Plan validation using the VAL tool.

Provides functions to validate PDDL plans and check for unsolvable instances.
"""

import os


def validate_plan(domain, instance, plan_file):
    """Validate a PDDL plan using the VAL tool.

    Args:
        domain: Path to the domain PDDL file.
        instance: Path to the problem instance PDDL file.
        plan_file: Path to the plan file to validate.

    Returns:
        True if plan is valid, False otherwise.

    Raises:
        Exception: If there's a problem with the domain file.
    """
    val_path = os.getenv("VAL")
    cmd = f"{val_path}/validate {domain} {instance} {plan_file}"
    response = os.popen(cmd).read()
    if 'Problem in domain' in response:
        raise Exception('Problem in domain: Check PDDL Writer')
    return True if "Plan valid" in response else False


def validate_plan_unsolvable(llm_plan):
    """Check if the LLM response indicates no plan is possible.

    Args:
        llm_plan: The LLM's response text.

    Returns:
        True if the response indicates the problem is unsolvable.
    """
    if "no plan possible" in llm_plan:
        return True
    return False

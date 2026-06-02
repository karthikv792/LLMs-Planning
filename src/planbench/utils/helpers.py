"""Generic utility functions for PlanBench.

Contains helpers that don't belong to any specific module:
caesar cipher, text formatting, cost extraction.
No side effects at import time.
"""


def treat_on(letters_dict, atom):
    """Format an 'on' predicate atom for display."""
    terms = atom.subterms
    return f"the {letters_dict[terms[0].name]} block on top of the {letters_dict[terms[1].name]} block"


def get_cost_gpt_3(gpt3_response):
    """Extract execution cost from a GPT-3 response.

    Looks for 'time to execute' in the response and extracts
    the first integer found after that line.

    Args:
        gpt3_response: The text response from GPT-3.

    Returns:
        The extracted cost as an integer, or 0 if not found.
    """
    lines = [line.strip() for line in gpt3_response.split("\n")]
    flag = True
    for i in range(len(lines)):
        if 'time to execute' in lines[i]:
            flag = False
        if flag:
            continue
        res = [int(i) for i in lines[i].split() if i.isdigit()]
        if len(res) > 0:
            return res[0]
    return 0


def caesar_encode(query):
    """Encode text using a Caesar cipher with shift of 5."""
    key = 5
    alpha = ['ABCDEFGHIJKLMNOPQRSTUVWXYZ', 'abcdefghijklmnopqrstuvwxyz']
    new_query = ''
    for i in query:
        if i in alpha[0]:
            new_letter = (alpha[0].find(i) + key) % 26
            new_query += alpha[0][new_letter]
        elif i in alpha[1]:
            new_letter = (alpha[1].find(i) + key) % 26
            new_query += alpha[1][new_letter]
        else:
            new_query += i
    return new_query


def save_gpt3_response(response, file):
    """Extract PDDL actions from an LLM response and save to a plan file.

    Filters for lines starting with ``(`` and ignores empty actions
    and "no actions" lines.

    Args:
        response: Raw text response from the LLM.
        file: Path to write the extracted plan to.

    Returns:
        The extracted plan as a newline-joined string.
    """
    plan = []
    for line in response.split('\n'):
        if not line.startswith('('):
            continue
        if '()' in line:
            continue
        if 'no actions' in line:
            continue
        plan.append(line.lower().strip())
    with open(file, 'w') as f:
        f.write('\n'.join(plan))
    return '\n'.join(plan)


def caesar_decode(gpt3_resp):
    """Decode text using a Caesar cipher with shift of 5."""
    key = 5
    alpha = ['ABCDEFGHIJKLMNOPQRSTUVWXYZ', 'abcdefghijklmnopqrstuvwxyz']
    new_query = ''
    for i in gpt3_resp:
        if i in alpha[0]:
            new_letter = (alpha[0].find(i) - key) % 26
            new_query += alpha[0][new_letter]
        elif i in alpha[1]:
            new_letter = (alpha[1].find(i) - key) % 26
            new_query += alpha[1][new_letter]
        else:
            new_query += i
    return new_query

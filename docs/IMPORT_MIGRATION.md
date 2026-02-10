# Import Migration Spec

Maps every symbol currently exported by `from utils import *` (via `llm_planning_analysis/utils/__init__.py`) to its future module path under `src/planbench/`.

## Current Export Chain

```
utils/__init__.py
  -> from .llm_utils import *
  -> from .pddl_to_text import *
  -> from .text_to_pddl import *
  -> from .task_utils import *
  + direct definitions (validate_plan, caesar_encode, BWGenerator, etc.)
```

## Symbol Map

### From `utils/llm_utils.py` -> `src/planbench/llm/`

| Symbol | Type | Future Module |
|--------|------|---------------|
| `generate_from_bloom` | function | `src/planbench/llm/bloom.py` |
| `connect_google` | function | `src/planbench/llm/google.py` |
| `send_query` | function | `src/planbench/llm/query.py` |
| `send_query_multiple_for_loop` | function | `src/planbench/llm/query.py` |
| `send_query_multiple` | function | `src/planbench/llm/query.py` |
| `send_query_with_feedback` | function | `src/planbench/llm/query.py` |
| `llama_messages_to_single_prompt` | function | `src/planbench/llm/aws.py` |
| `save_gpt3_response` | function | `src/planbench/llm/query.py` |

### From `utils/pddl_to_text.py` -> `src/planbench/converters/pddl_to_text.py`

| Symbol | Type | Future Module |
|--------|------|---------------|
| `get_sorted` | function | `src/planbench/converters/pddl_to_text.py` |
| `parse_problem` | function | `src/planbench/converters/pddl_to_text.py` |
| `fill_template` | function | `src/planbench/converters/pddl_to_text.py` |
| `instance_to_text` | function | `src/planbench/converters/pddl_to_text.py` |
| `get_plan_as_text` | function | `src/planbench/converters/pddl_to_text.py` |

### From `utils/text_to_pddl.py` -> `src/planbench/converters/text_to_pddl.py`

| Symbol | Type | Future Module |
|--------|------|---------------|
| `get_ordered_objects` | function | `src/planbench/converters/text_to_pddl.py` |
| `text_to_plan` | function (dispatcher) | `src/planbench/converters/text_to_pddl.py` |
| `text_to_plan_with_llm` | function | `src/planbench/converters/text_to_pddl.py` |
| `has_digit` | function (helper) | `src/planbench/converters/text_to_pddl.py` |
| `text_to_plan_logistics` | function | `src/planbench/converters/text_to_pddl.py` |
| `text_to_plan_obfuscated` | function | `src/planbench/converters/text_to_pddl.py` |
| `text_to_plan_blocksworld` | function | `src/planbench/converters/text_to_pddl.py` |
| `text_to_plan_depots` | function | `src/planbench/converters/text_to_pddl.py` |
| `text_to_state` | function (dispatcher) | `src/planbench/converters/text_to_pddl.py` |
| `text_to_state_obfuscated` | function | `src/planbench/converters/text_to_pddl.py` |
| `text_to_state_mystery` | function | `src/planbench/converters/text_to_pddl.py` |
| `text_to_state_blocksworld` | function | `src/planbench/converters/text_to_pddl.py` |
| `text_to_state_logistics` | function | `src/planbench/converters/text_to_pddl.py` |
| `text_to_state_depots` | function | `src/planbench/converters/text_to_pddl.py` |

### From `utils/task_utils.py` -> `src/planbench/converters/task_utils.py`

| Symbol | Type | Future Module |
|--------|------|---------------|
| `get_action_text` | function | `src/planbench/converters/task_utils.py` |
| `get_state_translation` | function | `src/planbench/converters/task_utils.py` |
| `paraphrase_goal` | function | `src/planbench/converters/task_utils.py` |
| `generate_plan_cot` | function | `src/planbench/tasks/plan_generation.py` |
| `parsed_instance_to_text_blocksworld` | function | `src/planbench/converters/task_utils.py` |
| `generate_plan_subset` | function | `src/planbench/tasks/plan_generation.py` |
| `optimality` | function | `src/planbench/tasks/optimality.py` |
| `replanning` | function | `src/planbench/tasks/replanning.py` |
| `plan_execution` | function | `src/planbench/tasks/execution.py` |
| `plan_verification_zero_shot` | function | `src/planbench/tasks/verification.py` |
| `plan_verification_zero_shot_val_form` | function | `src/planbench/tasks/verification.py` |
| `plan_verification` | function | `src/planbench/tasks/verification.py` |
| `reformat_feedback` | function | `src/planbench/tasks/verification.py` |
| `get_val_feedback` | function | `src/planbench/tasks/verification.py` |
| `get_validation_message` | function | `src/planbench/tasks/verification.py` |

### From `utils/__init__.py` (direct definitions) -> various

| Symbol | Type | Future Module |
|--------|------|---------------|
| `LogisticsGenerator` | class | `src/planbench/generators/logistics.py` |
| `BWGenerator` | class | `src/planbench/generators/blocksworld.py` |
| `treat_on` | function | `src/planbench/converters/pddl_to_text.py` |
| `validate_plan_unsolvable` | function | `src/planbench/evaluation/validation.py` |
| `validate_plan` | function | `src/planbench/evaluation/validation.py` |
| `get_cost_gpt_3` | function | `src/planbench/evaluation/validation.py` |
| `caesar_encode` | function | `src/planbench/converters/caesar.py` |
| `caesar_decode` | function | `src/planbench/converters/caesar.py` |

## Migration Notes

- All `from utils import *` statements will be replaced with explicit imports from `src/planbench/` submodules
- The `numpy` (`np`) import is re-exported from `utils/__init__.py` and used directly by consumers (e.g., `response_evaluation.py:15`)
- The `openai`, `random`, `os`, `hashlib` imports in `__init__.py` are also re-exported; consumers should import these directly
- The `PDDLReader` and `tarski` imports are used by `__init__.py` for `BWGenerator.instance_ok()`; these stay with the generator classes

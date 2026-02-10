# Design: Merge `plan-bench/` and `llm_planning_analysis/` into `planbench`

**Date:** 2026-02-09
**Status:** Validated against codebase audit
**Precedence rule:** `llm_planning_analysis` is the actively developed fork and takes priority for all diverged files.

---

## 1. Context

`plan-bench/` and `llm_planning_analysis/` are forked copies of the same codebase that diverged over time. Neither is installable, both use hardcoded relative paths, wildcard imports, and have side effects at import time. This design merges them into a single clean Python package.

### What each fork contributes

| Aspect | `plan-bench` | `llm_planning_analysis` |
|--------|-------------|------------------------|
| Tasks | 12 task methods (t1-t8_3 + t3_1 + t3_zero_shot) | 5 task methods (t1 variants: zero-shot, CoT, PDDL) |
| LLM support | Old OpenAI API + BLOOM | OpenAI, Gemini, Claude, Bedrock, Groq, DeepSeek, QwQ, Qwen, BLOOM |
| Domains | blocksworld, logistics, mystery, depots, obfuscated | blocksworld, logistics, mystery, sokoban, obfuscated, unsolvable, blocksworld_hard |
| Parallelism | Sequential only | ThreadPoolExecutor in response gen, evaluation, and backprompting |
| Evaluation | evaluate_plan, evaluate_state, evaluate_verification | evaluate_plan, evaluate_plan_pddl, evaluate_plan_parallel, extract_plans |
| Extras | -- | backprompting (sequential + parallel), stats, full_validator |
| Instances | ~4,430 PDDL files (includes depots) | ~2,440 PDDL files (includes sokoban, unsolvable) |
| Configs | 10 YAML | 15 YAML (includes state_tracking intros, compact objects) |

### Modules confirmed identical (safe to take either)

- `model_parser/__init__.py`, `constants.py`, `parser_new.py`, `writer_new.py` (all 4 files)
- `problem_generators.py` (516 lines each)
- `obfuscate/random_words_1.txt`

### Modules with significant divergence (require true merge)

| Module | Risk | Key differences |
|--------|------|----------------|
| `Executor/__init__.py` | HIGH | plan-bench has `len(plan)<2` safety check + depots replanning; llm_planning_analysis has better error reporting in `ground_strips_action()` |
| `utils/llm_utils.py` | CRITICAL | plan-bench: 80 lines, deprecated API. llm_planning_analysis: 625 lines, 10 providers, broken groq_client, duplicate imports, side effects at import |
| `utils/text_to_pddl.py` | CRITICAL | llm_planning_analysis adds `text_to_plan_with_llm()` (295 lines). plan-bench has depots support |
| `utils/task_utils.py` | HIGH | plan-bench has depots branches in domain-specific functions. llm_planning_analysis has `validate_plan_unsolvable()` |
| `obfuscator.py` | MEDIUM | plan-bench 17KB, llm_planning_analysis 22KB (larger, more complete) |
| `utils/pddl_to_text.py` | LOW | llm_planning_analysis has better error reporting; plan-bench has depots handling |
| `utils/__init__.py` | LOW | llm_planning_analysis adds `validate_plan_unsolvable()` |

---

## 2. Bugs to Fix Before Restructuring

These exist in the current `llm_planning_analysis` codebase and must be fixed in Phase 0:

1. **`groq_client` undefined** (`llm_utils.py:211`): `groq_client.chat.completions.create()` is called but `groq_client` is never imported or instantiated. Runtime NameError when `_groq` engine is used.

2. **Duplicate import blocks** (`llm_utils.py:18-20` duplicated at `25-27`): boto3 Config and client created twice at module level.

3. **Duplicate function** (`llm_utils.py`): `llama_messages_to_single_prompt()` defined twice. Second definition shadows the first.

4. **Side effects at import time** (`llm_utils.py`): `OpenAI()` client and `boto3.client('bedrock-runtime')` created at module level. Importing `utils` fails without AWS credentials even for non-AWS tasks.

5. **Side effects at import time** (`utils/__init__.py`): `openai.api_key = os.environ["OPENAI_API_KEY"]` and `random.seed(10)` run on import.

6. **Missing requirements**: `anthropic`, `google-genai`, `boto3`, `tqdm`, `rich`, `groq` are used in code but not listed in `requirements.txt`. `numpy==1.19.5` is incompatible with Python 3.10+.

---

## 3. Canonical Task Registry

16 unique tasks across both codebases (t1 overlaps):

| ID | Name | Source |
|----|------|--------|
| t1 | Plan Generation (one-shot) | Both |
| t1_zero | Plan Generation (zero-shot) | llm_planning_analysis |
| t1_cot | Plan Generation (CoT / state tracking) | llm_planning_analysis |
| t1_pddl | Plan Generation PDDL (one-shot) | llm_planning_analysis |
| t1_zero_pddl | Plan Generation PDDL (zero-shot) | llm_planning_analysis |
| t2 | Optimal Planning | plan-bench |
| t3 | Plan Verification | plan-bench |
| t3_1 | Plan Verification with LLM Plans | plan-bench |
| t3_zero_shot | Zero-shot Plan Verification | plan-bench |
| t4 | Plan Reuse | plan-bench |
| t5 | Plan Generalization | plan-bench |
| t6 | Replanning | plan-bench |
| t7 | Reasoning about Plan Execution | plan-bench |
| t8_1 | Goal Reformulation (Goal shuffling) | plan-bench |
| t8_2 | Goal Reformulation (Full -> Partial) | plan-bench |
| t8_3 | Goal Reformulation (Partial -> Full) | plan-bench |

---

## 4. Target Directory Structure

```
LLMs-Planning/
  pyproject.toml
  README.md
  LICENSE
  .gitignore
  docs/
    plans/
  src/
    planbench/
      __init__.py
      config.py                     # DomainConfig dataclass (22 fields)
      pipeline.py                   # Orchestrates prompt -> response -> eval (all 16 tasks)
      cli.py                        # CLI entry points
      utils.py                      # caesar_encode/decode, treat_on, get_cost_gpt_3, misc
      prompts/
        __init__.py
        generator.py                # Unified PromptGenerator (all 16 tasks)
      llm/
        __init__.py                 # get_client(engine) factory
        base.py                     # LLMClient protocol, LLMResponse dataclass
        openai_client.py            # OpenAI chat/completion + DeepSeek-R1
        google_client.py            # Gemini + PaLM
        anthropic_client.py         # Claude direct
        aws_client.py               # Bedrock (Claude, LLaMA)
        groq_client.py              # Groq
        local_client.py             # BLOOM, QwQ, Qwen (transformers + vLLM)
      evaluation/
        __init__.py
        evaluator.py                # Merged ResponseEvaluator (all eval methods)
        validator.py                # validate_plan(), validate_plan_unsolvable(), VAL wrapper
      translation/
        __init__.py
        pddl_to_text.py             # Merged (llm_planning_analysis base + depots)
        text_to_pddl.py             # llm_planning_analysis version (superset)
      executor/
        __init__.py
        executor.py                 # Merged Executor (llm_planning_analysis base + plan-bench safety checks + depots)
        full_validator.py            # get_all_errors()
      generators/
        __init__.py
        base.py                     # Shared: instance_ok(), hash dedup, plan computation
        blocksworld.py
        logistics.py
        depots.py
        sokoban.py
      obfuscation/
        __init__.py
        obfuscator.py               # llm_planning_analysis version (22KB)
      analysis/
        __init__.py
        stats.py                    # Configurable stats generation
      backprompting/
        __init__.py                 # Exports SequentialBackPrompter, ParallelBackPrompter
        base.py                     # Shared: init, read_config, get_problem, get_executor, IO
        backprompter.py             # Sequential implementation
        backprompter_parallel.py    # Parallel implementation (depends on full_validator)
      response/
        __init__.py
        generator.py                # Merged ResponseGenerator (LLMClient + ThreadPoolExecutor)
      task_utils/
        __init__.py                 # Ordered re-exports (state first)
        state.py                    # BASE: get_action_text, get_state_translation, parsed_instance_to_text_blocksworld
        cot.py                      # generate_plan_cot, plan_execution
        verification.py             # plan_verification, plan_verification_zero_shot, VAL feedback functions
        replanning.py               # replanning, generate_plan_subset
        optimality.py               # optimality
        goal_reformulation.py       # paraphrase_goal
      model_parser/
        __init__.py
        constants.py
        parser.py                   # Renamed from parser_new.py
        writer.py                   # Renamed from writer_new.py
  data/
    configs/                        # Merged YAML configs (22 files)
    instances/                      # Merged PDDL instances (~5,063 files)
    obfuscate/                      # Word lists
    pddlgenerators/                 # C/Python generators (blocksworld, logistics, sokoban)
  planner_tools/                    # External planner binaries (untouched)
```

---

## 5. Key Design Decisions

### 5.1 DomainConfig Dataclass

Replaces scattered `yaml.safe_load()` calls. All 22 config keys found across 25 YAML files are covered:

```python
@dataclass
class DomainConfig:
    # Required
    domain_name: str
    domain_file: str
    instance_dir: str
    instances_template: str
    n_instances: int
    start: int
    end: int
    domain_intro: str
    actions: dict[str, str]
    predicates: dict[str, str]
    encoded_objects: dict[str, str]

    # Optional intro variants (used by specific tasks)
    domain_intro_state_tracking: str | None = None   # t1_cot
    domain_intro_zero_shot: str | None = None         # t1_zero
    domain_intro_cost: str | None = None              # t2 (optimality)

    # Optional paths
    generalized_instance_dir: str | None = None       # t5 (generalization)

    # Optional mappings
    predicate_mapping: dict[str, str] | None = None
    encoded_objects_compact: dict[str, str] | None = None

    # Optional obfuscation
    action_obfuscation: dict[str, str] | None = None
    predicate_obfuscation: dict[str, str] | None = None

    # Optional metadata
    objects: list[str] | None = None
    callbacks: list[str] | None = None

    @classmethod
    def from_yaml(cls, path: str | Path) -> "DomainConfig": ...

    def resolve_instance_path(self, instance_id: int) -> Path: ...
```

### 5.2 LLMClient Protocol

Replaces the 625-line `send_query()` function. Three methods cover all calling patterns found in the codebase:

```python
class LLMClient(Protocol):
    def query(self, prompt: str, max_tokens: int, **kwargs) -> LLMResponse: ...
    def query_with_history(self, messages: list[dict], max_tokens: int, **kwargs) -> LLMResponse: ...
    def query_multiple(self, prompt: str, n: int, max_tokens: int, **kwargs) -> list[LLMResponse]: ...

@dataclass
class LLMResponse:
    text: str
    raw_response: Any
    time_taken: float
    usage: dict | None = None                   # Must support nested reasoning_tokens for o1
    messages: list[dict] | None = None           # Updated history for multi-turn
    context_window_exceeded: bool = False         # Context limit flag
    null_response: bool = False                   # Empty/whitespace response flag
```

**Provider-specific requirements** (encapsulated per client, not leaked to callers):

| Provider | Quirks to encapsulate |
|----------|----------------------|
| OpenAI chat | o1 models: no temperature; reasoning token extraction |
| Gemini | 70s rate-limit sleep; thinking token separation; alpha API |
| Claude (direct) | System prompt; content array filtering |
| AWS Bedrock | Claude vs LLaMA different formats; LLaMA prompt tags |
| Groq | Temperature clamped to 0.0001; 2s sleep |
| Local models | GPU device mapping; 8-bit quantization; no usage tracking; DeepSeek-R1 uses vLLM not transformers |

**Factory**: `get_client(engine: str) -> LLMClient` dispatches to the right provider. All clients are lazy-initialized (no import-time side effects). Missing credentials raise on first `query()` call, not at import.

### 5.3 task_utils Split

The `task_utils/` subpackage has an acyclic dependency graph with `state.py` as the root:

```
state.py (BASE - no internal deps)
  get_action_text()
  get_state_translation()
  parsed_instance_to_text_blocksworld()     <-- shared by 6+ functions
    |
    +---> cot.py                            depends on state.py
    |       generate_plan_cot()
    |       plan_execution()
    |
    +---> goal_reformulation.py             depends on state.py
    |       paraphrase_goal()
    |
    +---> optimality.py                     depends on state.py
    |       optimality()
    |
    +---> replanning.py                     depends on state.py
    |       replanning()
    |       generate_plan_subset()
    |
    +---> verification.py                   depends on state.py
            plan_verification()
            plan_verification_zero_shot()
            plan_verification_zero_shot_val_form()
            reformat_feedback()
            get_val_feedback()
            get_validation_message()
```

Import order in `__init__.py`: `state` -> `goal_reformulation`, `cot` -> `optimality`, `replanning`, `verification`. No circular imports possible.

### 5.4 Backprompting: Separate Implementations

`back_prompting.py` and `back_prompting_parallel.py` are architecturally incompatible and cannot be merged into a single file:

| Aspect | Sequential | Parallel |
|--------|-----------|----------|
| Execution | Single loop over instances | ThreadPoolExecutor with worker functions |
| Output dir | `results/{domain}/{engine}/json/` | `results_backprompting/{domain}/{engine}/` |
| Plan files | Single `gpt_sas_plan` | Indexed `gpt_sas_plan_{}` |
| Verification | `get_repeated_verification()` | `get_repeated_verification_parallel()` with step_dict |
| PDDL tasks | None | `task_1_plan_generation_backprompting_zero_shot_pddl()` |
| full_validator | Not used | Required dependency |
| Unique methods | `is_already_correct()`, `self_critique()`, `compute_plan()` | `per_instance_output()`, `per_instance_output_pddl()`, `verification()`, `get_info_data()` |

Structure: 4-file package with shared base class and two separate implementations exported as `SequentialBackPrompter` and `ParallelBackPrompter`.

### 5.5 Compatibility Shim Strategy

To avoid breaking all consumers when splitting `utils/`:

1. **Phase 3**: Split utils into new modules. Keep `utils/__init__.py` as a re-export shim that imports from the new locations. All existing `from utils import *` consumers keep working unchanged.
2. **Phase 5**: Rewrite all consumers to use explicit imports from new module paths.
3. **Phase 9**: Remove the shim.

This means each phase is independently testable and the codebase is never in a broken state between phases.

### 5.6 Path Resolution

All path-dependent code uses `DomainConfig.resolve_instance_path()` instead of hardcoded `./instances/`. A `workspace_root` parameter defaults to `data/` within the project or a user-specified directory.

### 5.7 CLI Entry Points

```toml
[project.scripts]
planbench = "planbench.cli:main"
```

Subcommands: `planbench run`, `planbench prompt`, `planbench evaluate`, `planbench generate`, `planbench obfuscate`, `planbench stats`, `planbench backprompt`.

All `eval()` calls on user input replaced with proper `action="store_true"` argparse.

---

## 6. Data Migration

### Configs (22 files merged)

| Config | Source | Notes |
|--------|--------|-------|
| blocksworld.yaml | llm_planning_analysis | Has `domain_intro_state_tracking`, `encoded_objects_compact` |
| blocksworld_3.yaml | llm_planning_analysis | |
| blocksworld_hard.yaml | llm_planning_analysis | Unique |
| depots.yaml | plan-bench | Unique |
| logistics.yaml | llm_planning_analysis | Has `domain_intro_zero_shot` |
| mystery_blocksworld.yaml | llm_planning_analysis | |
| mystery_blocksworld_3.yaml | llm_planning_analysis | |
| obfuscated_deceptive_logistics.yaml | Either | |
| obfuscated_randomized_logistics.yaml | Either | |
| random_blocksworld.yaml | Either | |
| random_blocksworld_3.yaml | Either | |
| sokoban.yaml | llm_planning_analysis | Unique |
| unsolvable_blocksworld.yaml | llm_planning_analysis | Unique |
| unsolvable_obfuscated_randomized_blocksworld.yaml | llm_planning_analysis | Unique |

`config_mask.yaml` and `default_template.yaml` are development artifacts for obfuscation generation. Decision needed: keep in `data/configs/` or move to `data/templates/`.

### Instances (~5,063 PDDL files merged)

| Domain | Source | Count |
|--------|--------|-------|
| blocksworld (+ mystery, random variants) | plan-bench (larger set) | 2,209 |
| depots | plan-bench only | 502 |
| logistics (+ obfuscated variants) | Both sets | ~1,720 |
| sokoban | llm_planning_analysis only | 57 |
| blocksworld_hard | llm_planning_analysis only | 111 |
| unsolvable_blocksworld | llm_planning_analysis only | 102 |
| unsolvable_obfuscated_randomized_blocksworld | llm_planning_analysis only | 102 |

### Historical experiment data

`prompts/`, `responses/`, `results/`, `results_backprompting/`, `graphs/` contain experiment outputs. Strategy: gitignore going forward, do not delete existing data from history.

### Binary / scratch files to delete

`gpt_sas_plan`, `sas_plan`, `llm_plan`, `R1_reasoning_examples_2.txt`, `quick_stats.py`, `example_generator.py`, `promptgen.sh`.

---

## 7. Dependencies

```toml
[project]
name = "planbench"
version = "0.2.0"
requires-python = ">=3.10"
dependencies = [
    "numpy",
    "openai",
    "PyYAML",
    "tarski",
    "transformers",
    "pddl",
    "tqdm",
    "rich",
    "anthropic",
    "boto3",
    "google-genai",
    "groq",
    "seaborn",
    "matplotlib",
    "pandas",
]

[project.optional-dependencies]
local = ["vllm"]
dev = ["pytest", "ruff", "mypy"]
```

---

## 8. Migration Phases

### Phase 0: Merge Audit and Bug Fixes
Fix bugs in current code (groq_client, duplicate imports/functions, side effects). Diff all shared modules and decide canonical source per the merge strategy. Create the canonical task registry (16 tasks). Create import migration spec mapping every symbol to its future location.

### Phase 1: Scaffolding
Create `pyproject.toml`, `src/planbench/__init__.py`. Merge configs, instances, obfuscate, pddlgenerators into `data/`. Define strategy for historical experiment data. Update `.gitignore`.

### Phase 2: Core Modules
Create `config.py` (DomainConfig with 22 fields). Move `model_parser/` (identical, rename files). Move merged Executor. Move `full_validator`.

### Phase 3: Break Up Utils (with compatibility shim)
Split into `translation/`, `task_utils/` (6 submodules with `state.py` as base), `evaluation/validator.py`, slim `utils.py`. Keep `utils/__init__.py` as re-export shim. Fix all import-time side effects.

### Phase 4: LLM Abstraction
Create `llm/` package with `LLMClient` protocol (3 methods), `LLMResponse` (7 fields), and per-provider clients. Lazy initialization via `get_client()` factory. No side effects at import.

### Phase 5: Merge Pipeline Modules + Rewrite Imports
Rewrite all `from utils import *` to explicit imports. Merge `prompt_generation.py` (16 tasks), `response_generation.py` (LLMClient + parallelization), `response_evaluation.py` (all eval methods). Create `backprompting/` as 4-file package. Create `pipeline.py` orchestrator.

### Phase 6: Merge Generators and Obfuscation
Split `problem_generators.py` into per-domain files with shared base. Move obfuscator.

### Phase 7: Analysis and Stats
Move `stats_generation.py`, make hardcoded dicts configurable. Delete scratch files.

### Phase 8: CLI and Entry Points
Create `cli.py` with subcommands. Replace `eval()` with proper arg parsing. Wire up `pyproject.toml` entry points.

### Phase 9: Cleanup
Remove compatibility shim. Delete legacy directories and dead files. Add type hints and docstrings.

### Phase 10: Documentation
Update README (install, quickstart, engine reference, domain list, leaderboard paths). Document Python 3.10+ requirement. Document planner_tools env vars. Create migration guide and CONTRIBUTING.md.

---

## 9. Risk Summary

| Risk | Severity | Mitigation |
|------|----------|------------|
| Phase 3 breaks consumers | HIGH | Compatibility shim re-exports from new locations |
| Depots support lost during merge | HIGH | Phase 0 audit explicitly tracks depots branches in every file |
| Import cycles after utils split | MEDIUM | Acyclic dependency graph verified; import order documented |
| LLM provider inconsistencies | MEDIUM | LLMResponse normalizes all return types; provider quirks encapsulated |
| Historical experiment paths break | LOW | Phase 10 updates README; old data preserved in git history |
| Missing test coverage | LOW | Phase 9 adds type hints; dev dependencies include pytest |

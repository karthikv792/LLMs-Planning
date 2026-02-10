---
name: Restructure and Merge LLM Planning Library
overview: Merge `plan-bench/` and `llm_planning_analysis/` into a single clean, installable Python library (`planbench`) with proper packaging, modular architecture, explicit imports, configurable paths, and CLI entry points.
todos:
  - id: phase0-merge-analysis
    content: Audit both codebases file-by-file, resolve conflicts, and produce a single canonical version of each shared module (Executor, model_parser, utils/, task_utils, configs, instances)
    status: pending
  - id: phase1-scaffolding
    content: Create pyproject.toml, src/planbench/__init__.py, merge and move data/ assets (configs, instances, pddlgenerators, obfuscate), update .gitignore
    status: pending
  - id: phase2-core-modules
    content: Create config.py (DomainConfig dataclass), migrate merged model_parser/, executor/, full_validator/ with updated imports
    status: pending
  - id: phase3-break-utils
    content: Split merged utils/ into translation/, task_utils/ submodules, evaluation/validator.py, evaluation/feedback.py, and a slim utils.py; eliminate wildcard imports
    status: pending
  - id: phase4-llm-abstraction
    content: Create llm/ package with LLMClient protocol, per-provider modules (openai, google, anthropic, aws, local), and factory function; remove side effects at import
    status: pending
  - id: phase5-pipeline-modules
    content: Merge and refactor both prompt_generation.py, response_generation.py, response_evaluation.py, plus back_prompting.py into new module locations with all 13 tasks unified
    status: pending
  - id: phase6-generators
    content: Merge both problem_generators.py into generators/blocksworld.py, logistics.py, sokoban.py; extract shared base; remove duplicate classes
    status: pending
  - id: phase7-analysis
    content: Move stats_generation.py to analysis/stats.py with configurable model dicts and font paths; delete scratch files
    status: pending
  - id: phase8-cli
    content: Create cli.py with subcommands covering all 13 tasks; replace eval() with proper arg parsing; add entry points in pyproject.toml
    status: pending
  - id: phase9-cleanup
    content: Remove __pycache__ from git, delete dead code/files, remove plan-bench/ and llm_planning_analysis/ legacy directories, add type hints
    status: pending
  - id: phase10-docs
    content: Update README.md with install + quickstart covering all tasks, add module docstrings, create CONTRIBUTING.md
    status: pending
isProject: false
---

# Merge `plan-bench/` and `llm_planning_analysis/` into a Clean Python Library

## Relationship Between the Two Codebases

`plan-bench/` and `llm_planning_analysis/` are **forked copies of the same codebase** that diverged over time. They share identical scaffolding (`Executor/`, `model_parser/`, `utils/__init__.py`, `utils/pddl_to_text.py`, `utils/text_to_pddl.py`) but each added features the other lacks:

| Aspect | `plan-bench/` (PlanBench paper) | `llm_planning_analysis/` (Planning Abilities + o1 papers) |
|--------|-------------------------------|--------------------------------------------------------|
| **Tasks** | 10 tasks: t1-t8 (plan gen, optimality, verification, reuse, generalization, replanning, execution, goal reformulation x3) | 5 tasks: t1, t1_zero, t1_cot, t1_pddl, t1_zero_pddl + backprompting |
| **LLM support** | Old OpenAI API + BLOOM only | New OpenAI client, Gemini, Claude, AWS Bedrock, DeepSeek, Groq, QwQ, local models |
| **Domains** | blocksworld, logistics, mystery_blocksworld, depots, obfuscated variants | blocksworld, logistics, mystery_blocksworld, sokoban, obfuscated variants, unsolvable variants |
| **Parallelism** | Sequential only | ThreadPoolExecutor for response generation and evaluation |
| **Evaluation** | evaluate_plan, evaluate_state, evaluate_verification | evaluate_plan, evaluate_plan_pddl, evaluate_plan_parallel (LLM-based extraction) |
| **Extras** | -- | obfuscator.py, stats_generation.py, back_prompting.py, full_validator/ |
| **Config overlap** | 10 YAML configs (includes depots) | 15 YAML configs (includes sokoban, unsolvable, blocksworld_hard) |
| **Instances** | ~4285 PDDL files (includes depots) | ~2397 PDDL files (includes sokoban, unsolvable) |

### Shared modules (identical or near-identical)
- `Executor/__init__.py` -- plan-bench adds `is_capitalized` field; both have same core logic
- `model_parser/` -- identical across both
- `utils/__init__.py` -- identical (BWGenerator, LogisticsGenerator, validate_plan, caesar_encode, etc.)
- `utils/pddl_to_text.py` -- identical
- `utils/task_utils.py` -- plan-bench has depots support + more task functions; llm_planning_analysis has identical base
- `problem_generators.py` -- identical base; llm_planning_analysis adds sokoban generator

## Current Problems (both codebases)

- **Two diverged copies**: The same core code exists in two directories with no shared imports, causing silent divergence
- **No packaging**: No `pyproject.toml`/`setup.py`; code is not installable via `pip`
- **Hardcoded relative paths**: Every file uses paths like `./instances/`, `./configs/`, `./results/` -- only works when run from within each directory
- **Wildcard imports**: `from utils import *` used everywhere, making dependencies invisible
- **Massive code duplication**: `read_config()` duplicated in 8+ classes across both; `LogisticsGenerator`/`BWGenerator` duplicated in `utils/__init__.py` of both AND in `problem_generators.py`; `llama_messages_to_single_prompt` defined twice in the same file
- **Side effects at import time**: Both `utils/__init__.py` set `openai.api_key` and `random.seed(10)` on import; `llm_utils.py` creates AWS boto3 clients at module level
- **Monolithic files**: `task_utils.py` (500+ lines) mixes prompt templates, plan verification, replanning, execution, and COT generation; `llm_utils.py` (625 lines) has every LLM provider in one giant function
- **No type hints or docstrings** on most public APIs
- **`eval()` on user input**: `eval(args.verbose)` and `eval(args.random_example)` are security risks
- **Global mutable state**: `np.random.seed(42)` at module level in multiple files
- **Dead code**: Extensive commented-out blocks, unused functions, scratch files
- **Outdated dependencies**: `numpy==1.19.5` (from 2021); plan-bench still uses old `openai.Completion.create` API
- **Inconsistent naming**: `Instance_Generator` vs `GeneralizationInstanceGenerator`, `Executor/` (uppercase folder)

---

## Merge Strategy (file-by-file)

For each shared module, the following canonical source is chosen:

| Module | Canonical source | Merge notes |
|--------|-----------------|-------------|
| `Executor/__init__.py` | plan-bench (has `is_capitalized`) | Add `replanning_domain_specific()` depots branch from plan-bench |
| `model_parser/*` | Either (identical) | Use plan-bench copy |
| `utils/pddl_to_text.py` | Either (identical) | Use llm_planning_analysis copy |
| `utils/text_to_pddl.py` | llm_planning_analysis | Has `text_to_plan_with_llm()` that plan-bench lacks |
| `utils/task_utils.py` | Merge both | plan-bench has `plan_verification()`, `optimality()`, `replanning()`, `plan_execution()`, `paraphrase_goal()`, `generate_plan_subset()` for depots; llm_planning_analysis has the same functions without depots. Take plan-bench's version + add any llm_planning_analysis-only functions |
| `utils/__init__.py` | Delete | BWGenerator/LogisticsGenerator already in problem_generators.py; validate_plan/caesar_encode move to utils.py |
| `utils/llm_utils.py` | llm_planning_analysis | Has all modern providers (Gemini, Claude, Bedrock, DeepSeek, Groq); plan-bench only has OpenAI+BLOOM. Refactor into llm/ package |
| `prompt_generation.py` | Merge both | plan-bench has tasks t1-t8; llm_planning_analysis has t1 variants (zero_shot, cot, pddl). Unified PromptGenerator supports all 13 tasks |
| `response_generation.py` | Merge both | Use llm_planning_analysis's LLM routing; add plan-bench's task dict entries |
| `response_evaluation.py` | Merge both | plan-bench has `evaluate_state()`, `evaluate_verification()`, `parse_output()`; llm_planning_analysis has `evaluate_plan_pddl()`, `evaluate_plan_parallel()`. Keep all |
| `problem_generators.py` | Merge both | plan-bench has depots generator; llm_planning_analysis has sokoban. Keep all |
| `obfuscator.py` | llm_planning_analysis | Only exists in llm_planning_analysis |
| `back_prompting.py` | llm_planning_analysis | Only exists in llm_planning_analysis |
| `stats_generation.py` | llm_planning_analysis | Only exists in llm_planning_analysis |
| `configs/*.yaml` | Merge all | Union of plan-bench (10 YAMLs including depots) + llm_planning_analysis (15 YAMLs including sokoban/unsolvable) |
| `instances/` | Merge all | Union: plan-bench contributes depots (~502 PDDL files); llm_planning_analysis contributes sokoban. Shared domains (blocksworld, logistics) may have different instance sets -- keep both under separate subdirectories if they differ |

## Proposed Directory Structure

```
LLMs-Planning/
  pyproject.toml
  README.md
  LICENSE
  .gitignore
  src/
    planbench/
      __init__.py                 # version, top-level re-exports
      config.py                   # DomainConfig dataclass, load/validate YAML
      pipeline.py                 # orchestrates prompt -> response -> eval (all 13 tasks)
      cli.py                      # click/argparse CLI entry points
      prompts/
        __init__.py
        generator.py              # Unified PromptGenerator (merged: plan-bench t1-t8 + llm_planning_analysis t1 variants)
        templates.py              # fill_template, instance_to_text helpers
      llm/
        __init__.py               # factory: get_client(engine_name)
        base.py                   # abstract LLMClient protocol/ABC
        openai_client.py          # OpenAI chat + completion models
        google_client.py          # Gemini / PaLM
        anthropic_client.py       # Claude direct
        aws_client.py             # Bedrock (Claude, LLaMA)
        local_client.py           # BLOOM, QwQ, DeepSeek-R1 local
      evaluation/
        __init__.py
        evaluator.py              # Merged ResponseEvaluator (evaluate_plan, evaluate_state, evaluate_verification, evaluate_plan_pddl, evaluate_plan_parallel)
        validator.py              # validate_plan(), validate_plan_unsolvable(), VAL wrapper
        feedback.py               # get_val_feedback(), get_validation_message(), reformat_feedback()
      translation/
        __init__.py
        pddl_to_text.py           # parse_problem, instance_to_text, get_plan_as_text
        text_to_pddl.py           # text_to_plan, text_to_plan_with_llm, text_to_state
      executor/
        __init__.py
        executor.py               # Merged Executor class (plan-bench's is_capitalized + both sets of domain-specific replanning)
        full_validator.py          # get_all_errors (from full_validator/__init__.py)
      generators/
        __init__.py
        base.py                   # shared instance_ok(), convert_pddl(), hash helpers
        blocksworld.py            # blocksworld instance + generalization generators
        logistics.py              # logistics generators
        depots.py                 # depots generators (from plan-bench)
        sokoban.py                # sokoban generators (from llm_planning_analysis)
      obfuscation/
        __init__.py
        obfuscator.py             # from llm_planning_analysis/obfuscator.py
      analysis/
        __init__.py
        stats.py                  # stats_generation.py (cleaned up)
      backprompting/
        __init__.py
        backprompter.py           # from llm_planning_analysis/back_prompting.py
      model_parser/               # keep mostly as-is, just rename files
        __init__.py
        constants.py
        parser.py                 # was parser_new.py
        writer.py                 # was writer_new.py
      response/
        __init__.py
        generator.py              # Merged ResponseGenerator (modern LLM routing + all task dicts)
      task_utils/
        __init__.py
        cot.py                    # generate_plan_cot, plan_execution
        replanning.py             # replanning(), generate_plan_subset
        verification.py           # plan_verification, plan_verification_zero_shot
        optimality.py             # optimality() (from plan-bench task_utils)
        goal_reformulation.py     # paraphrase_goal() (from plan-bench task_utils)
        state.py                  # get_state_translation, get_action_text (merged: includes depots support)
      utils.py                    # caesar_encode/decode, treat_on, get_cost_gpt_3, misc
  data/
    configs/                      # Merged YAML domain configs (union of both directories)
    instances/                    # Merged PDDL instances (union of both directories)
    obfuscate/                    # word lists
    pddlgenerators/               # C generators
  results/                        # output results (gitignored by default)
  planner_tools/                  # external planner binaries, untouched
```

---

## Key Design Decisions

### 1. Package name: `planbench`

Matches the paper name, is short, and is `pip install`-able. The `src/` layout prevents accidental imports from the project root. The merged library covers all three papers (Planning Abilities, PlanBench, o1 Evaluation).

### 2. Configuration as a dataclass

Replace scattered `yaml.safe_load()` calls with a single `DomainConfig` dataclass in [src/planbench/config.py](src/planbench/config.py):

```python
@dataclass
class DomainConfig:
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
    # ... optional fields with defaults

    @classmethod
    def from_yaml(cls, path: str | Path) -> "DomainConfig": ...
```

### 3. LLM client abstraction

Replace the 300-line `send_query()` function with a protocol-based approach:

```python
class LLMClient(Protocol):
    def query(self, prompt: str, **kwargs) -> LLMResponse: ...
    def query_with_history(self, messages: list[dict], **kwargs) -> LLMResponse: ...

@dataclass
class LLMResponse:
    text: str
    raw_response: Any
    time_taken: float
    usage: dict | None = None
```

A factory function `get_client(engine: str) -> LLMClient` dispatches to the right provider. No boto3/anthropic clients are created at import time.

### 4. Path resolution

All path-dependent code will use a `workspace_root` parameter (defaults to `data/` within the installed package or a user-specified directory). The current pattern of `./instances/...` will be replaced with:

```python
config.resolve_instance_path(instance_id)  # returns Path object
```

### 5. CLI via entry points

Define CLI commands in `pyproject.toml`:

```toml
[project.scripts]
planbench = "planbench.cli:main"
```

Subcommands: `planbench run`, `planbench prompt`, `planbench evaluate`, `planbench generate`, `planbench obfuscate`, `planbench stats`.

---

## Migration Steps (in order)

### Phase 0: Merge Audit and Conflict Resolution

This new phase ensures both codebases are properly unified before restructuring.

- **Diff shared modules**: Run a file-by-file diff of all shared modules between `plan-bench/` and `llm_planning_analysis/`:
  - `Executor/__init__.py` -- merge plan-bench's `is_capitalized` field + depots replanning branch into llm_planning_analysis's version
  - `utils/task_utils.py` -- take plan-bench's version as base (has depots support + all 10 task helper functions), then add any functions unique to llm_planning_analysis (e.g., `plan_verification_zero_shot`, VAL feedback functions)
  - `utils/text_to_pddl.py` -- take llm_planning_analysis's version (superset: has `text_to_plan_with_llm()`)
  - `utils/pddl_to_text.py` -- identical, use either
  - `model_parser/` -- identical, use either
- **Merge configs**: Create union of all YAML config files. Resolve `depots.yaml` (plan-bench only) and `sokoban.yaml` (llm_planning_analysis only) into the shared `data/configs/` directory. For configs that exist in both (blocksworld, logistics, mystery variants), diff and take the version with more fields
- **Merge instances**: Create union of all PDDL instance directories. Blocksworld instances exist in both but plan-bench has ~2209 vs llm_planning_analysis's ~500. Keep the larger set (plan-bench), supplemented with any llm_planning_analysis-only domains (sokoban, unsolvable_blocksworld)
- **Merge problem_generators.py**: plan-bench has depots generator logic + `Instance_Generator` with label tracking; llm_planning_analysis has sokoban generator + `GeneralizationInstanceGenerator`. Combine into a single `Instance_Generator` base with all domain generators
- **Plan task ID unification**: Define a canonical task registry covering all 13 tasks:
  - t1: Plan Generation (one-shot) -- from plan-bench
  - t1_zero: Plan Generation (zero-shot) -- from llm_planning_analysis
  - t1_cot: Plan Generation (CoT) -- from llm_planning_analysis
  - t1_pddl: Plan Generation (PDDL one-shot) -- from llm_planning_analysis
  - t1_zero_pddl: Plan Generation (PDDL zero-shot) -- from llm_planning_analysis
  - t2: Optimal Planning -- from plan-bench
  - t3: Plan Verification -- from plan-bench
  - t4: Plan Reuse -- from plan-bench
  - t5: Plan Generalization -- from plan-bench
  - t6: Replanning -- from plan-bench
  - t7: Reasoning about Plan Execution -- from plan-bench
  - t8_1: Goal Reformulation (Goal shuffling) -- from plan-bench
  - t8_2: Goal Reformulation (Full -> Partial) -- from plan-bench
  - t8_3: Goal Reformulation (Partial -> Full) -- from plan-bench

### Phase 1: Scaffolding

- Create `pyproject.toml` with metadata, dependencies, and entry points
- Create `src/planbench/__init__.py` with `__version__`
- Move merged `data/` assets (configs from both, instances from both, obfuscate, pddlgenerators from both)
- Update `.gitignore` (add `__pycache__/`, `*.pyc`, `dist/`, `build/`, `.eggs/`, `*.egg-info`)

### Phase 2: Core modules (no behavior changes)

- Create `config.py` with `DomainConfig` dataclass
- Move `model_parser/` as-is, rename `parser_new.py` -> `parser.py`, `writer_new.py` -> `writer.py`
- Move merged `Executor/` -> `executor/executor.py`, update imports
- Move `full_validator/` -> `executor/full_validator.py`

### Phase 3: Break up utils

- Create `utils.py` for truly generic helpers (caesar encode/decode, get_cost_gpt_3, etc.)
- Create `translation/pddl_to_text.py` from merged `utils/pddl_to_text.py` -- explicit imports, remove domain-specific logic duplication
- Create `translation/text_to_pddl.py` from llm_planning_analysis's `utils/text_to_pddl.py` (superset)
- Create `task_utils/state.py`, `task_utils/cot.py`, `task_utils/verification.py`, `task_utils/replanning.py`, `task_utils/optimality.py`, `task_utils/goal_reformulation.py` by splitting merged `utils/task_utils.py`
- Create `evaluation/validator.py` extracting `validate_plan()`, `validate_plan_unsolvable()` from old `utils/__init__.py`
- Create `evaluation/feedback.py` extracting `get_val_feedback()`, `get_validation_message()` from `utils/task_utils.py`
- Remove both old `utils/__init__.py` wildcard barrel files

### Phase 4: LLM abstraction

- Create `llm/base.py` with `LLMClient` protocol and `LLMResponse` dataclass
- Create `llm/openai_client.py` (chat, completion, finetuned -- covers plan-bench's old API too)
- Create `llm/google_client.py` (Gemini, PaLM)
- Create `llm/anthropic_client.py` (direct Claude)
- Create `llm/aws_client.py` (Bedrock LLaMA, Claude)
- Create `llm/local_client.py` (BLOOM, QwQ, Qwen, DeepSeek local)
- Create `llm/__init__.py` with `get_client(engine)` factory -- lazy initialization, no side effects at import
- Remove both `utils/llm_utils.py` files

### Phase 5: Merge and refactor pipeline modules

- **Merge prompt_generation.py**: plan-bench's `PromptGenerator` has tasks t1-t8 (10 methods); llm_planning_analysis's has t1 variants (5 methods). Create a unified `PromptGenerator` in `prompts/generator.py` with all 13+ task methods. Use `DomainConfig`, explicit imports
- **Merge response_generation.py**: plan-bench's `ResponseGenerator` uses old OpenAI API; llm_planning_analysis's supports all modern providers. Create unified `ResponseGenerator` in `response/generator.py` using the new `LLMClient` abstraction with all task dict entries
- **Merge response_evaluation.py**: plan-bench's `ResponseEvaluator` has `evaluate_plan`, `evaluate_state`, `evaluate_verification`, `parse_output`; llm_planning_analysis's has `evaluate_plan`, `evaluate_plan_pddl`, `evaluate_plan_parallel`. Create unified `ResponseEvaluator` in `evaluation/evaluator.py` with all evaluation methods
- Move `back_prompting.py` -> `backprompting/backprompter.py` (llm_planning_analysis only)
- Create `pipeline.py` as the orchestrator (replaces both `llm_plan_pipeline.py` files, supports all 13 tasks)

### Phase 6: Merge generators and obfuscation

- Merge `problem_generators.py` from both into `generators/blocksworld.py`, `generators/logistics.py`, `generators/depots.py`, `generators/sokoban.py`
- Extract shared logic into `generators/base.py` (hash dedup, `instance_ok()`, plan computation)
- Move `obfuscator.py` -> `obfuscation/obfuscator.py`
- Remove duplicate generator classes from both old `utils/__init__.py` files

### Phase 7: Analysis and stats

- Move `stats_generation.py` -> `analysis/stats.py`
- Make font paths and model dictionaries configurable instead of hardcoded
- Remove `quick_stats.py` and `example_generator.py` (scratch files)

### Phase 8: CLI and entry points

- Create `cli.py` using `argparse` (or `click`) with subcommands covering all 13 tasks
- Replace `eval()` with proper `action="store_true"` for booleans
- Wire up `[project.scripts]` in `pyproject.toml`
- Ensure backward-compatible task ID aliases (e.g., `--task t1` still works)

### Phase 9: Cleanup

- Update `requirements.txt` -> use `pyproject.toml` `[project.dependencies]` with modern version ranges
- Remove all `__pycache__/` directories from tracking
- **Delete legacy directories**: `plan-bench/` and `llm_planning_analysis/` (all code now lives in `src/planbench/`)
- Delete dead files: `R1_reasoning_examples_2.txt`, `quick_stats.py`, `example_generator.py`
- Remove commented-out code blocks
- Add module-level docstrings to every `__init__.py`
- Add basic type hints to all public function signatures

### Phase 10: Documentation

- Update root `README.md` with installation (`pip install -e .`), quickstart, full task reference table, and API reference pointers
- Create a migration guide for existing users of `plan-bench/` or `llm_planning_analysis/`
- Add a `CONTRIBUTING.md` with development setup instructions

---

## Dependency Updates (`pyproject.toml`)

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
    "seaborn",
    "matplotlib",
    "pandas",
]

[project.optional-dependencies]
local = ["vllm"]  # for local model inference
dev = ["pytest", "ruff", "mypy"]
```


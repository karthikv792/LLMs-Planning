# Restructuring TODO: Merge into `planbench` Package

> Corrected phase plan based on deep audit of both codebases.
> Precedence rule: **llm_planning_analysis is the latest code** and takes priority for all diverged files.
> Import strategy: **compatibility shim** in Phase 3 to avoid breaking consumers until Phase 5.

---

## Phase 0: Merge Audit and Bug Fixes

- [ ] Fix `groq_client` bug in `llm_planning_analysis/utils/llm_utils.py:211` (referenced but never defined)
- [ ] Remove duplicate import blocks in `llm_planning_analysis/utils/llm_utils.py` (lines 18-20 duplicated at 25-27)
- [ ] Remove duplicate `llama_messages_to_single_prompt()` definition in `llm_planning_analysis/utils/llm_utils.py`
- [ ] Diff `Executor/__init__.py` between both codebases:
  - Take llm_planning_analysis as base
  - Backport plan-bench's `len(self.plan)<2` safety check in `random_prefix_execution()`
  - Backport plan-bench's depots replanning branch
- [ ] Diff `utils/task_utils.py` between both codebases:
  - Take plan-bench as base (has depots support + all task helper functions)
  - Merge in any llm_planning_analysis-only functions (e.g., `validate_plan_unsolvable()`)
- [ ] Diff `utils/pddl_to_text.py`: take llm_planning_analysis (better error reporting), backport plan-bench's depots domain handling
- [ ] Diff `utils/text_to_pddl.py`: take llm_planning_analysis (superset with `text_to_plan_with_llm()`)
- [ ] Diff `obfuscator.py`: take llm_planning_analysis (22KB, larger/more complete) as base, verify depots support from plan-bench isn't lost
- [ ] Diff `utils/__init__.py`: take llm_planning_analysis (has `validate_plan_unsolvable()`)
- [ ] Create canonical task registry (16 unique tasks):
  - t1: Plan Generation (one-shot) — overlap, both codebases
  - t1_zero: Plan Generation (zero-shot) — llm_planning_analysis
  - t1_cot: Plan Generation (CoT / state tracking) — llm_planning_analysis
  - t1_pddl: Plan Generation PDDL (one-shot) — llm_planning_analysis
  - t1_zero_pddl: Plan Generation PDDL (zero-shot) — llm_planning_analysis
  - t2: Optimal Planning — plan-bench
  - t3: Plan Verification — plan-bench
  - t3_1: Plan Verification with LLM Plans — plan-bench
  - t3_zero_shot: Zero-shot Plan Verification — plan-bench
  - t4: Plan Reuse — plan-bench
  - t5: Plan Generalization — plan-bench
  - t6: Replanning — plan-bench
  - t7: Reasoning about Plan Execution — plan-bench
  - t8_1: Goal Reformulation (Goal shuffling) — plan-bench
  - t8_2: Goal Reformulation (Full -> Partial) — plan-bench
  - t8_3: Goal Reformulation (Partial -> Full) — plan-bench
- [ ] Create import migration spec: table mapping every symbol from `from utils import *` to its future module path

---

## Phase 1: Scaffolding

- [ ] Create `pyproject.toml` with metadata, dependencies (fix numpy pin, add all actual deps: anthropic, google-genai, boto3, tqdm, rich, seaborn, matplotlib, pandas, groq), and entry points
- [ ] Create `src/planbench/__init__.py` with `__version__`
- [ ] Merge configs into `data/configs/`:
  - Use llm_planning_analysis version for overlapping configs (has `domain_intro_state_tracking`, `encoded_objects_compact`)
  - Include plan-bench-only: `depots.yaml`
  - Include llm_planning_analysis-only: `sokoban.yaml`, `blocksworld_hard.yaml`, `unsolvable_blocksworld.yaml`, `unsolvable_obfuscated_randomized_blocksworld.yaml`
  - Decide on `config_mask.yaml` and `default_template.yaml` (development artifacts? keep or drop?)
- [ ] Merge instances into `data/instances/`:
  - blocksworld: use plan-bench set (2,209 files, larger)
  - depots: plan-bench only (502 files) — CRITICAL, do not lose
  - logistics: keep both sets (plan-bench 573, llm_planning_analysis 287)
  - sokoban: llm_planning_analysis only (57 files)
  - blocksworld_hard: llm_planning_analysis only (111 files)
  - unsolvable variants: llm_planning_analysis only (204 files)
  - obfuscated variants: keep both sets
- [ ] Merge `data/obfuscate/` (identical `random_words_1.txt`)
- [ ] Merge `data/pddlgenerators/`: identical blocksworld/logistics from both + sokoban from llm_planning_analysis
- [ ] Define strategy for historical experiment data (`prompts/`, `responses/`, `results/`, `results_backprompting/`, `graphs/`): gitignore going forward or move to `legacy/`
- [ ] Decide on binary files: `gpt_sas_plan`, `sas_plan`, `llm_plan` — keep or delete?
- [ ] Update `.gitignore` (add `__pycache__/`, `*.pyc`, `dist/`, `build/`, `.eggs/`, `*.egg-info`, `results/`)

---

## Phase 2: Core Modules (no behavior changes)

- [ ] Create `config.py` with `DomainConfig` dataclass and `from_yaml()` classmethod. Must include ALL 22 config keys found across both codebases:
  - **Required**: `domain_name`, `domain_file`, `instance_dir`, `instances_template`, `n_instances`, `start`, `end`, `domain_intro`, `actions`, `predicates`, `encoded_objects`
  - **Optional intro variants**: `domain_intro_state_tracking`, `domain_intro_zero_shot`, `domain_intro_cost`
  - **Optional instance paths**: `generalized_instance_dir`
  - **Optional mappings**: `predicate_mapping`, `encoded_objects_compact`
  - **Optional obfuscation**: `action_obfuscation`, `predicate_obfuscation`
  - **Optional metadata**: `objects`, `callbacks`
- [ ] Move `model_parser/` as-is (identical in both), rename `parser_new.py` -> `parser.py`, `writer_new.py` -> `writer.py`
- [ ] Move merged `Executor/` -> `executor/executor.py`:
  - Base: llm_planning_analysis version
  - Merge in: plan-bench's `len(self.plan)<2` safety check
  - Merge in: plan-bench's depots replanning branch
  - Update internal imports to new module paths
- [ ] Move `full_validator/` -> `executor/full_validator.py`

---

## Phase 3: Break Up Utils (with compatibility shim)

- [ ] Create `translation/pddl_to_text.py` from merged `pddl_to_text.py` (llm_planning_analysis base + depots handling from plan-bench)
- [ ] Create `translation/text_to_pddl.py` from llm_planning_analysis's version (superset with `text_to_plan_with_llm()`)
- [ ] Create `task_utils/state.py` — `get_state_translation()`, `get_action_text()`, **`parsed_instance_to_text_blocksworld()`** (merged, includes depots support). This is the base module with no internal dependencies.
- [ ] Create `task_utils/cot.py` — `generate_plan_cot()`, `plan_execution()`. Depends on: `state.py`
- [ ] Create `task_utils/verification.py` — `plan_verification()`, `plan_verification_zero_shot()`, `plan_verification_zero_shot_val_form()`, `reformat_feedback()`, `get_val_feedback()`, `get_validation_message()`. Depends on: `state.py`
- [ ] Create `task_utils/replanning.py` — `replanning()`, `generate_plan_subset()`. Depends on: `state.py`
- [ ] Create `task_utils/optimality.py` — `optimality()`. Depends on: `state.py`
- [ ] Create `task_utils/goal_reformulation.py` — `paraphrase_goal()`. Depends on: `state.py`
- [ ] Create `task_utils/__init__.py` with ordered re-exports: state first, then all others (acyclic dependency graph confirmed)
- [ ] Create `evaluation/validator.py` — `validate_plan()`, `validate_plan_unsolvable()`, VAL wrapper
- [ ] Create slim `utils.py` — `caesar_encode/decode`, `treat_on`, `get_cost_gpt_3`, misc helpers
- [ ] **CRITICAL: Keep `utils/__init__.py` as a re-export shim** that imports from all new locations and re-exports, so existing `from utils import *` consumers keep working
- [ ] **Fix side effects**: remove `openai.api_key = ...`, `random.seed(10)`, boto3 client creation, and `OpenAI()` instantiation from module-level code in all new modules

---

## Phase 4: LLM Abstraction

- [ ] Create `llm/base.py` with:
  - `LLMClient` protocol with 3 methods:
    - `query(prompt, max_tokens, **kwargs) -> LLMResponse` — single prompt, single response
    - `query_with_history(messages, max_tokens, **kwargs) -> LLMResponse` — multi-turn conversation (used by backprompting)
    - `query_multiple(prompt, n, max_tokens, **kwargs) -> list[LLMResponse]` — multiple completions per prompt
  - `LLMResponse` dataclass with all fields actually used across codebase:
    - `text: str`
    - `raw_response: Any`
    - `time_taken: float`
    - `usage: dict | None` (must support nested `completion_tokens_details.reasoning_tokens` for o1/thinking models)
    - `messages: list[dict] | None` (updated conversation history for multi-turn)
    - `context_window_exceeded: bool` (flag for context limit errors)
    - `null_response: bool` (flag for empty/whitespace responses)
- [ ] Create `llm/openai_client.py` (chat + completion, covers plan-bench's old API). Must handle:
  - o1 models: no temperature parameter, reasoning token extraction
  - Chat models: message formatting, retry with 3000s sleep
  - Finetuned: legacy completion API
  - DeepSeek-R1: separate API key, no system message, no temperature
- [ ] Create `llm/google_client.py` (Gemini, PaLM/Vertex AI). Must handle:
  - Gemini thinking models: separate thinking tokens from response text
  - 70-second rate-limit sleep between requests
  - Alpha API version for thinking config
  - PaLM: deprecated `text-bison@001` model (consider dropping)
- [ ] Create `llm/anthropic_client.py` (Claude direct). Must handle:
  - Hard-coded system prompt (make configurable)
  - Response content array filtering by type='text'
- [ ] Create `llm/aws_client.py` (Bedrock: Claude, LLaMA). Must handle:
  - Different request/response formats for Claude vs LLaMA
  - LLaMA custom prompt format (`<|begin_of_text|>` tags) via message-to-prompt conversion
  - Different max token params: `max_tokens` (Claude) vs `max_gen_len` (LLaMA)
- [ ] Create `llm/groq_client.py` (fix the broken provider — proper import + client init). Must handle:
  - Temperature clamped to 0.0001 minimum
  - 2-second rate-limit sleep between requests
- [ ] Create `llm/local_client.py` (BLOOM, QwQ, Qwen, DeepSeek-R1 local). Must handle:
  - Model loading via transformers (AutoTokenizer/AutoModelForCausalLM)
  - DeepSeek-R1 loads via vLLM LLM() instead of transformers
  - GPU device mapping and 8-bit quantization
  - No token usage tracking available
  - Return model dict `{'model': ..., 'tokenizer': ...}` during init
- [ ] Create `llm/__init__.py` with `get_client(engine)` factory — **lazy initialization, no side effects at import**
- [ ] Handle missing credentials gracefully (error on first `query()` call, not at import)
- [ ] Remove both old `utils/llm_utils.py` files
- [ ] Update the `utils/__init__.py` shim to re-export from `llm/` if any consumers used `send_query()` via the shim

---

## Phase 5: Merge Pipeline Modules + Rewrite Imports

- [ ] **Rewrite all `from utils import *`** to explicit imports from new module paths:
  - `prompt_generation.py`
  - `response_generation.py`
  - `response_evaluation.py`
  - `back_prompting.py`
  - `back_prompting_parallel.py`
- [ ] Merge `prompt_generation.py`: unified `PromptGenerator` in `prompts/generator.py` with all 16 task methods. Use `DomainConfig`, explicit imports
- [ ] Merge `response_generation.py`: unified `ResponseGenerator` in `response/generator.py` using `LLMClient` abstraction. Include llm_planning_analysis's `ThreadPoolExecutor` parallelization + cost tracking + all task dict entries
- [ ] Merge `response_evaluation.py`: unified `ResponseEvaluator` in `evaluation/evaluator.py`:
  - From plan-bench: `evaluate_plan()`, `evaluate_state()`, `evaluate_verification()`, `parse_output()`
  - From llm_planning_analysis: `evaluate_plan_pddl()`, `evaluate_plan_parallel()`, `extract_plans()`
- [ ] Create `backprompting/` as a 4-file package (NOT a single merged file — architectures are incompatible):
  - `backprompting/base.py` — shared utilities: `__init__`, `read_config()`, `get_problem()`, `get_executor()`, `get_bloom()`, `save_json()`, `load_json()`
  - `backprompting/backprompter.py` — sequential `BackPrompter`: `compute_plan()`, `is_already_correct()`, `task_1_plan_generation_backprompting()`, `get_llm_feedback()`, `get_repeated_verification()`
  - `backprompting/backprompter_parallel.py` — parallel `BackPrompter`: `_compute_plan_optimal()`, `per_instance_output()`, `per_instance_output_pddl()`, `task_1_plan_generation_backprompting()`, `task_1_plan_generation_backprompting_zero_shot_pddl()`, `get_repeated_verification_parallel()`, `get_repeated_verification_pddl()`, `verification()`. Depends on `full_validator`.
  - `backprompting/__init__.py` — exports `SequentialBackPrompter` and `ParallelBackPrompter`
- [ ] Create `pipeline.py` orchestrator (replaces both `llm_plan_pipeline.py`, supports all 16 tasks)
- [ ] Delete `promptgen.sh` (shell script replaced by CLI)

---

## Phase 6: Merge Generators and Obfuscation

- [ ] Split merged `problem_generators.py` into `generators/blocksworld.py`, `generators/logistics.py`, `generators/depots.py`, `generators/sokoban.py`
- [ ] Extract shared logic into `generators/base.py` (hash dedup, `instance_ok()`, plan computation)
- [ ] Move merged `obfuscator.py` -> `obfuscation/obfuscator.py` (llm_planning_analysis version, 22KB)
- [ ] Remove duplicate generator classes from old `utils/__init__.py` shim (BWGenerator, LogisticsGenerator)

---

## Phase 7: Analysis and Stats

- [ ] Move `stats_generation.py` -> `analysis/stats.py`
- [ ] Make MODELS dict, MODEL_GROUPS, DOMAINS, TASKS, font paths all configurable (not hardcoded)
- [ ] Mark for deletion: `quick_stats.py`, `example_generator.py`, `R1_reasoning_examples_2.txt`

---

## Phase 8: CLI and Entry Points

- [ ] Create `cli.py` with `argparse` (or `click`) subcommands covering all 16 tasks
- [ ] Replace `eval(args.verbose)` and `eval(args.random_example)` with proper `action="store_true"`
- [ ] Wire up `[project.scripts]` in `pyproject.toml`: `planbench = "planbench.cli:main"`
- [ ] Subcommands: `planbench run`, `planbench prompt`, `planbench evaluate`, `planbench generate`, `planbench obfuscate`, `planbench stats`, `planbench backprompt`
- [ ] Ensure backward-compatible task ID aliases

---

## Phase 9: Cleanup

- [ ] **Remove the `utils/__init__.py` compatibility shim** (all consumers now use explicit imports)
- [ ] Remove all `__pycache__/` directories from git tracking
- [ ] Delete legacy directories: `plan-bench/`, `llm_planning_analysis/`
- [ ] Delete dead files: `R1_reasoning_examples_2.txt`, `quick_stats.py`, `example_generator.py`, `promptgen.sh`
- [ ] Delete binary artifacts: `gpt_sas_plan`, `sas_plan`, `llm_plan` (if decided in Phase 1)
- [ ] Address `results_backprompting/` and `graphs/` (per Phase 1 decision)
- [ ] Remove all commented-out code blocks
- [ ] Add module-level docstrings to every `__init__.py`
- [ ] Add basic type hints to all public function signatures

---

## Phase 10: Documentation

- [ ] Update root `README.md`:
  - Installation: `pip install -e .`
  - Quickstart covering all 16 tasks
  - Update leaderboard paths (no longer `llm_planning_analysis/results/`)
  - Comprehensive engine reference (OpenAI, Gemini, Claude, Bedrock, Groq, DeepSeek, QwQ, Qwen, BLOOM)
- [ ] Document Python 3.10+ as minimum (breaking change from 3.6+)
- [ ] Document all supported domains (blocksworld, logistics, depots, sokoban, mystery variants, unsolvable variants, blocksworld_hard)
- [ ] Document `planner_tools/` env vars: `FAST_DOWNWARD`, `VAL`, `PR2`
- [ ] Create migration guide for existing users of `plan-bench/` or `llm_planning_analysis/`
- [ ] Create `CONTRIBUTING.md` with development setup instructions

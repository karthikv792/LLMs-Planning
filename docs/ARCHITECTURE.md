# Architecture

This document explains how PlanBench is structured and how data flows through it. Read it before
adding a task, a domain, or a model provider.

## High-level data flow

A benchmark run takes a **classical planning problem** (PDDL), turns it into a **prompt**, sends it
to an **LLM**, and **evaluates** the response against ground truth computed by sound planners.

```
            ┌─────────────┐
 config ───▶│ DomainConfig│   (data/configs/<domain>.yaml)
            └──────┬──────┘
                   │
 instance ─────────┼────────────────────────────────────────────────┐
 (PDDL)            ▼                                                  │
            ┌──────────────┐   PDDL → natural language               │
            │ PromptGenerator│  via translation/ + task_utils/        │
            └──────┬────────┘                                         │
                   │ prompt (text or PDDL)                            │
                   ▼                                                  │
            ┌──────────────┐   get_client(engine).query(...)         │
            │ ResponseGen.  │  llm/ provider clients                  │
            └──────┬────────┘                                         │
                   │ raw model response                               │
                   ▼                                                  │
            ┌──────────────┐   natural language → PDDL plan          │
            │ ResponseEval. │  translation/text_to_pddl               │
            │               │  → VAL validate (evaluation/validator)  │◀┘
            └──────┬────────┘   ↑ ground-truth plan from Fast Downward
                   │
                   ▼
            results/<domain>/<engine>/<task>.json   →  analysis/ (stats, plots)
```

The three pipeline stages are orchestrated by `planbench.pipeline.run_pipeline` and exposed
through the `planbench` CLI (`planbench.cli`). Each stage also has its own CLI subcommand
(`prompt`, `evaluate`) so they can be run independently.

## Modules and responsibilities

| Module | Responsibility | Key entry points |
|--------|----------------|------------------|
| `config.py` | Load + validate a domain config YAML | `DomainConfig.from_yaml` |
| `model_parser/` | PDDL ↔ internal `dict` model, backed by `tarski` | `parse_model`, `ModelWriter` |
| `translation/` | PDDL ↔ natural language | `instance_to_text`, `text_to_plan`, `text_to_state` |
| `task_utils/` | Per-task NL construction (CoT, verification, replanning, …) | `parsed_instance_to_text_blocksworld`, `get_state_translation` |
| `prompts/` | Build the prompt for each of the tasks | `PromptGenerator.task_*` |
| `llm/` | Provider clients behind one protocol | `get_client`, `LLMClient`, `LLMResponse` |
| `response/` | Query models (optionally in parallel), record cost | `ResponseGenerator.get_responses` |
| `evaluation/` | Extract a plan from the response and validate it | `ResponseEvaluator`, `validate_plan` |
| `backprompting/` | Iterative refinement with verifier feedback | `SequentialBackPrompter`, `ParallelBackPrompter` |
| `generators/` | Generate PDDL instances per domain | `BlocksworldInstanceGenerator`, … |
| `obfuscation/` | Rename actions/predicates to strip world knowledge | `random_mapping`, `translate_domain_pddl` |
| `analysis/` | Aggregate results into stats and figures | `StatsGenerator` |

Design principles carried over from the merge:

- **No import-time side effects.** Importing any module never creates an API client, reads an API
  key, or seeds the global RNG. Provider SDKs are imported lazily inside each client on first use.
  (`tests/test_imports.py` enforces this.)
- **One protocol for every provider.** `llm/base.py` defines `LLMClient` (a `Protocol`) and a single
  `LLMResponse` dataclass. Adding a provider means adding a client class and a branch in
  `get_client`; nothing downstream changes.

## The translation layer (most important for new domains)

`translation/` is where PDDL meets natural language, and it is **domain-aware**. Both directions
dispatch on `data['domain_name']`:

- `pddl_to_text.py` — `instance_to_text(problem, get_plan, data)` renders a parsed instance into
  the `(INIT, GOAL, PLAN, data)` natural-language tuple, using the config's `predicates`,
  `actions`, and `encoded_objects` templates.
- `text_to_pddl.py` — `text_to_plan(text, action_set, plan_file, data)` and
  `text_to_state(text, data)` parse a model's NL output back into grounded PDDL. These contain a
  per-domain branch (`text_to_plan_blocksworld`, `text_to_plan_logistics`, `text_to_plan_depots`,
  `text_to_plan_obfuscated`, and the matching `text_to_state_*`). There is an explicit
  `# ADD SPECIFIC TRANSLATION FOR EACH DOMAIN HERE` hook.

A new domain whose object/predicate surface forms differ from existing domains needs new branches
here. See [ADDING_A_DOMAIN.md](ADDING_A_DOMAIN.md).

## The working-directory convention

The pipeline classes resolve data paths **relative to the current working directory**
(`./configs/<name>.yaml`, `./instances/<instance_dir>/...`) and write outputs to `./prompts`,
`./responses`, `./results`. Because all bundled data lives under `data/`, you run `planbench`
commands from inside `data/`. (`DomainConfig.resolve_instance_path(data_root=...)` offers an
absolute-path alternative but is not yet threaded through the pipeline — a good future refactor.)

## Tasks

The task registry lives in `pipeline.py`:

- `ALL_TASKS` maps the 15 wired task keys (e.g. `t1`) to internal task names
  (e.g. `task_1_plan_generation`).
- `EVAL_PLAN_TASKS` / `EVAL_PLAN_PDDL_TASKS` / `EVAL_STATE_TASKS` / `EVAL_VERIFICATION_TASKS`
  route each task to its evaluation method.
- `_dispatch_prompt_generation` routes each task to the matching `PromptGenerator.task_*` method.

Adding a task means: add a `task_*` prompt method, register it in `_dispatch_prompt_generation`,
add it to the right `EVAL_*` table, and (if needed) add an evaluation method.

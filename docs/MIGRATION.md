# Migration guide

PlanBench's two historical code trees — `plan-bench/` and `llm_planning_analysis/` — have been
merged into a single installable package, **`planbench`** (under `src/`). This guide helps you move
existing code and workflows over.

## TL;DR

| Before | After |
|--------|-------|
| Two parallel dirs with duplicated code | One package: `pip install -e .` then `import planbench` |
| `from utils import *` | Explicit imports from `planbench.*` (see symbol map below) |
| `python llm_plan_pipeline.py --task t1 ...` | `planbench run --task t1 ...` |
| `eval(args.verbose)` string flags | Proper `--verbose` / `--ignore_existing` boolean flags |
| `configs/`, `instances/` in the code dir | `data/configs/`, `data/instances/` — run from `data/` |
| Module-level `openai.api_key = ...`, client creation at import | Lazy, credential-free imports; clients built on first query |

## 1. Installation

Instead of running scripts inside a subdirectory:

```bash
pip install -e .          # provides `import planbench` and the `planbench` CLI
```

## 2. Imports

The old catch-all `from utils import *` is gone. Import the specific symbols from their new homes:

| Old (via `utils`) | New |
|-------------------|-----|
| `instance_to_text`, `fill_template`, `get_plan_as_text` | `from planbench.translation import ...` |
| `text_to_plan`, `text_to_state`, `text_to_plan_with_llm` | `from planbench.translation import ...` |
| `get_state_translation`, `generate_plan_cot`, `plan_verification`, `replanning`, `optimality`, `paraphrase_goal` | `from planbench.task_utils import ...` |
| `validate_plan`, `validate_plan_unsolvable` | `from planbench.evaluation.validator import ...` |
| `parse_model`, `ModelWriter`, `Parser_PDDL` | `from planbench.model_parser import ...` |
| `send_query` and friends (`llm_utils`) | `from planbench.llm import get_client` → `get_client(engine).query(...)` |
| `caesar_encode`/`caesar_decode`, misc helpers | `from planbench.utils.helpers import ...` |
| `Executor` | `from planbench.executor import Executor` |
| `BWGenerator`, `LogisticsGenerator`, … | `from planbench.generators import BlocksworldInstanceGenerator, ...` |

A complete, symbol-by-symbol map (every name that used to be exported by `utils`) is in
[IMPORT_MIGRATION.md](IMPORT_MIGRATION.md).

### LLM calls

The old free functions for querying models are replaced by a client protocol:

```python
# before
resp = send_query(prompt, engine, max_tokens=1000)

# after
from planbench.llm import get_client
client = get_client(engine)            # no credentials touched here
resp = client.query(prompt, max_tokens=1000)
print(resp.text, resp.usage, resp.time_taken)
```

`get_client` dispatches on the engine string (`*_chat` → OpenAI, `*_aws` → Bedrock,
`claude-*` → Anthropic, `gemini*`/`palm` → Google, `*_groq` → Groq, `bloom`/`*_local`/`qwq*`/`qwen*`
→ local). See the engine table in the [README](../README.md#supported-model-engines).

## 3. Command line

The shell scripts and per-task entry points are replaced by one CLI with subcommands:

| Before | After |
|--------|-------|
| `python llm_plan_pipeline.py --task t1 --config blocksworld --engine gpt-4o_chat` | `planbench run --task t1 --config blocksworld --engine gpt-4o_chat` |
| `python prompt_generation.py ...` | `planbench prompt ...` |
| `python response_evaluation.py ...` | `planbench evaluate ...` |
| `python problem_generators.py ...` | `planbench generate ...` |
| `python obfuscator.py ...` | `planbench obfuscate ...` |
| `python stats_generation.py ...` | `planbench stats ...` |
| `python back_prompting.py` / `back_prompting_parallel.py` | `planbench backprompt [--parallel]` |
| `./promptgen.sh` | removed — use `planbench prompt` |

Boolean flags are now real flags: pass `--verbose` to enable it (previously `--verbose True`, parsed
with `eval`). Run `planbench <subcommand> --help` for the current options.

## 4. Data layout & working directory

Configs and instances moved under `data/`:

```
configs/<name>.yaml      →  data/configs/<name>.yaml
instances/<...>          →  data/instances/<...>
```

The pipeline still resolves paths relative to the current working directory, so **run `planbench`
commands from inside `data/`** (or symlink your data there). Outputs land in
`data/{prompts,responses,results}/`.

## 5. Behavioural changes to be aware of

- **Python 3.10+** is required (the old trees ran on 3.6+).
- **No import side effects:** importing `planbench` (or any submodule) no longer sets API keys,
  instantiates SDK clients, or calls `random.seed(...)`. Seeding now happens explicitly via the
  `--seed` flag / function arguments.
- **Lazy provider SDKs:** `openai`, `anthropic`, `boto3`, `google-genai`, `groq`, `transformers`,
  and `vllm` are imported only when a client of that type is actually used. Missing optional
  dependencies surface at query time, not import time.
- **The Groq client is fixed** (the old `groq_client` reference bug is resolved).

## Still stuck?

Open an issue describing the old call you're porting; we'll add it to the map.

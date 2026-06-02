# Contributing to PlanBench

Thanks for your interest in improving PlanBench! This guide covers local setup, the test suite,
code style, and the pull-request process.

## Development setup

PlanBench targets **Python 3.10+**. `tarski` (the PDDL backend) is most reliable on **3.12**.

```bash
git clone https://github.com/karthikv792/LLMs-Planning.git
cd LLMs-Planning
python3.12 -m venv .venv && source .venv/bin/activate
pip install -e ".[dev]"
```

The `[dev]` extra installs `pytest`, `ruff`, and `mypy`. The `[local]` extra
(`transformers` + `vllm`) is only needed if you work on the local-model clients.

### Optional: external planners

Some tests and all real benchmark runs use external tools, located via environment variables:

```bash
export VAL=/path/to/VAL/build/.../bin     # the `validate` binary's directory
export FAST_DOWNWARD=/path/to/downward    # contains fast-downward.py
```

You do **not** need these to develop or to run the default test suite — tests that require them
skip automatically.

## Running the tests

```bash
pytest                 # full offline suite (no network, no API keys)
pytest -rs             # also show why gated tests were skipped
pytest tests/test_translation.py -q
```

The suite is designed to be **hermetic**: no live LLM calls, no writes into the dataset directory.
Tests needing the VAL validator, Fast Downward, or an `OPENAI_API_KEY` are marked with
`requires_val`, `requires_fast_downward`, and `requires_openai_key` (see `tests/conftest.py`) and
skip when those resources are absent. Anything that mutates files writes into a pytest `tmp_path`.

If you add a test that needs a resource, gate it with the matching marker rather than assuming the
environment has it.

## Code style

- **Lint:** `ruff check src tests`. CI gates on the bug-catching rule set (syntax errors + undefined
  names); run `ruff check --select F src` for the full report including unused imports/vars.
- **Format:** keep changes consistent with surrounding code. New modules use
  `from __future__ import annotations` and type hints on public signatures.
- **No import-time side effects.** Importing a module must never create an API client, read a
  credential, or seed the global RNG. Provider SDKs are imported lazily inside their client classes.
  `tests/test_imports.py` enforces this — keep it green.

## Where things go

| You want to… | Start here | See |
|--------------|-----------|-----|
| Add a planning domain | `data/configs/`, `translation/` | [docs/ADDING_A_DOMAIN.md](docs/ADDING_A_DOMAIN.md) |
| Add a model provider | `src/planbench/llm/` + `get_client` | [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) |
| Add a task | `pipeline.py`, `prompts/`, `evaluation/` | [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) |
| Understand the data flow | — | [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) |

## Pull-request process

1. Branch off `main` (or the current development branch).
2. Make your change with tests. Keep PRs focused.
3. Ensure `pytest` and `ruff check src tests` pass locally.
4. Open a PR describing the change and how you verified it. CI runs the lint + test suite on
   Python 3.10–3.12.
5. For leaderboard submissions, attach the result JSON file (see the README).

## Reporting issues

Open a GitHub issue with: what you ran (command + engine + task + domain), what you expected, what
happened, and your environment (OS, Python version, planner versions). Minimal repros help a lot.

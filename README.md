# PlanBench

**PlanBench** is a benchmark suite for evaluating the planning and reasoning-about-change
capabilities of Large Language Models (LLMs) and Large Reasoning Models (LRMs). It generates
prompts from classical PDDL planning problems, queries a model, and rigorously evaluates the
responses against ground truth using sound automated planners and the VAL plan validator.

This repository hosts the code for three papers:

1. ["PlanBench: An Extensible Benchmark for Evaluating Large Language Models on Planning and Reasoning about Change"](https://arxiv.org/abs/2206.10498) (NeurIPS 2023 D&B)
2. ["On the Planning Abilities of Large Language Models — A Critical Investigation"](https://arxiv.org/abs/2305.15771) (NeurIPS 2023 Spotlight)
3. ["A Systematic Evaluation of the Planning and Scheduling Abilities of the Reasoning Model o1"](https://openreview.net/forum?id=FkKBxp0FhR) (TMLR 2025)

> **Note:** The two historical code trees (`plan-bench/` and `llm_planning_analysis/`) have been
> merged into a single installable package, `planbench` (under `src/`). If you used the old layout,
> see [docs/MIGRATION.md](docs/MIGRATION.md).

---

## Table of contents

- [Installation](#installation)
- [External planning tools](#external-planning-tools)
- [Quickstart](#quickstart)
- [The tasks](#the-tasks)
- [Supported domains](#supported-domains)
- [Supported model engines](#supported-model-engines)
- [Project layout](#project-layout)
- [Documentation](#documentation)
- [Leaderboard](#planbench-static-test-set-leaderboard)
- [Citations](#citations)

---

## Installation

PlanBench requires **Python 3.10+**.

```bash
git clone https://github.com/karthikv792/LLMs-Planning.git
cd LLMs-Planning
python -m venv .venv && source .venv/bin/activate
pip install -e .
```

This installs the `planbench` package and the `planbench` command-line entry point, plus all
runtime dependencies (the provider SDKs `openai`, `anthropic`, `google-genai`, `boto3`, `groq`;
the PDDL stack `tarski` + `pddl`; and `numpy`/`pandas`/`matplotlib`/`seaborn` for analysis).

Optional extras:

```bash
pip install -e ".[dev]"     # pytest, ruff, mypy (for contributors)
pip install -e ".[local]"   # transformers + vllm (only for self-hosted/local models)
```

Provider credentials are read from environment variables and are only needed at query time
(no key is required to import the package or run the offline test suite):

| Provider | Env var |
|----------|---------|
| OpenAI | `OPENAI_API_KEY` |
| Anthropic | `ANTHROPIC_API_KEY` |
| Google Gemini | `GOOGLE_API_KEY` (or ADC) |
| AWS Bedrock | standard AWS credentials (`boto3`) |
| Groq | `GROQ_API_KEY` |
| DeepSeek (direct) | `DEEPSEEK_API_KEY` |

## External planning tools

Evaluation and instance generation rely on classical planners and a validator, located via
environment variables:

| Tool | Env var | Used for | Vendored? |
|------|---------|----------|-----------|
| [VAL](https://github.com/KCL-Planning/VAL) | `VAL` | Sound plan validation (`evaluate_plan`, backprompting) | `planner_tools/VAL/` (build for your OS) |
| [Fast Downward](https://www.fast-downward.org/) | `FAST_DOWNWARD` | Computing optimal/reference plans | No — install separately |
| [PR2](http://www.fast-downward.org/) | `PR2` | Plan-reuse / replanning experiments | `planner_tools/PR2/` |

```bash
export VAL=/path/to/VAL/build/.../bin          # dir containing the `validate` binary
export FAST_DOWNWARD=/path/to/downward          # dir containing `fast-downward.py`
```

## Quickstart

> **Working-directory convention.** The pipeline resolves all data paths *relative to the current
> working directory* (`./configs`, `./instances`, `./responses`, `./results`). All bundled data
> lives under `data/`, so **run `planbench` commands from inside `data/`.**

```bash
cd data

# 1) Generate prompts for one-shot plan generation on Blocksworld, instances 1–5
planbench prompt --task t1 --config blocksworld --engine gpt-4o_chat --specific_instances 1 2 3 4 5

# 2) Run the full pipeline (prompt → query the model → evaluate) for the same task
export OPENAI_API_KEY=sk-...
planbench run --task t1 --config blocksworld --engine gpt-4o_chat --specific_instances 1 2 3 4 5

# 3) Evaluate only (e.g. re-score an existing response file with rule-based extraction)
planbench evaluate --task t1 --config blocksworld --engine gpt-4o_chat --no_llm_based_extraction
```

Run `planbench --help` (or `planbench <subcommand> --help`) for the full set of options. The seven
subcommands are: `run`, `prompt`, `evaluate`, `generate`, `obfuscate`, `stats`, `backprompt`.

Outputs are written, relative to `data/`, to `prompts/<domain>/`, `responses/<domain>/<engine>/`,
and `results/<domain>/<engine>/`.

## The tasks

15 tasks are wired into the pipeline (task keys passed via `--task`). See
[docs/TASK_REGISTRY.md](docs/TASK_REGISTRY.md) for the full canonical registry.

| Key | Task | Evaluation |
|-----|------|------------|
| `t1` | Plan generation (one-shot) | `evaluate_plan` |
| `t1_zero` | Plan generation (zero-shot) | `evaluate_plan` |
| `t1_cot` | Plan generation (CoT / state-tracking) | `evaluate_plan` |
| `t1_pddl` | Plan generation, PDDL output (one-shot) | `evaluate_plan_pddl` |
| `t1_zero_pddl` | Plan generation, PDDL output (zero-shot) | `evaluate_plan_pddl` |
| `t2` | Optimal planning (cost-aware) | `evaluate_plan` |
| `t3` | Plan verification | `evaluate_verification` |
| `t3_1` | Plan verification with LLM-generated plans | `evaluate_verification` |
| `t4` | Plan reuse | `evaluate_plan` |
| `t5` | Plan generalization | `evaluate_plan` |
| `t6` | Replanning | `evaluate_plan` |
| `t7` | Reasoning about plan execution (state tracking) | `evaluate_state` |
| `t8_1` | Goal reformulation — goal shuffling | `evaluate_plan` |
| `t8_2` | Goal reformulation — full → partial | `evaluate_plan` |
| `t8_3` | Goal reformulation — partial → full | `evaluate_plan` |

> `t3_zero` (zero-shot verification) appears in the canonical registry but is **not yet wired** into
> the pipeline. Contributions welcome.

## Supported domains

Each domain is described by a config in `data/configs/` and PDDL instances in `data/instances/`:

| Domain | Config | Notes |
|--------|--------|-------|
| Blocksworld | `blocksworld.yaml` | The primary domain |
| Blocksworld (hard) | `blocksworld_hard.yaml` | Larger/harder instances |
| Mystery Blocksworld | `mystery_blocksworld.yaml` | Semantically obfuscated blocksworld |
| Logistics | `logistics.yaml` | Trucks/airplanes/packages |
| Depots | `depots.yaml` | Blocksworld × logistics hybrid |
| Sokoban | `sokoban.yaml` | Grid puzzle |
| Obfuscated (randomized / deceptive) | `obfuscated_*_logistics.yaml`, … | Tests reliance on world knowledge |
| Unsolvable variants | `unsolvable_*.yaml` | Models should detect infeasibility |

To add your own domain, see **[docs/ADDING_A_DOMAIN.md](docs/ADDING_A_DOMAIN.md)**.

## Supported model engines

The engine string passed to `--engine` selects the provider via `planbench.llm.get_client`:

| Engine pattern | Provider | Examples |
|----------------|----------|----------|
| `*_chat` | OpenAI chat | `gpt-4o_chat`, `o1-mini_chat` |
| `finetuned`, *(other)* | OpenAI completion / fine-tuned | `finetuned` |
| `deepseek-r1` | DeepSeek (direct API) | `deepseek-r1` |
| `claude-3-opus`, `claude-3.5-sonnet` | Anthropic | `claude-3.5-sonnet` |
| `*_aws` | AWS Bedrock (Claude, LLaMA) | `claude-3-sonnet_aws`, `llama-3.1-405b_aws` |
| `*_groq` | Groq | `llama-3.1-70b_groq` |
| `palm`, `gemini*` | Google | `gemini-1.5-pro`, `gemini-2.5-thinking` |
| `bloom`, `*_local`, `qwq*`, `qwen*` | Local (Transformers/vLLM) | `bloom`, `deepseek-r1_local`, `qwq-32b` |

## Project layout

```
src/planbench/
  config.py          # DomainConfig: load/validate domain config YAML
  cli.py             # `planbench` CLI (7 subcommands)
  pipeline.py        # prompt → response → evaluate orchestration
  model_parser/      # PDDL ↔ internal model (parser, writer) via tarski
  translation/       # PDDL ↔ natural language (pddl_to_text, text_to_pddl)
  task_utils/        # per-task prompt construction helpers
  prompts/           # PromptGenerator — builds prompts for all tasks
  llm/               # provider clients + get_client() factory
  response/          # ResponseGenerator — queries models (parallel-capable)
  evaluation/        # ResponseEvaluator + VAL-based validator
  backprompting/     # iterative refinement (sequential + parallel)
  generators/        # PDDL instance generators per domain
  obfuscation/       # domain/instance name obfuscation
  analysis/          # stats + plotting

data/                # configs, instances, PDDL generators (the working dir)
planner_tools/       # VAL, PR2
tests/               # offline pytest suite
docs/                # architecture, adding-a-domain, migration, task registry
```

See [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) for how data flows through the pipeline.

## Documentation

- [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) — system architecture and data flow
- [docs/ADDING_A_DOMAIN.md](docs/ADDING_A_DOMAIN.md) — add a new PDDL domain to test LLMs
- [CONTRIBUTING.md](CONTRIBUTING.md) — dev setup, tests, code style, PR process
- [docs/MIGRATION.md](docs/MIGRATION.md) — migrating from `plan-bench/` / `llm_planning_analysis/`
- [docs/TASK_REGISTRY.md](docs/TASK_REGISTRY.md) — full task catalog

---

# PlanBench Static Test Set Leaderboard

The leaderboard below shows model performance on the PlanBench static test set with zero-shot
prompting. See `results/` for detailed files; Blocksworld Hard results are under
`results/backprompting/`.

| Model Name | Model Type | Blocksworld - NL - 600 instances | Mystery Blocksworld - NL - 600 instances | Randomized Mystery Blocksworld - NL - 600 instances | Blocksworld Hard - PDDL - 110 instances |
|------------|------------|----------------------------------|----------------------------------------|--------------------------------------------------|----------------------------------------|
| Deepseek R1 | LRM | 99.1% | 43.3% | 25.8% | 53.6% |
| o1-preview | LRM | 97.8% | 52.8% | 37.3% | 23.65% |
| o1-mini | LRM | 56.6% | 19.1% | 3.5% | 10% |
| Claude-3.5 Sonnet | LLM | 54.8% | 0% | - | - |
| GPT-4o | LLM | 35.5% | 0% | - | - |
| LLaMA-3.1 405B | LLM | 62.6% | 0.8% | - | - |
| Claude 3 Opus | LLM | 59.3% | 0% | - | - |
| LLaMA-3 70B | LLM | 34.16% | 0% | - | - |
| GPT-4 | LLM | 34.6% | 0% | - | - |
| Gemini 1.5 Pro | LLM | 23.8% | - | - | - |

> LLM = Large Language Model, LRM = Large Reasoning Model, NL = Natural Language prompting,
> PDDL = PDDL prompting.

### Submitting to the leaderboard

Submit results for new models by opening a pull request with the result file; the leaderboard will
be updated.

## Citations

PlanBench — _NeurIPS 2023 Datasets and Benchmarks Track_:
```
@article{valmeekam2023planbench,
  title={Planbench: An extensible benchmark for evaluating large language models on planning and reasoning about change},
  author={Valmeekam, Karthik and Marquez, Matthew and Olmo, Alberto and Sreedharan, Sarath and Kambhampati, Subbarao},
  journal={Advances in Neural Information Processing Systems},
  volume={36},
  pages={38975--38987},
  year={2023}
}
```

On the Planning Abilities of Large Language Models — _NeurIPS 2023 Spotlight_:
```
@article{valmeekam2023planning,
  title={On the planning abilities of large language models-a critical investigation},
  author={Valmeekam, Karthik and Marquez, Matthew and Sreedharan, Sarath and Kambhampati, Subbarao},
  journal={Advances in Neural Information Processing Systems},
  volume={36},
  pages={75993--76005},
  year={2023}
}
```

A Systematic Evaluation of the Planning and Scheduling Abilities of the Reasoning Model o1 — _TMLR_:
```
@article{valmeekam2025a,
title={A Systematic Evaluation of the Planning and Scheduling Abilities of the Reasoning Model o1},
author={Karthik Valmeekam and Kaya Stechly and Atharva Gundawar and Subbarao Kambhampati},
journal={Transactions on Machine Learning Research},
issn={2835-8856},
year={2025},
url={https://openreview.net/forum?id=FkKBxp0FhR},
note={}
}
```

## Star History

[![Star History Chart](https://api.star-history.com/svg?repos=karthikv792/LLMs-Planning&type=Date)](https://www.star-history.com/#karthikv792/LLMs-Planning&Date)

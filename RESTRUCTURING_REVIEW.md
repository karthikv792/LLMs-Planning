---
name: Restructuring Plan Review
overview: Review of the RESTRUCTURING.md plan against the actual codebases, identifying factual errors, missing items, and inconsistencies with the existing READMEs.
todos:
  - id: fix-task-count
    content: Fix '13 tasks' to '14 tasks' (or 15 with t3_1) throughout RESTRUCTURING.md
    status: pending
  - id: fix-obfuscator
    content: "Correct the merge strategy table: obfuscator.py exists in BOTH codebases, not just llm_planning_analysis"
    status: pending
  - id: add-t3_1
    content: Add t3_1 (plan_verification_with_llm_plans) to the canonical task registry or explicitly note its deprecation
    status: pending
  - id: fix-sokoban-claim
    content: Correct the problem_generators.py merge notes to reflect that sokoban code exists in both codebases
    status: pending
  - id: add-missing-files
    content: Add back_prompting_parallel.py, promptgen.sh, results_backprompting/, config_mask.yaml, default_template.yaml to the plan
    status: pending
  - id: add-data-migration
    content: Add a strategy for historical prompts/, responses/, results/ data and the root README leaderboard path
    status: pending
  - id: update-docs-notes
    content: "Expand Phase 10 notes to cover: Python version bump, engine reference, leaderboard paths, planner_tools env vars"
    status: pending
isProject: false
---

# Restructuring Plan Review

## Overall Assessment

The restructuring plan in [RESTRUCTURING.md](llm_planning_analysis/RESTRUCTURING.md) is **well-structured and thorough**, with a clear phased approach, good design decisions (DomainConfig dataclass, LLMClient protocol, CLI entry points, src-layout packaging), and a comprehensive merge strategy. However, there are several **factual inaccuracies and missing items** that should be corrected before execution.

---

## Factual Errors to Fix

### 1. Task count: "13 tasks" should be "14 tasks"

The plan says "all 13 tasks" in multiple places (Phases 0, 5, and 8), but the canonical task registry in Phase 0 actually lists **14 distinct tasks**:

- 5 from `llm_planning_analysis`: t1, t1_zero, t1_cot, t1_pddl, t1_zero_pddl
- 10 from `plan-bench`: t1, t2, t3, t4, t5, t6, t7, t8_1, t8_2, t8_3
- t1 overlaps, so: 5 + 10 - 1 = **14 unique tasks**

Every occurrence of "13 tasks" needs to become "14 tasks" (or "15" if t3_1 is also included -- see below).

### 2. Missing task t3_1 from plan-bench

`plan-bench/prompt_generation.py` defines `t3_1` -> `task_3_plan_verification_with_llm_plans`. This task ID is **not listed** in the canonical task registry in Phase 0. It should either be:

- Added as a 15th task, or
- Explicitly noted as deprecated/dropped with justification

### 3. `obfuscator.py` is NOT unique to `llm_planning_analysis`

The merge strategy table states:

> `obfuscator.py` | llm_planning_analysis | Only exists in llm_planning_analysis

This is **wrong**. Both codebases have `obfuscator.py`, and both READMEs document its usage with identical command-line interfaces. The merge strategy should say "exists in both; diff and take the more complete version" or "identical; use either."

### 4. `problem_generators.py` sokoban claim is misleading

The comparison table implies sokoban support is unique to `llm_planning_analysis`, and the merge strategy says "plan-bench has depots generator; llm_planning_analysis has sokoban." However, the exploration found that **both** copies of `problem_generators.py` contain sokoban code in `Instance_Generator`. The actual difference is:

- `llm_planning_analysis` has sokoban **configs + instances** (plan-bench does not)
- The generator code itself exists in both

The merge strategy for `problem_generators.py` should reflect the actual code differences rather than implying one has sokoban and the other doesn't.

---

## Missing Items

### 5. `back_prompting_parallel.py` not accounted for

`llm_planning_analysis/back_prompting_parallel.py` exists but is not mentioned anywhere in the restructuring plan. It should be either:

- Merged into `backprompting/backprompter.py` alongside `back_prompting.py`
- Explicitly listed for deletion in Phase 9

### 6. `promptgen.sh` not mentioned

`llm_planning_analysis/promptgen.sh` (a shell script for batch prompt generation) is not mentioned in the cleanup phase. It should be listed for deletion or migration.

### 7. `results_backprompting/` directory not addressed

`llm_planning_analysis/results_backprompting/` contains backprompting experiment results across multiple domains. The plan discusses `results/` but never mentions this separate directory. Should be noted in Phase 1 (data migration) or Phase 9 (cleanup).

### 8. `config_mask.yaml` and `default_template.yaml` not addressed

Among the 15 configs in `llm_planning_analysis`, two are non-standard: `config_mask.yaml` and `default_template.yaml`. These don't follow the same pattern as domain configs. The merge strategy should clarify whether they should be included in `data/configs/` or treated as development artifacts.

### 9. Historical data migration strategy missing

Both codebases have `prompts/`, `responses/`, and `results/` directories containing actual experiment outputs. The plan doesn't discuss whether these are preserved (e.g., moved to a `legacy/` directory), gitignored going forward, or deleted. The root README includes a leaderboard that references `llm_planning_analysis/results/` -- this path would break after restructuring.

---

## Inconsistencies with READMEs

### 10. Root README leaderboard path breakage

[README.md](README.md) line 11 says:

> Check out llm_planning_analysis/results/ folder for the detailed files.

After the merge, this path will no longer exist. Phase 10 (Documentation) should explicitly note updating the leaderboard section and any file path references.

### 11. Python version jump

Both subdirectory READMEs say "Python 3.6+" while the restructuring targets `>=3.10`. This is fine for the restructuring (modern type hints require 3.10+), but the Phase 10 documentation update should mention this as a breaking change for users on older Python versions.

### 12. LLM engine documentation outdated

Both READMEs only document OpenAI and BLOOM engines, but `llm_planning_analysis` already supports Gemini, Claude, Bedrock, DeepSeek, Groq, QwQ, and local models. Phase 10 should create a comprehensive engine reference that reflects the actual current capabilities, not just the legacy README content.

---

## Minor Suggestions

### 13. `data/` directory placement

The plan puts `data/` at the project root alongside `src/`. For a pip-installable package, consider whether configs/instances should be **package data** (inside `src/planbench/data/`) for portability, or if external `data/` with a configurable path is intentional. The current plan's `workspace_root` approach (Key Decision 4) seems to favor external data, which is fine but should be explicit about how `pip install planbench` users will access the default data.

### 14. `planner_tools/` not discussed

The restructuring plan says `planner_tools/` is "untouched" in the directory structure, but both READMEs reference it for VAL and PR2. The plan should clarify that environment variables (`FAST_DOWNWARD`, `VAL`, `PR2`) remain the configuration mechanism for external planners.

### 15. The comparison table says plan-bench has "~4285 PDDL files"

The exploration found ~4,430+ PDDL files, and llm_planning_analysis has more domains than stated. These counts are approximate, which is fine, but worth noting they may be slightly off.
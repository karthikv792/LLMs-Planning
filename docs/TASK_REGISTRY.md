# Task Registry

All 16 canonical PlanBench tasks across both codebases (`llm_planning_analysis` and `plan-bench`).

## Tasks

| ID | Name | Source | Prompt Method | Evaluation Method | Supported Domains |
|----|------|--------|---------------|-------------------|-------------------|
| t1 | Plan Generation (one-shot) | Both | `PromptGenerator.task_1_plan_generation()` | `evaluate_plan()` | blocksworld, mystery_blocksworld, logistics, depots, obfuscated |
| t1_zero | Plan Generation (zero-shot) | llm_planning_analysis | `PromptGenerator.task_1_plan_generation_zero_shot()` | `evaluate_plan()` | blocksworld, mystery_blocksworld, logistics, depots, obfuscated |
| t1_cot | Plan Generation (CoT/state-tracking) | llm_planning_analysis | `PromptGenerator.task_1_plan_generation_state_tracking()` | `evaluate_plan()` | blocksworld, mystery_blocksworld, logistics, depots |
| t1_pddl | Plan Generation PDDL (one-shot) | llm_planning_analysis | `PromptGenerator.task_1_plan_generation_pddl()` | `evaluate_plan_pddl()` | blocksworld, mystery_blocksworld, logistics, depots, obfuscated |
| t1_zero_pddl | Plan Generation PDDL (zero-shot) | llm_planning_analysis | `PromptGenerator.task_1_plan_generation_zero_shot_pddl()` | `evaluate_plan_pddl()` | blocksworld, mystery_blocksworld, logistics, depots, obfuscated |
| t2 | Optimal Planning | plan-bench | `task_2_plan_optimality()` | `evaluate_plan()` | blocksworld, logistics |
| t3 | Plan Verification | plan-bench | `task_3_plan_verification()` | `evaluate_verification()` | blocksworld, mystery_blocksworld, logistics |
| t3_1 | Plan Verification (LLM plans) | plan-bench | `task_3_plan_verification_with_llm_plans()` | `evaluate_verification()` | blocksworld, mystery_blocksworld, logistics |
| t3_zero | Plan Verification (zero-shot) | plan-bench | `task_3_zero_shot_plan_verification()` | `evaluate_verification()` | blocksworld, mystery_blocksworld, logistics |
| t4 | Plan Reuse | plan-bench | `task_4_plan_reuse()` | `evaluate_plan()` | blocksworld, mystery_blocksworld, logistics |
| t5 | Plan Generalization | plan-bench | `task_5_plan_generalization()` | `evaluate_plan()` | blocksworld, logistics |
| t6 | Replanning | plan-bench | `task_6_replanning()` | `evaluate_plan()` | blocksworld, mystery_blocksworld, logistics |
| t7 | Reasoning about Plan Execution | plan-bench | `task_7_plan_execution()` | `evaluate_state()` | blocksworld, mystery_blocksworld, logistics |
| t8_1 | Goal Reformulation (shuffling) | plan-bench | `task_8_1_goal_shuffling()` | `evaluate_plan()` | blocksworld, mystery_blocksworld, logistics |
| t8_2 | Goal Reformulation (full-to-partial) | plan-bench | `task_8_2_full_to_partial()` | `evaluate_plan()` | blocksworld, mystery_blocksworld, logistics |
| t8_3 | Goal Reformulation (partial-to-full) | plan-bench | `task_8_3_partial_to_full()` | `evaluate_plan()` | blocksworld, mystery_blocksworld, logistics |

## Evaluation Methods

| Method | File (llm_planning_analysis) | File (plan-bench) | Description |
|--------|------------------------------|-------------------|-------------|
| `evaluate_plan()` | `response_evaluation.py:156` | `response_evaluation.py:74` | Validates plan via text-to-PDDL + VAL |
| `evaluate_plan_parallel()` | `response_evaluation.py:101` | -- | Parallel plan validation with LLM-based extraction |
| `evaluate_plan_pddl()` | `response_evaluation.py:201` | -- | Validates raw PDDL plan output |
| `evaluate_state()` | -- | `response_evaluation.py:139` | Validates state prediction (t7) |
| `evaluate_verification()` | -- | `response_evaluation.py:229` | Validates verification response (t3) |

## Notes

- Tasks t1 through t1_zero_pddl exist in `llm_planning_analysis/prompt_generation.py`
- Tasks t2 through t8_3 exist in `plan-bench/prompt_generation.py`
- Both forks share the same t1 (one-shot plan generation) implementation
- The `llm_planning_analysis` fork added zero-shot, CoT, and PDDL prompt variants
- The `plan-bench` fork added verification, reuse, generalization, replanning, execution, and goal reformulation tasks
- Depots domain support was backported to `llm_planning_analysis` in Phase 0

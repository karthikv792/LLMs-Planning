"""Unified pipeline orchestrator for PlanBench.

Runs the full prompt-generation → response-generation → response-evaluation
pipeline for any of the 16 supported tasks.

Replaces both ``plan-bench/llm_plan_pipeline.py`` and
``llm_planning_analysis/llm_plan_pipeline.py``.
"""

from __future__ import annotations

import argparse
import os
import random

from planbench.prompts import PromptGenerator
from planbench.response import ResponseGenerator
from planbench.evaluation import ResponseEvaluator


# ── task routing tables ──────────────────────────────────────────────────

# Tasks that use evaluate_plan
EVAL_PLAN_TASKS: dict[str, str] = {
    't1': 'task_1_plan_generation',
    't1_zero': 'task_1_plan_generation_zero_shot',
    't1_cot': 'task_1_plan_generation_state_tracking',
    't2': 'task_2_plan_optimality',
    't4': 'task_4_plan_reuse',
    't5': 'task_5_plan_generalization',
    't6': 'task_6_replanning',
    't8_1': 'task_8_1_goal_shuffling',
    't8_2': 'task_8_2_full_to_partial',
    't8_3': 'task_8_3_partial_to_full',
}

# Tasks that use evaluate_plan_pddl
EVAL_PLAN_PDDL_TASKS: dict[str, str] = {
    't1_pddl': 'task_1_plan_generation_pddl',
    't1_zero_pddl': 'task_1_plan_generation_zero_shot_pddl',
}

# Tasks that use evaluate_state
EVAL_STATE_TASKS: dict[str, str] = {
    't7': 'task_7_plan_execution',
}

# Tasks that use evaluate_verification
EVAL_VERIFICATION_TASKS: dict[str, str] = {
    't3': 'task_3_plan_verification',
    't3_1': 'task_3_plan_verification_with_llm_plans',
}

# Complete task name mapping for response generation
ALL_TASKS: dict[str, str] = {
    **EVAL_PLAN_TASKS,
    **EVAL_PLAN_PDDL_TASKS,
    **EVAL_STATE_TASKS,
    **EVAL_VERIFICATION_TASKS,
}

# Tasks supporting LLM-based parallel extraction
PARALLEL_EXTRACT_TASKS = {'t1', 't1_zero', 't1_cot'}


def run_pipeline(
    task: str,
    config_file: str,
    engine: str,
    *,
    verbose: bool = False,
    ignore_existing: bool = False,
    specified_instances: list[int] | None = None,
    random_example: bool = False,
    seed: int = 42,
    run_till_completion: bool = False,
    max_workers: int = 1,
    translator_engine: str = "gpt-4o",
    no_llm_based_extraction: bool = False,
) -> None:
    """Execute the full pipeline for a given task.

    Args:
        task: Task key (e.g. 't1', 't1_zero_pddl', 't3').
        config_file: Path to domain config YAML.
        engine: LLM engine string.
        verbose: Print detailed output.
        ignore_existing: Re-run even if outputs exist.
        specified_instances: Only process these instance IDs.
        random_example: Use random examples instead of sequential.
        seed: Random seed.
        run_till_completion: Retry failed responses.
        max_workers: Thread pool size for response generation.
        translator_engine: Engine for LLM-based plan extraction.
        no_llm_based_extraction: Use rule-based extraction instead of LLM.
    """
    specified_instances = specified_instances or []

    if task not in ALL_TASKS:
        raise ValueError(
            f"Unknown task '{task}'. Valid tasks: {sorted(ALL_TASKS.keys())}"
        )
    task_name = ALL_TASKS[task]

    # ── Stage 1: Prompt Generation ──
    prompt_gen = PromptGenerator(config_file, verbose, ignore_existing, seed)
    _dispatch_prompt_generation(prompt_gen, task, specified_instances, random_example)

    # ── Stage 2: Response Generation ──
    response_gen = ResponseGenerator(config_file, engine, verbose, ignore_existing)
    response_gen.get_responses(
        task_name, max_workers, specified_instances,
        run_till_completion=run_till_completion,
    )

    # ── Stage 3: Response Evaluation ──
    evaluator = ResponseEvaluator(
        config_file, engine, specified_instances, verbose,
        ignore_existing, translator_engine,
    )

    if task in EVAL_PLAN_TASKS:
        if task in PARALLEL_EXTRACT_TASKS and not no_llm_based_extraction:
            evaluator.evaluate_plan_parallel(task_name)
        else:
            evaluator.evaluate_plan(task_name)
    elif task in EVAL_PLAN_PDDL_TASKS:
        evaluator.evaluate_plan_pddl(task_name)
    elif task in EVAL_STATE_TASKS:
        evaluator.evaluate_state(task_name)
    elif task in EVAL_VERIFICATION_TASKS:
        evaluator.evaluate_verification(task_name)


def _dispatch_prompt_generation(
    pg: PromptGenerator,
    task: str,
    specified_instances: list[int],
    random_example: bool,
) -> None:
    """Route to the correct prompt generation method."""
    if task == 't1':
        pg.task_1_plan_generation(specified_instances, random_example)
    elif task == 't1_zero':
        pg.task_1_plan_generation_zero_shot(specified_instances, random_example)
    elif task == 't1_cot':
        pg.task_1_plan_generation_state_tracking(specified_instances, random_example)
    elif task == 't1_pddl':
        pg.task_1_plan_generation_pddl(specified_instances, random_example)
    elif task == 't1_zero_pddl':
        pg.task_1_plan_generation_zero_shot_pddl(specified_instances, random_example)
    elif task == 't2':
        pg.task_2_plan_optimality(specified_instances, random_example)
    elif task == 't3':
        pg.task_3_plan_verification(specified_instances)
    elif task == 't3_1':
        pg.task_3_plan_verification_with_llm_plans(specified_instances)
    elif task == 't4':
        pg.task_4_plan_reuse(specified_instances)
    elif task == 't5':
        pg.task_5_plan_generalization(specified_instances, random_example)
    elif task == 't6':
        pg.task_6_replanning(specified_instances, random_example)
    elif task == 't7':
        pg.task_7_plan_execution(specified_instances, random_example)
    elif task == 't8_1':
        pg.task_8_1_goal_shuffling(specified_instances)
    elif task == 't8_2':
        pg.task_8_2_full_to_partial(specified_instances)
    elif task == 't8_3':
        pg.task_8_3_partial_to_full(specified_instances)


def main() -> None:
    """Legacy CLI entry point.  Prefer ``planbench run`` via ``planbench.cli``."""
    import warnings
    warnings.warn(
        "pipeline.main() is deprecated. Use `planbench run` instead.",
        DeprecationWarning,
        stacklevel=2,
    )

    parser = argparse.ArgumentParser(description="PlanBench Pipeline (legacy)")
    parser.add_argument('--task', type=str, required=True,
                        help=f"Task key: {', '.join(sorted(ALL_TASKS.keys()))}")
    parser.add_argument('--config', type=str, required=True,
                        help='Config file name (no .yaml extension)')
    parser.add_argument('--engine', type=str, required=True,
                        help='LLM engine string')
    parser.add_argument('--verbose', action='store_true')
    parser.add_argument('--ignore_existing', action='store_true')
    parser.add_argument('--specific_instances', nargs='+', type=int, default=[])
    parser.add_argument('--random_example', action='store_true')
    parser.add_argument('--seed', type=int, default=42)
    parser.add_argument('--run_till_completion', action='store_true')
    parser.add_argument('--max_workers', type=int, default=1)
    parser.add_argument('--translator_engine', type=str, default="gpt-4o")
    parser.add_argument('--no_llm_based_extraction', action='store_true')

    args = parser.parse_args()
    random.seed(args.seed)

    config_file = f'./configs/{args.config}.yaml'
    assert os.path.exists(config_file), f"Config file {config_file} does not exist"

    run_pipeline(
        task=args.task,
        config_file=config_file,
        engine=args.engine,
        verbose=args.verbose,
        ignore_existing=args.ignore_existing,
        specified_instances=args.specific_instances,
        random_example=args.random_example,
        seed=args.seed,
        run_till_completion=args.run_till_completion,
        max_workers=args.max_workers,
        translator_engine=args.translator_engine,
        no_llm_based_extraction=args.no_llm_based_extraction,
    )


if __name__ == "__main__":
    main()

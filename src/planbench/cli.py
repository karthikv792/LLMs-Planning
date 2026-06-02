"""Unified CLI for PlanBench.

Subcommands
-----------
* ``planbench run``        — full pipeline (prompt → response → evaluate)
* ``planbench prompt``     — prompt generation only
* ``planbench evaluate``   — response evaluation only
* ``planbench generate``   — PDDL instance generation
* ``planbench obfuscate``  — domain / instance obfuscation
* ``planbench stats``      — result analysis and graph generation
* ``planbench backprompt`` — iterative plan refinement via verifier feedback

Replaces the ``eval(args.verbose)`` anti-pattern with proper
``action="store_true"`` flags throughout.
"""

from __future__ import annotations

import argparse
import os
import random
import sys

from planbench.pipeline import ALL_TASKS, run_pipeline


# ── Shared argument helpers ──────────────────────────────────────────────

def _add_common_args(parser: argparse.ArgumentParser) -> None:
    """Add arguments shared across most subcommands."""
    parser.add_argument(
        "--config", type=str, required=True,
        help="Config file name (without .yaml extension)",
    )
    parser.add_argument(
        "--engine", type=str, required=True,
        help="LLM engine string (e.g. gpt-4o_chat, claude-3.5-sonnet_aws)",
    )
    parser.add_argument("--verbose", action="store_true", help="Verbose output")
    parser.add_argument(
        "--ignore_existing", action="store_true",
        help="Re-run even if outputs already exist",
    )
    parser.add_argument("--seed", type=int, default=42, help="Random seed")


def _add_instance_args(parser: argparse.ArgumentParser) -> None:
    """Add instance-selection arguments."""
    parser.add_argument(
        "--specific_instances", nargs="+", type=int, default=[],
        help="Only process these instance IDs",
    )
    parser.add_argument(
        "--random_example", action="store_true",
        help="Use random examples instead of sequential",
    )


def _resolve_config(config_name: str) -> str:
    """Resolve config name to file path, checking existence."""
    config_file = f"./configs/{config_name}.yaml"
    if not os.path.exists(config_file):
        print(f"Error: Config file {config_file} does not exist", file=sys.stderr)
        sys.exit(1)
    return config_file


# ── Subcommand: run ──────────────────────────────────────────────────────

def _cmd_run(args: argparse.Namespace) -> None:
    """Full pipeline: prompt → response → evaluate."""
    random.seed(args.seed)
    config_file = _resolve_config(args.config)
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


# ── Subcommand: prompt ──────────────────────────────────────────────────

def _cmd_prompt(args: argparse.Namespace) -> None:
    """Prompt generation only."""
    from planbench.prompts import PromptGenerator
    from planbench.pipeline import _dispatch_prompt_generation

    random.seed(args.seed)
    config_file = _resolve_config(args.config)
    pg = PromptGenerator(config_file, args.verbose, args.ignore_existing, args.seed)
    _dispatch_prompt_generation(
        pg, args.task, args.specific_instances, args.random_example,
    )


# ── Subcommand: evaluate ────────────────────────────────────────────────

def _cmd_evaluate(args: argparse.Namespace) -> None:
    """Response evaluation only."""
    from planbench.evaluation import ResponseEvaluator
    from planbench.pipeline import (
        EVAL_PLAN_TASKS, EVAL_PLAN_PDDL_TASKS,
        EVAL_STATE_TASKS, EVAL_VERIFICATION_TASKS,
        PARALLEL_EXTRACT_TASKS,
    )

    random.seed(args.seed)
    config_file = _resolve_config(args.config)
    task = args.task
    if task not in ALL_TASKS:
        print(f"Unknown task '{task}'. Valid: {sorted(ALL_TASKS.keys())}", file=sys.stderr)
        sys.exit(1)
    task_name = ALL_TASKS[task]

    evaluator = ResponseEvaluator(
        config_file, args.engine, args.specific_instances,
        args.verbose, args.ignore_existing, args.translator_engine,
    )

    if task in EVAL_PLAN_TASKS:
        if task in PARALLEL_EXTRACT_TASKS and not args.no_llm_based_extraction:
            evaluator.evaluate_plan_parallel(task_name)
        else:
            evaluator.evaluate_plan(task_name)
    elif task in EVAL_PLAN_PDDL_TASKS:
        evaluator.evaluate_plan_pddl(task_name)
    elif task in EVAL_STATE_TASKS:
        evaluator.evaluate_state(task_name)
    elif task in EVAL_VERIFICATION_TASKS:
        evaluator.evaluate_verification(task_name)


# ── Subcommand: generate ────────────────────────────────────────────────

def _cmd_generate(args: argparse.Namespace) -> None:
    """PDDL instance generation."""
    random.seed(args.seed)
    config_file = _resolve_config(args.config)

    if args.generalization:
        from planbench.generators import (
            BlocksworldGeneralizationGenerator,
            LogisticsGeneralizationGenerator,
        )
        import yaml
        with open(config_file, "r") as f:
            data = yaml.safe_load(f)
        domain = data["domain_name"]
        if domain == "blocksworld":
            gen = BlocksworldGeneralizationGenerator(config_file)
        elif domain == "logistics":
            gen = LogisticsGeneralizationGenerator(config_file)
        else:
            print(f"No generalization generator for domain '{domain}'", file=sys.stderr)
            sys.exit(1)
        gen.t5_gen_generalization_instances(args.n_instances)
    else:
        from planbench.generators import (
            BlocksworldInstanceGenerator,
            LogisticsInstanceGenerator,
            SokobanInstanceGenerator,
        )
        import yaml
        with open(config_file, "r") as f:
            data = yaml.safe_load(f)
        domain = data["domain_name"]
        if domain == "blocksworld":
            gen = BlocksworldInstanceGenerator(config_file, seed=args.seed)
        elif domain == "logistics":
            gen = LogisticsInstanceGenerator(config_file, seed=args.seed)
        elif domain == "sokoban":
            gen = SokobanInstanceGenerator(config_file, seed=args.seed)
        else:
            print(f"No instance generator for domain '{domain}'", file=sys.stderr)
            sys.exit(1)
        gen.gen_goal_directed_instances(args.n_instances, args.max_blocks)


# ── Subcommand: obfuscate ───────────────────────────────────────────────

def _cmd_obfuscate(args: argparse.Namespace) -> None:
    """Domain and instance obfuscation."""
    import string
    import yaml
    from planbench.obfuscation import (
        ALL_WORDS,
        random_mapping,
        translate_domain_pddl,
        create_new_config,
        create_obfuscated_instances,
    )

    random.seed(args.seed)
    config_file = _resolve_config(args.config)
    with open(config_file, "r") as f:
        config = yaml.safe_load(f)

    # Build vocabulary
    if args.obfuscation_type == "randomized":
        obf_type = "randomized"
        while len(ALL_WORDS) < 100:
            x = "".join(
                random.choices(string.ascii_lowercase, k=1)
                + random.choices(string.ascii_lowercase + string.digits, k=15)
            )
            if x not in ALL_WORDS:
                ALL_WORDS.append(x)
    elif args.obfuscation_type == "all_as":
        obf_type = "all_as"
        if not args.not_random_as:
            while len(ALL_WORDS) < 100:
                y = random.randint(3, 105)
                x = "a" * y
                if x not in ALL_WORDS:
                    ALL_WORDS.append(x)
        else:
            ALL_WORDS.clear()
            ALL_WORDS.extend(["a" * i for i in range(3, 105)])
    else:
        obf_type = "deceptive"
        with open(args.words_file, "r") as f:
            for line in f.readlines():
                if line.strip():
                    ALL_WORDS.append(line.strip())
        if len(ALL_WORDS) < 100:
            print("Error: Not enough words provided (need >= 100)", file=sys.stderr)
            sys.exit(1)

    action_mapping, predicate_mapping = random_mapping(config)

    pddl_subpath = args.instances_pddl_subpath
    if "[DOMAIN]" in pddl_subpath:
        prefix = obf_type
        if args.obfuscation_type == "all_as" and args.not_random_as:
            prefix = obf_type + "_not_random"
        pddl_subpath = pddl_subpath.replace("[DOMAIN]", f"{prefix}_{args.config}")

    new_domain = translate_domain_pddl(
        f"instances/{config['domain_file']}",
        action_mapping,
        predicate_mapping,
        f"instances/{pddl_subpath}",
        "generated_domain.pddl",
    )

    output_filename = args.output or f"configs/obfuscated_{obf_type}_{args.config}.yaml"
    create_new_config(
        config, output_filename,
        action_mapping, predicate_mapping,
        pddl_subpath, new_domain, obf_type,
    )

    # Reload original config for instance creation
    with open(config_file, "r") as f:
        config = yaml.safe_load(f)
    create_obfuscated_instances(config, output_filename, args.generalization, obf_type)

    if args.update_extra:
        input_extra = f"configs/{args.config}_3.yaml"
        output_extra = (
            args.output.replace(".", "_3.") if args.output
            else f"configs/obfuscated_{obf_type}_{args.config}_3.yaml"
        )
        with open(input_extra, "r") as f:
            extra_config = yaml.safe_load(f)
        create_new_config(
            extra_config, output_extra,
            action_mapping, predicate_mapping,
            pddl_subpath, new_domain, obf_type,
        )


# ── Subcommand: stats ───────────────────────────────────────────────────

def _cmd_stats(args: argparse.Namespace) -> None:
    """Result analysis and graph generation."""
    from planbench.analysis import StatsConfig, StatsGenerator

    cfg_kwargs = {}
    if args.results_dir:
        cfg_kwargs["results_dir"] = args.results_dir
    if args.graphs_dir:
        cfg_kwargs["graphs_dir"] = args.graphs_dir
    if args.font_path:
        cfg_kwargs["font_path"] = args.font_path

    config = StatsConfig(**cfg_kwargs)
    gen = StatsGenerator(config)

    graph_type = args.type
    if graph_type == "comparison":
        gen.get_stats_graph()
    elif graph_type == "optimal_correct":
        gen.get_new_graphs_optimal_correct()
    elif graph_type == "logistics":
        gen.get_logistics_graphs()
    elif graph_type == "reasoning_tokens":
        gen.reasoning_tokens_plan_length()
    elif graph_type == "obfuscated_reasoning":
        gen.obfuscated_reasoning_tokens_plan_length()
    elif graph_type == "new_blocksworld":
        gen.new_blocksworld_graphs()
    else:
        print(f"Unknown stats type '{graph_type}'", file=sys.stderr)
        sys.exit(1)


# ── Subcommand: backprompt ───────────────────────────────────────────────

def _cmd_backprompt(args: argparse.Namespace) -> None:
    """Iterative plan refinement via verifier feedback."""
    random.seed(args.seed)
    config_file = _resolve_config(args.config)

    if args.parallel:
        from planbench.backprompting import ParallelBackPrompter

        bp = ParallelBackPrompter(args.engine, args.verbose, args.ignore_existing)
        if args.pddl:
            bp.task_1_plan_generation_backprompting_zero_shot_pddl(
                config_file,
                specified_instances=args.specific_instances or None,
                max_workers=args.max_workers,
                llm_verifier=args.llm_verifier,
                sound_verifier=args.sound_verifier,
                llm_critique=args.llm_critique,
                sound_critique=args.sound_critique,
                history=args.history,
                cot=args.cot,
                t_verifier=args.t_verifier,
                t_generator=args.t_generator,
                feedback_type=args.feedback_type,
                ignore_verifier=args.ignore_verifier,
                iterations=args.iterations,
            )
        else:
            bp.task_1_plan_generation_backprompting(
                config_file,
                specified_instances=args.specific_instances or None,
                random_example=args.random_example,
                max_workers=args.max_workers,
                llm_verifier=args.llm_verifier,
                sound_verifier=args.sound_verifier,
                llm_critique=args.llm_critique,
                sound_critique=args.sound_critique,
                history=args.history,
                cot=args.cot,
                t_verifier=args.t_verifier,
                t_generator=args.t_generator,
                feedback_type=args.feedback_type,
                ignore_verifier=args.ignore_verifier,
                iterations=args.iterations,
            )
    else:
        from planbench.backprompting import SequentialBackPrompter

        use_llm_feedback = {
            "use_llm": args.llm_validation,
            "zero_shot": True,
        }
        bp = SequentialBackPrompter(args.engine, args.verbose, args.ignore_existing)
        bp.task_1_plan_generation_backprompting(
            config_file,
            use_llm_feedback,
            specified_instances=args.specific_instances or None,
            random_example=args.random_example,
        )


# ── Top-level parser ─────────────────────────────────────────────────────

def build_parser() -> argparse.ArgumentParser:
    """Build the full CLI argument parser with all subcommands."""
    parser = argparse.ArgumentParser(
        prog="planbench",
        description="PlanBench — Benchmark suite for evaluating LLM planning capabilities",
    )
    subparsers = parser.add_subparsers(dest="command", help="Available subcommands")

    # ── run ──
    p_run = subparsers.add_parser("run", help="Full pipeline (prompt → response → evaluate)")
    _add_common_args(p_run)
    _add_instance_args(p_run)
    p_run.add_argument(
        "--task", type=str, required=True,
        help=f"Task key: {', '.join(sorted(ALL_TASKS.keys()))}",
    )
    p_run.add_argument("--run_till_completion", action="store_true",
                       help="Retry failed responses")
    p_run.add_argument("--max_workers", type=int, default=1,
                       help="Thread pool size for response generation")
    p_run.add_argument("--translator_engine", type=str, default="gpt-4o",
                       help="Engine for LLM-based plan extraction")
    p_run.add_argument("--no_llm_based_extraction", action="store_true",
                       help="Use rule-based extraction instead of LLM")
    p_run.set_defaults(func=_cmd_run)

    # ── prompt ──
    p_prompt = subparsers.add_parser("prompt", help="Prompt generation only")
    _add_common_args(p_prompt)
    _add_instance_args(p_prompt)
    p_prompt.add_argument(
        "--task", type=str, required=True,
        help=f"Task key: {', '.join(sorted(ALL_TASKS.keys()))}",
    )
    p_prompt.set_defaults(func=_cmd_prompt)

    # ── evaluate ──
    p_eval = subparsers.add_parser("evaluate", help="Response evaluation only")
    _add_common_args(p_eval)
    _add_instance_args(p_eval)
    p_eval.add_argument(
        "--task", type=str, required=True,
        help=f"Task key: {', '.join(sorted(ALL_TASKS.keys()))}",
    )
    p_eval.add_argument("--translator_engine", type=str, default="gpt-4o",
                        help="Engine for LLM-based plan extraction")
    p_eval.add_argument("--no_llm_based_extraction", action="store_true",
                        help="Use rule-based extraction instead of LLM")
    p_eval.set_defaults(func=_cmd_evaluate)

    # ── generate ──
    p_gen = subparsers.add_parser("generate", help="PDDL instance generation")
    p_gen.add_argument("--config", type=str, required=True,
                       help="Config file name (without .yaml)")
    p_gen.add_argument("--seed", type=int, default=10, help="Random seed")
    p_gen.add_argument("--generalization", action="store_true",
                       help="Generate task-5 generalization instances")
    p_gen.add_argument("--n_instances", type=int, default=0,
                       help="Number of instances to generate (0 = use config default)")
    p_gen.add_argument("--max_blocks", type=int, default=5,
                       help="Max blocks for blocksworld domain")
    p_gen.set_defaults(func=_cmd_generate)

    # ── obfuscate ──
    p_obf = subparsers.add_parser("obfuscate", help="Domain / instance obfuscation")
    p_obf.add_argument("--config", type=str, required=True,
                       help="Config file name (without .yaml)")
    p_obf.add_argument("--seed", type=int, default=0, help="Random seed")
    p_obf.add_argument(
        "--obfuscation_type", type=str, default="deceptive",
        choices=["deceptive", "randomized", "all_as"],
        help="Obfuscation strategy",
    )
    p_obf.add_argument("--output", type=str, default="",
                       help="Output config filename (auto-generated if empty)")
    p_obf.add_argument("--words_file", type=str, default="obfuscate/random_words_1.txt",
                       help="Word list file for deceptive obfuscation")
    p_obf.add_argument("--instances_pddl_subpath", type=str,
                       default="obfuscated_[DOMAIN]",
                       help="PDDL instance subpath ([DOMAIN] is replaced)")
    p_obf.add_argument("--generalization", action="store_true",
                       help="Also obfuscate generalization instances")
    p_obf.add_argument("--not_random_as", action="store_true",
                       help="Use sequential 'a' strings instead of random lengths")
    p_obf.add_argument("--update_extra", action="store_true",
                       help="Also update the _3 extra config")
    p_obf.set_defaults(func=_cmd_obfuscate)

    # ── stats ──
    p_stats = subparsers.add_parser("stats", help="Result analysis and graph generation")
    p_stats.add_argument(
        "--type", type=str, required=True,
        choices=[
            "comparison", "optimal_correct", "logistics",
            "reasoning_tokens", "obfuscated_reasoning", "new_blocksworld",
        ],
        help="Type of analysis graph to generate",
    )
    p_stats.add_argument("--results_dir", type=str, default="",
                         help="Results directory (default: results/)")
    p_stats.add_argument("--graphs_dir", type=str, default="",
                         help="Graphs output directory (default: graphs/)")
    p_stats.add_argument("--font_path", type=str, default="",
                         help="Path to custom font file")
    p_stats.set_defaults(func=_cmd_stats)

    # ── backprompt ──
    p_bp = subparsers.add_parser("backprompt", help="Iterative plan refinement")
    _add_common_args(p_bp)
    _add_instance_args(p_bp)
    p_bp.add_argument("--parallel", action="store_true",
                      help="Use parallel backprompter with ThreadPoolExecutor")
    p_bp.add_argument("--pddl", action="store_true",
                      help="Use PDDL format (parallel mode only)")
    p_bp.add_argument("--max_workers", type=int, default=1,
                      help="Thread pool size (parallel mode)")
    p_bp.add_argument("--llm_validation", action="store_true",
                      help="Use LLM for validation (sequential mode)")
    p_bp.add_argument("--llm_verifier", action="store_true",
                      help="Use LLM verifier (parallel mode)")
    p_bp.add_argument("--sound_verifier", action="store_true",
                      help="Use sound (VAL) verifier (parallel mode)")
    p_bp.add_argument("--llm_critique", action="store_true",
                      help="Use LLM critique (parallel mode)")
    p_bp.add_argument("--sound_critique", action="store_true",
                      help="Use sound critique (parallel mode)")
    p_bp.add_argument("--history", type=int, default=-1,
                      help="History window size (-1 = unlimited)")
    p_bp.add_argument("--cot", action="store_true",
                      help="Enable chain-of-thought prompting")
    p_bp.add_argument("--t_verifier", type=float, default=0.0,
                      help="Verifier temperature")
    p_bp.add_argument("--t_generator", type=float, default=0.0,
                      help="Generator temperature")
    p_bp.add_argument("--feedback_type", type=int, default=1,
                      help="Feedback detail level (0=none, 1=first error, 2=all)")
    p_bp.add_argument("--ignore_verifier", action="store_true",
                      help="Skip verifier checks")
    p_bp.add_argument("--iterations", type=int, default=15,
                      help="Max refinement iterations")
    p_bp.set_defaults(func=_cmd_backprompt)

    return parser


def main(argv: list[str] | None = None) -> None:
    """CLI entry point for ``planbench``."""
    parser = build_parser()
    args = parser.parse_args(argv)

    if not args.command:
        parser.print_help()
        sys.exit(0)

    args.func(args)


if __name__ == "__main__":
    main()

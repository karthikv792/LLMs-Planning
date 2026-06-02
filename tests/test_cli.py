"""Tests for the unified CLI argument parser."""

from __future__ import annotations

import pytest

from planbench.cli import build_parser
from planbench.pipeline import ALL_TASKS


def test_help_exits_zero(capsys):
    parser = build_parser()
    with pytest.raises(SystemExit) as exc:
        parser.parse_args(["--help"])
    assert exc.value.code == 0


def test_run_subcommand_parsing():
    parser = build_parser()
    args = parser.parse_args(
        ["run", "--config", "blocksworld", "--engine", "gpt-4o_chat", "--task", "t1"]
    )
    assert args.command == "run"
    assert args.task == "t1"
    assert args.engine == "gpt-4o_chat"
    # store_true flags default to False (no more eval(args.verbose) anti-pattern)
    assert args.verbose is False
    assert args.ignore_existing is False
    assert callable(args.func)


def test_verbose_flag_is_boolean():
    parser = build_parser()
    args = parser.parse_args(
        ["run", "--config", "bw", "--engine", "e", "--task", "t1", "--verbose"]
    )
    assert args.verbose is True


@pytest.mark.parametrize(
    "argv",
    [
        ["prompt", "--config", "bw", "--engine", "e", "--task", "t1"],
        ["evaluate", "--config", "bw", "--engine", "e", "--task", "t3"],
        ["generate", "--config", "bw"],
        ["obfuscate", "--config", "bw", "--obfuscation_type", "randomized"],
        ["stats", "--type", "comparison"],
        ["backprompt", "--config", "bw", "--engine", "e", "--parallel"],
    ],
)
def test_all_subcommands_parse(argv):
    parser = build_parser()
    args = parser.parse_args(argv)
    assert args.command == argv[0]
    assert callable(args.func)


def test_obfuscation_type_choices_enforced():
    parser = build_parser()
    with pytest.raises(SystemExit):
        parser.parse_args(
            ["obfuscate", "--config", "bw", "--obfuscation_type", "not-a-mode"]
        )


def test_task_table_is_complete():
    # 15 tasks are wired into the pipeline (t3_zero_shot from the canonical
    # registry is defined but not yet implemented — see docs/TASK_REGISTRY.md).
    assert len(ALL_TASKS) == 15
    assert len(ALL_TASKS) == len(set(ALL_TASKS))  # no duplicate keys
    for key in ("t1", "t1_zero", "t1_cot", "t2", "t3", "t3_1", "t7", "t8_1"):
        assert key in ALL_TASKS

"""Tests for response evaluation parsing (offline, no LLM)."""

from __future__ import annotations

import pytest

from planbench.evaluation import ResponseEvaluator
from planbench.evaluation.validator import validate_plan, validate_plan_unsolvable
from conftest import requires_val


@pytest.fixture
def evaluator(in_data_dir):
    # engine string is only used for output paths; no client is constructed here.
    return ResponseEvaluator("configs/blocksworld.yaml", "gpt-4o_chat")


def test_parse_output_valid(evaluator, bw_problem):
    out = evaluator.parse_output(bw_problem.actions, "the above plan is valid")
    assert out.get("valid") is True


def test_parse_output_invalid(evaluator, bw_problem):
    out = evaluator.parse_output(bw_problem.actions, "the above plan is invalid")
    assert out.get("valid") is False


def test_parse_output_unmet_goal(evaluator, bw_problem):
    text = (
        "The above plan is invalid.\n"
        "The following facts are an unmet goal:\n"
        "the red block is clear"
    )
    out = evaluator.parse_output(bw_problem.actions, text)
    assert out.get("valid") is False
    assert "unmet_goal" in out


# ── plan validation ──────────────────────────────────────────────────────

def test_validate_plan_unsolvable():
    assert validate_plan_unsolvable("there is no plan possible here") is True
    assert validate_plan_unsolvable("(pick-up a)\n(stack a b)") is False


@requires_val
def test_validate_plan_with_val(bw_paths, tmp_path):
    """A genuinely valid plan validates; a broken one does not (real VAL)."""
    # instance-1: init has (on b c); goal is (on c b).
    good_plan = "(unstack b c)\n(put-down b)\n(pick-up c)\n(stack c b)\n"
    bad_plan = "(stack c b)\n"  # precondition not met from the initial state

    good_file = tmp_path / "good_plan"
    bad_file = tmp_path / "bad_plan"
    good_file.write_text(good_plan)
    bad_file.write_text(bad_plan)

    assert validate_plan(bw_paths["domain"], bw_paths["instance"], str(good_file)) is True
    assert validate_plan(bw_paths["domain"], bw_paths["instance"], str(bad_file)) is False

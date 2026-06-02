"""Tests for PDDL <-> natural-language translation (offline)."""

from __future__ import annotations

import tempfile

from planbench.translation import instance_to_text, text_to_plan, text_to_state


def test_instance_to_text_structure(bw_problem, bw_config):
    """instance_to_text returns (INIT, GOAL, PLAN, data) as NL text."""
    init, goal, plan, data = instance_to_text(bw_problem, False, bw_config)
    assert isinstance(init, str) and init
    assert isinstance(goal, str) and goal
    # Blocksworld init descriptions mention table / clear relations.
    assert "on the table" in init or "clear" in init
    assert data is bw_config  # data passed through unchanged


def test_text_to_plan_roundtrip(bw_problem, bw_config):
    """A natural-language plan is converted to grounded PDDL action syntax."""
    nl_plan = (
        "unstack the blue block from on top of the orange block\n"
        "put down the blue block"
    )
    plan_file = tempfile.mktemp(suffix=".sas")
    pddl_plan, _ = text_to_plan(nl_plan, bw_problem.actions, plan_file, bw_config)
    lines = [ln for ln in pddl_plan.strip().split("\n") if ln]
    assert len(lines) == 2
    assert lines[0].startswith("(unstack")
    assert lines[1].startswith("(put-down")


def test_text_to_state(bw_config):
    state = text_to_state("the red block is clear and the hand is empty", bw_config)
    assert "handempty" in state
    assert any(p.startswith("clear") for p in state)


def test_text_to_plan_ignores_non_action_prose(bw_problem, bw_config):
    """Surrounding prose should not produce spurious actions."""
    nl_plan = (
        "Here is my plan:\n"
        "unstack the blue block from on top of the orange block\n"
        "That completes the task."
    )
    plan_file = tempfile.mktemp(suffix=".sas")
    pddl_plan, _ = text_to_plan(nl_plan, bw_problem.actions, plan_file, bw_config)
    lines = [ln for ln in pddl_plan.strip().split("\n") if ln]
    assert lines == ["(unstack b c)"]

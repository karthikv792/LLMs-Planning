"""Tests for task_utils state-translation helpers (offline)."""

from __future__ import annotations

from planbench.task_utils import get_state_translation
from planbench.translation import text_to_state


def test_get_state_translation_roundtrips_through_text(bw_config):
    """A grounded state can be rendered back to natural language."""
    state = text_to_state("the red block is clear and the hand is empty", bw_config)
    nl = get_state_translation(state, bw_config)
    assert isinstance(nl, str)
    assert "clear" in nl
    assert "hand is empty" in nl or "empty" in nl


def test_task_utils_public_api():
    import planbench.task_utils as tu

    for name in (
        "get_state_translation",
        "get_action_text",
        "parsed_instance_to_text_blocksworld",
        "generate_plan_cot",
        "plan_execution",
        "paraphrase_goal",
        "optimality",
        "replanning",
        "plan_verification",
    ):
        assert hasattr(tu, name), f"task_utils missing public symbol {name!r}"

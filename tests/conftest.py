"""Shared pytest fixtures and skip markers for the PlanBench test suite.

The suite is designed to run fully offline: no live LLM API calls and no
external planner binaries are required for the default run.  Tests that need
the VAL validator, Fast Downward, or an LLM API key are decorated with the
``requires_*`` markers below and auto-skip when those resources are absent.

Note on working directory: the PlanBench pipeline resolves data paths
relative to the current working directory (``./configs``, ``./instances``,
``./results`` ...), so tests that exercise pipeline classes use the
``in_data_dir`` fixture to ``chdir`` into ``data/`` for their duration.
Pure-function tests operate on absolute paths and do not need it.
"""

from __future__ import annotations

import os
from pathlib import Path

import pytest
import yaml

REPO_ROOT = Path(__file__).resolve().parent.parent
DATA_DIR = REPO_ROOT / "data"


# ── resource availability ────────────────────────────────────────────────

def _val_available() -> bool:
    # validate_plan() runs "$VAL/validate", so VAL is only usable when the env
    # var is set AND points at a runnable validate binary for this platform.
    val = os.environ.get("VAL")
    return bool(val) and (Path(val) / "validate").exists()


def _fast_downward_available() -> bool:
    fd = os.environ.get("FAST_DOWNWARD")
    return bool(fd) and (Path(fd) / "fast-downward.py").exists()


def _openai_key_available() -> bool:
    return bool(os.environ.get("OPENAI_API_KEY"))


requires_val = pytest.mark.skipif(
    not _val_available(), reason="VAL validator not available (set $VAL)"
)
requires_fast_downward = pytest.mark.skipif(
    not _fast_downward_available(),
    reason="Fast Downward not available (set $FAST_DOWNWARD)",
)
requires_openai_key = pytest.mark.skipif(
    not _openai_key_available(), reason="OPENAI_API_KEY not set"
)


# ── path / config fixtures ───────────────────────────────────────────────

@pytest.fixture(scope="session")
def repo_root() -> Path:
    return REPO_ROOT


@pytest.fixture(scope="session")
def data_dir() -> Path:
    return DATA_DIR


@pytest.fixture
def in_data_dir():
    """chdir into ``data/`` for the test (pipeline uses cwd-relative paths)."""
    old = os.getcwd()
    os.chdir(DATA_DIR)
    try:
        yield DATA_DIR
    finally:
        os.chdir(old)


@pytest.fixture(scope="session")
def bw_config_path() -> str:
    return str(DATA_DIR / "configs" / "blocksworld.yaml")


@pytest.fixture
def bw_config(bw_config_path: str) -> dict:
    with open(bw_config_path) as f:
        return yaml.safe_load(f)


@pytest.fixture(scope="session")
def bw_paths() -> dict:
    """Absolute paths to a blocksworld domain + a sample instance."""
    with open(DATA_DIR / "configs" / "blocksworld.yaml") as f:
        cfg = yaml.safe_load(f)
    domain = DATA_DIR / "instances" / cfg["domain_file"]
    instance = DATA_DIR / "instances" / cfg["instance_dir"] / "instance-1.pddl"
    assert domain.exists(), f"missing test domain: {domain}"
    assert instance.exists(), f"missing test instance: {instance}"
    return {"domain": str(domain), "instance": str(instance), "config": cfg}


@pytest.fixture
def bw_problem(bw_paths: dict):
    """A tarski-parsed blocksworld problem (parsed from absolute paths)."""
    from tarski.io import PDDLReader

    reader = PDDLReader(raise_on_error=True)
    reader.parse_domain(bw_paths["domain"])
    return reader.parse_instance(bw_paths["instance"])

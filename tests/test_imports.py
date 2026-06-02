"""Import-smoke tests.

Guards against (a) the kind of stale-import bugs that the package merge left
behind (``writer_new``, bare ``model_parser`` imports) and (b) any module-level
side effects that would require API keys / network at import time.
"""

from __future__ import annotations

import importlib
import pkgutil
from pathlib import Path

import planbench


def _all_submodules() -> list[str]:
    root = Path(planbench.__file__).parent
    return [m.name for m in pkgutil.walk_packages([str(root)], prefix="planbench.")]


def test_every_submodule_imports():
    """Every module under planbench imports cleanly."""
    failures = {}
    for name in _all_submodules():
        try:
            importlib.import_module(name)
        except Exception as exc:  # pragma: no cover - failure path
            failures[name] = repr(exc)
    assert not failures, f"modules failed to import: {failures}"


def test_import_requires_no_api_keys(monkeypatch):
    """Importing the LLM layer must not need provider credentials."""
    for var in (
        "OPENAI_API_KEY",
        "ANTHROPIC_API_KEY",
        "DEEPSEEK_API_KEY",
        "GROQ_API_KEY",
        "GOOGLE_API_KEY",
        "AWS_ACCESS_KEY_ID",
    ):
        monkeypatch.delenv(var, raising=False)

    import planbench.llm  # noqa: F401
    from planbench.llm import get_client, LLMClient, LLMResponse  # noqa: F401


def test_public_package_metadata():
    assert isinstance(planbench.__version__, str)
    assert planbench.__version__

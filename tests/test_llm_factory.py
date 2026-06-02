"""Tests for the LLM client factory and response container.

Clients are only inspected for type — no client is queried, so no API keys
or network access are required.
"""

from __future__ import annotations

import pytest

from planbench.llm import get_client, LLMResponse


@pytest.mark.parametrize(
    "engine, expected_cls",
    [
        ("gpt-4o_chat", "OpenAIClient"),
        ("o1-mini_chat", "OpenAIClient"),
        ("finetuned", "OpenAIClient"),
        ("deepseek-r1", "OpenAIClient"),
        ("claude-3.5-sonnet", "AnthropicClient"),
        ("claude-3-opus", "AnthropicClient"),
        ("claude-3-sonnet_aws", "AWSBedrockClient"),
        ("llama-3.1-405b_aws", "AWSBedrockClient"),
        ("llama-3.1-70b_groq", "GroqClient"),
        ("gemini-1.5-pro", "GoogleClient"),
        ("palm", "GoogleClient"),
        ("bloom", "LocalClient"),
        ("deepseek-r1_local", "LocalClient"),
        ("qwq-32b", "LocalClient"),
        ("qwen2.5-32b", "LocalClient"),
    ],
)
def test_get_client_dispatch(engine, expected_cls):
    client = get_client(engine)
    assert type(client).__name__ == expected_cls


def test_get_client_needs_no_credentials(monkeypatch):
    for var in ("OPENAI_API_KEY", "ANTHROPIC_API_KEY", "GROQ_API_KEY"):
        monkeypatch.delenv(var, raising=False)
    # Construction must not touch credentials.
    assert get_client("gpt-4o_chat") is not None
    assert get_client("claude-3.5-sonnet") is not None


def test_llm_response_defaults():
    r = LLMResponse(text="hello")
    assert r.text == "hello"
    assert r.raw_response is None
    assert r.time_taken == 0.0
    assert r.usage is None
    assert r.messages is None
    assert r.context_window_exceeded is False
    assert r.null_response is False


def test_llm_response_carries_usage_and_history():
    r = LLMResponse(
        text="hi",
        usage={"completion_tokens_details": {"reasoning_tokens": 42}},
        messages=[{"role": "user", "content": "x"}],
    )
    assert r.usage["completion_tokens_details"]["reasoning_tokens"] == 42
    assert r.messages[0]["role"] == "user"

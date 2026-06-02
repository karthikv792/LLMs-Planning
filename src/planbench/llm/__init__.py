"""LLM abstraction layer for PlanBench.

Provides a unified interface to all supported LLM providers through
the :class:`LLMClient` protocol and the :func:`get_client` factory.

Usage::

    from planbench.llm import get_client

    client = get_client("gpt-4_chat")
    response = client.query("What is 2+2?")
    print(response.text)

No side effects at import time — provider SDKs are imported lazily
inside each client class on first use.
"""

from __future__ import annotations

from typing import Any

from .base import LLMClient, LLMResponse

__all__ = ["LLMClient", "LLMResponse", "get_client"]


def get_client(engine: str, **kwargs: Any) -> LLMClient:
    """Create an LLM client for the given engine string.

    Engine string formats:

    =========================  ===========  ================================
    Engine pattern             Provider     Examples
    =========================  ===========  ================================
    ``bloom``                  Local        ``bloom``
    ``*_local``                Local        ``deepseek-r1_local``
    ``qwq*``, ``qwen*``       Local        ``qwq-32b``, ``qwen2.5-32b``
    ``palm``                   Google       ``palm``
    ``gemini*``                Google       ``gemini-1.5-pro``,
                                            ``gemini-2.5-thinking``
    ``claude-3-opus``          Anthropic    ``claude-3-opus``
    ``claude-3.5-sonnet``      Anthropic    ``claude-3.5-sonnet``
    ``finetuned``              OpenAI       ``finetuned`` (needs model_id)
    ``*_chat``                 OpenAI       ``gpt-4_chat``, ``o1-mini_chat``
    ``deepseek-r1``            OpenAI*      ``deepseek-r1`` (DS API)
    ``*_groq``                 Groq         ``llama-3.1-405b_groq``
    ``*_aws``                  AWS Bedrock  ``claude-3-sonnet_aws``,
                                            ``llama-3.1-405b_aws``
    (other)                    OpenAI       legacy completion engines
    =========================  ===========  ================================

    Args:
        engine: Engine identifier string.
        **kwargs: Forwarded to the client constructor (e.g. ``model_id``,
            ``model_path``, ``system_prompt``).

    Returns:
        An object satisfying the :class:`LLMClient` protocol.
    """
    eng_lower = engine.lower()

    # --- Local models ---
    if engine == "bloom" or "_local" in eng_lower:
        from .local_client import LocalClient
        return LocalClient(engine, **kwargs)
    if eng_lower.startswith("qwq") or eng_lower.startswith("qwen"):
        from .local_client import LocalClient
        return LocalClient(engine, **kwargs)

    # --- Google ---
    if engine == "palm" or eng_lower.startswith("gemini"):
        from .google_client import GoogleClient
        return GoogleClient(engine, **kwargs)

    # --- Anthropic (direct) ---
    if engine in ("claude-3-opus", "claude-3.5-sonnet") and "_aws" not in engine:
        from .anthropic_client import AnthropicClient
        return AnthropicClient(engine, **kwargs)

    # --- Groq ---
    if "_groq" in engine:
        from .groq_client import GroqClient
        return GroqClient(engine, **kwargs)

    # --- AWS Bedrock ---
    if "_aws" in engine:
        from .aws_client import AWSBedrockClient
        return AWSBedrockClient(engine, **kwargs)

    # --- OpenAI (chat, completion, finetuned, DeepSeek-R1) ---
    from .openai_client import OpenAIClient
    return OpenAIClient(engine, **kwargs)

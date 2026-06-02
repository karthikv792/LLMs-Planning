"""Base types for the LLM abstraction layer.

Defines LLMResponse (the unified return type for all providers)
and LLMClient (the protocol that every provider implements).
"""

from __future__ import annotations

from dataclasses import dataclass, field
from typing import Any, Protocol, runtime_checkable


@dataclass
class LLMResponse:
    """Unified response from any LLM provider.

    Attributes:
        text: The generated text (stripped of whitespace).
        raw_response: The original provider-specific response object.
        time_taken: Wall-clock seconds for the API call.
        usage: Token usage dict.  Shape varies by provider; for OpenAI
            reasoning models this may include nested
            ``completion_tokens_details.reasoning_tokens``.
        messages: Updated conversation history (list of role/content dicts)
            after this turn.  Populated by ``query_with_history``.
        context_window_exceeded: True when the provider signalled that the
            context window was full.
        null_response: True when the provider returned an empty or
            whitespace-only response.
    """

    text: str = ""
    raw_response: Any = None
    time_taken: float = 0.0
    usage: dict | None = None
    messages: list[dict] | None = None
    context_window_exceeded: bool = False
    null_response: bool = False


@runtime_checkable
class LLMClient(Protocol):
    """Protocol that every LLM provider must satisfy."""

    def query(
        self,
        prompt: str,
        max_tokens: int = 4096,
        *,
        temperature: float = 0.0,
        stop: str | None = "[STATEMENT]",
        **kwargs: Any,
    ) -> LLMResponse:
        """Single-prompt, single-response query.

        Args:
            prompt: The user prompt text.
            max_tokens: Maximum tokens to generate.
            temperature: Sampling temperature.
            stop: Stop sequence (provider support varies).
            **kwargs: Provider-specific overrides.

        Returns:
            An LLMResponse with at least ``text`` populated.
        """
        ...

    def query_with_history(
        self,
        prompt: str,
        messages: list[dict] | None = None,
        max_tokens: int = 4096,
        *,
        temperature: float = 0.0,
        history: int = -1,
        **kwargs: Any,
    ) -> LLMResponse:
        """Multi-turn conversational query (used by back-prompting).

        Args:
            prompt: The new user message.
            messages: Existing conversation history.  If *None* the
                implementation will start a fresh conversation.
            max_tokens: Maximum tokens to generate.
            temperature: Sampling temperature.
            history: Number of previous turn-pairs to include.
                ``-1`` means send the full history.
            **kwargs: Provider-specific overrides.

        Returns:
            An LLMResponse with ``text`` and ``messages`` populated.
        """
        ...

    def query_multiple(
        self,
        prompt: str,
        n: int = 1,
        max_tokens: int = 4096,
        *,
        temperature: float = 0.0,
        stop: str | None = "[STATEMENT]",
        **kwargs: Any,
    ) -> list[LLMResponse]:
        """Generate *n* independent completions for one prompt.

        Args:
            prompt: The user prompt text.
            n: Number of completions to generate.
            max_tokens: Maximum tokens to generate per completion.
            temperature: Sampling temperature.
            stop: Stop sequence.
            **kwargs: Provider-specific overrides.

        Returns:
            A list of *n* LLMResponse objects.
        """
        ...

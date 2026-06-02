"""Anthropic (Claude) LLM client.

Covers:
* Claude 3 Opus  (``claude-3-opus`` engine)
* Claude 3.5 Sonnet  (``claude-3.5-sonnet`` engine)
* Any future Claude model passed as the engine name

System prompt is configurable (defaults to the planner assistant prompt
used by the original codebase).

No side effects at import time — the ``Anthropic`` client is created lazily.
"""

from __future__ import annotations

import time
from typing import Any

from .base import LLMResponse

# Model ID lookup for legacy engine names.
_ENGINE_TO_MODEL: dict[str, str] = {
    "claude-3-opus": "claude-3-opus-20240229",
    "claude-3.5-sonnet": "claude-3-5-sonnet-20240620",
}

_DEFAULT_SYSTEM_PROMPT = (
    "You are the planner assistant who comes up with correct plans."
)


class AnthropicClient:
    """Claude client via the Anthropic Messages API."""

    def __init__(
        self,
        engine: str,
        *,
        system_prompt: str = _DEFAULT_SYSTEM_PROMPT,
    ) -> None:
        self._engine = engine
        self._model_id = _ENGINE_TO_MODEL.get(engine, engine)
        self._system_prompt = system_prompt
        self._client: Any | None = None

    def _get_client(self) -> Any:
        if self._client is None:
            import anthropic  # lazy
            self._client = anthropic.Anthropic()
        return self._client

    # -- protocol methods -----------------------------------------------------

    def query(
        self,
        prompt: str,
        max_tokens: int = 4096,
        *,
        temperature: float = 0.0,
        stop: str | None = "[STATEMENT]",
        **kwargs: Any,
    ) -> LLMResponse:
        client = self._get_client()

        try:
            st = time.time()
            response = client.messages.create(
                model=self._model_id,
                max_tokens=max_tokens,
                temperature=temperature,
                system=self._system_prompt,
                messages=[{"role": "user", "content": prompt}],
            )
            elapsed = time.time() - st
        except Exception as e:
            if "context" in str(e).lower() or "token" in str(e).lower():
                return LLMResponse(context_window_exceeded=True)
            raise

        text = _extract_text(response)
        return LLMResponse(
            text=text,
            raw_response=response,
            time_taken=elapsed,
            usage=_extract_usage(response),
            null_response=not text,
        )

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
        client = self._get_client()

        if messages is None or len(messages) == 0:
            messages = [{"role": "user", "content": prompt}]
        else:
            messages = list(messages)
            messages.append({"role": "user", "content": prompt})

        # Anthropic messages API does not use a system message in the
        # messages array — it's a separate parameter.  So history
        # truncation is simpler: just keep first + last N pairs.
        if history != -1 and len(messages) > 1:
            if history <= 0:
                sending = messages[:1]
            else:
                sending = messages[:1] + messages[-(history * 2):]
        else:
            sending = messages

        ctx_hit = False
        null = False
        try:
            st = time.time()
            response = client.messages.create(
                model=self._model_id,
                max_tokens=max_tokens,
                temperature=temperature,
                system=self._system_prompt,
                messages=sending,
            )
            elapsed = time.time() - st
        except Exception as e:
            if "context" in str(e).lower() or "token" in str(e).lower():
                ctx_hit = True
            else:
                raise
            elapsed = 0.0
            response = None

        text = _extract_text(response) if response else ""
        if not text:
            null = True

        messages.append({"role": "assistant", "content": text})

        return LLMResponse(
            text=text,
            raw_response=response,
            time_taken=elapsed,
            usage=_extract_usage(response) if response else None,
            messages=messages,
            context_window_exceeded=ctx_hit,
            null_response=null,
        )

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
        return [
            self.query(prompt, max_tokens, temperature=temperature, stop=stop)
            for _ in range(n)
        ]


# -- helpers ------------------------------------------------------------------

def _extract_text(response: Any) -> str:
    """Pull text from an Anthropic Messages response (filter by type='text')."""
    if response is None:
        return ""
    for block in response.content:
        if block.type == "text":
            return block.text.strip()
    return ""


def _extract_usage(response: Any) -> dict | None:
    if response is None:
        return None
    usage = getattr(response, "usage", None)
    if usage is None:
        return None
    return {
        "input_tokens": getattr(usage, "input_tokens", None),
        "output_tokens": getattr(usage, "output_tokens", None),
    }

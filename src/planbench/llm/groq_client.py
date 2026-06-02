"""Groq LLM client.

Engine format: ``<model>_groq`` (e.g. ``llama-3.1-405b_groq``,
``mixtral-8x7b_groq``).

Groq requires temperature >= 0.0001 (zero is rejected).
A 2-second sleep is inserted between requests for rate-limit compliance.

No side effects at import time — the ``Groq`` client is created lazily.
"""

from __future__ import annotations

import time
from typing import Any

from .base import LLMResponse

_MIN_TEMPERATURE = 0.0001
_RATE_LIMIT_SLEEP = 2  # seconds


class GroqClient:
    """Groq chat completions client."""

    def __init__(self, engine: str) -> None:
        self._engine = engine
        self._model = engine.split("_")[0]
        self._client: Any | None = None

    def _get_client(self) -> Any:
        if self._client is None:
            from groq import Groq  # lazy
            self._client = Groq()
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
        temp = max(temperature, _MIN_TEMPERATURE)

        messages = [
            {"role": "system", "content": "You are the planner assistant who comes up with correct plans."},
            {"role": "user", "content": prompt},
        ]

        try:
            st = time.time()
            response = client.chat.completions.create(
                messages=messages, model=self._model, temperature=temp,
            )
            elapsed = time.time() - st
        except Exception:
            return LLMResponse(null_response=True)

        text = response.choices[0].message.content or ""
        time.sleep(_RATE_LIMIT_SLEEP)

        return LLMResponse(
            text=text.strip(),
            raw_response=response,
            time_taken=elapsed,
            null_response=not text.strip(),
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
        temp = max(temperature, _MIN_TEMPERATURE)

        if messages is None or len(messages) == 0:
            messages = [
                {"role": "system", "content": "You are the planner assistant who comes up with correct plans."},
                {"role": "user", "content": prompt},
            ]
        else:
            messages = list(messages)
            messages.append({"role": "user", "content": prompt})

        # Truncate history (system + first user = prefix of 2)
        if history != -1 and len(messages) > 2:
            if history <= 0:
                sending = messages[:2]
            else:
                sending = messages[:2] + messages[-(history * 2):]
        else:
            sending = messages

        ctx_hit = False
        null = False
        try:
            st = time.time()
            response = client.chat.completions.create(
                messages=sending, model=self._model, temperature=temp,
            )
            elapsed = time.time() - st
        except Exception as e:
            if "context" in str(e).lower():
                ctx_hit = True
            else:
                raise
            elapsed = 0.0
            response = None

        text = ""
        if response is not None:
            text = response.choices[0].message.content or ""
        if not text or text.isspace():
            null = True
            text = ""

        time.sleep(_RATE_LIMIT_SLEEP)
        messages.append({"role": "assistant", "content": text.strip()})

        return LLMResponse(
            text=text.strip(),
            raw_response=response,
            time_taken=elapsed,
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

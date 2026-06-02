"""OpenAI LLM client.

Covers:
* Chat models  (``gpt-4_chat``, ``gpt-4o_chat``, …)
* Reasoning models (``o1-preview_chat``, ``o1-mini_chat`` — no temperature)
* DeepSeek-R1 via OpenAI-compatible API (separate key / base-url)
* Fine-tuned legacy completion models (``finetuned`` engine)
* Legacy completion API (default fallback)

No side effects at import time — the ``OpenAI`` client is created lazily
on the first call.
"""

from __future__ import annotations

import os
import time
from typing import Any

from .base import LLMResponse

# System prompt shared across chat-style calls.
_SYSTEM_PROMPT = "You are the planner assistant who comes up with correct plans."


class OpenAIClient:
    """Unified OpenAI / DeepSeek-R1 client."""

    def __init__(self, engine: str, *, model_id: str | None = None) -> None:
        self._engine = engine
        self._model_id = model_id  # explicit override (e.g. for finetuned)
        self._client: Any | None = None
        self._deepseek_client: Any | None = None

    # -- lazy client helpers --------------------------------------------------

    def _get_client(self) -> Any:
        if self._client is None:
            from openai import OpenAI
            self._client = OpenAI()
        return self._client

    def _get_deepseek_client(self) -> Any:
        if self._deepseek_client is None:
            from openai import OpenAI
            api_key = os.environ["DEEPSEEK_API_KEY"]
            self._deepseek_client = OpenAI(
                api_key=api_key,
                base_url="https://api.deepseek.com",
            )
        return self._deepseek_client

    # -- engine helpers -------------------------------------------------------

    def _is_chat(self) -> bool:
        return "_chat" in self._engine

    def _is_deepseek_r1(self) -> bool:
        return "deepseek-r1" in self._engine

    def _is_reasoning_model(self) -> bool:
        eng = self._chat_model_name()
        return any(tag in eng for tag in ("o1-", "o1_", "o3-", "o3_"))

    def _is_finetuned(self) -> bool:
        return self._engine == "finetuned"

    def _chat_model_name(self) -> str:
        """Extract model name from engine string (e.g. 'gpt-4_chat' → 'gpt-4')."""
        return self._engine.split("_")[0]

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
        if self._is_deepseek_r1():
            return self._query_deepseek(prompt, max_tokens)
        if self._is_chat():
            return self._query_chat(prompt, max_tokens, temperature=temperature)
        if self._is_finetuned():
            return self._query_finetuned(prompt, max_tokens, temperature=temperature)
        return self._query_completion(prompt, max_tokens, temperature=temperature, stop=stop)

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
        if self._is_deepseek_r1():
            return self._query_with_history_deepseek(prompt, messages, history=history)
        if self._is_chat():
            return self._query_with_history_chat(
                prompt, messages, max_tokens, temperature=temperature, history=history,
            )
        raise NotImplementedError(
            f"query_with_history not supported for engine {self._engine!r}"
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
        if self._is_finetuned():
            return self._query_multiple_finetuned(prompt, n, max_tokens, temperature=temperature)
        if self._is_chat():
            return self._query_multiple_chat(prompt, n, max_tokens, temperature=temperature)
        return self._query_multiple_completion(prompt, n, max_tokens, temperature=temperature, stop=stop)

    # -- internal implementations ---------------------------------------------

    def _query_chat(
        self, prompt: str, max_tokens: int, *, temperature: float,
    ) -> LLMResponse:
        client = self._get_client()
        eng = self._chat_model_name()
        is_reasoning = self._is_reasoning_model()

        messages = [{"role": "user", "content": prompt}]
        if not is_reasoning:
            messages.insert(0, {"role": "system", "content": _SYSTEM_PROMPT})

        call_kwargs: dict[str, Any] = {"model": eng, "messages": messages}
        if not is_reasoning:
            call_kwargs["temperature"] = temperature

        try:
            st = time.time()
            response = client.chat.completions.create(**call_kwargs)
            elapsed = time.time() - st
        except Exception as e:
            if "maximum context length" in str(e):
                return LLMResponse(context_window_exceeded=True)
            raise

        text = response.choices[0].message.content or ""
        usage = _extract_usage(response)
        null = not text or text.isspace()
        return LLMResponse(
            text=text.strip(),
            raw_response=response,
            time_taken=elapsed,
            usage=usage,
            null_response=null,
        )

    def _query_deepseek(self, prompt: str, max_tokens: int) -> LLMResponse:
        client = self._get_deepseek_client()
        messages = [{"role": "user", "content": prompt}]
        try:
            st = time.time()
            response = client.chat.completions.create(
                model="deepseek-reasoner", messages=messages, stream=False,
            )
            elapsed = time.time() - st
        except Exception as e:
            if "maximum context length" in str(e):
                return LLMResponse(context_window_exceeded=True)
            raise

        text = response.choices[0].message.content or ""
        null = not text or text.isspace()
        return LLMResponse(
            text=text.strip(),
            raw_response=response,
            time_taken=elapsed,
            usage=_extract_usage(response),
            null_response=null,
        )

    def _query_finetuned(
        self, prompt: str, max_tokens: int, *, temperature: float,
    ) -> LLMResponse:
        client = self._get_client()
        model_name = self._model_id
        if model_name is None:
            raise ValueError("model_id is required for finetuned engine")
        try:
            response = client.completions.create(
                model=model_name,
                prompt=prompt,
                temperature=temperature,
                max_tokens=max_tokens,
                top_p=1,
                frequency_penalty=0,
                presence_penalty=0,
                stop=["[PLAN END]"],
            )
        except Exception:
            return LLMResponse(null_response=True)

        text = response.choices[0].text or ""
        return LLMResponse(text=text.strip(), raw_response=response)

    def _query_completion(
        self, prompt: str, max_tokens: int, *, temperature: float, stop: str | None,
    ) -> LLMResponse:
        client = self._get_client()
        try:
            response = client.completions.create(
                model=self._engine,
                prompt=prompt,
                temperature=temperature,
                max_tokens=max_tokens,
                top_p=1,
                frequency_penalty=0,
                presence_penalty=0,
                stop=stop,
            )
        except Exception:
            return LLMResponse(null_response=True)

        text = response.choices[0].text or ""
        return LLMResponse(text=text.strip(), raw_response=response)

    # -- multi-turn -----------------------------------------------------------

    def _query_with_history_chat(
        self,
        prompt: str,
        messages: list[dict] | None,
        max_tokens: int,
        *,
        temperature: float,
        history: int,
    ) -> LLMResponse:
        client = self._get_client()
        eng = self._chat_model_name()
        is_reasoning = self._is_reasoning_model()

        if messages is None or len(messages) == 0:
            messages = []
            if not is_reasoning:
                messages.append({"role": "system", "content": _SYSTEM_PROMPT})
            messages.append({"role": "user", "content": prompt})
        else:
            messages = list(messages)
            messages.append({"role": "user", "content": prompt})

        sending = _truncate_history(messages, history, has_system=not is_reasoning)

        call_kwargs: dict[str, Any] = {"model": eng, "messages": sending}
        if not is_reasoning:
            call_kwargs["temperature"] = temperature

        ctx_hit = False
        null = False
        try:
            st = time.time()
            response = client.chat.completions.create(**call_kwargs)
            elapsed = time.time() - st
        except Exception as e:
            if "maximum context length" in str(e):
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

        messages.append({"role": "assistant", "content": text.strip()})

        return LLMResponse(
            text=text.strip(),
            raw_response=response,
            time_taken=elapsed,
            usage=_extract_usage(response) if response else None,
            messages=messages,
            context_window_exceeded=ctx_hit,
            null_response=null,
        )

    def _query_with_history_deepseek(
        self,
        prompt: str,
        messages: list[dict] | None,
        *,
        history: int,
    ) -> LLMResponse:
        client = self._get_deepseek_client()

        if messages is None or len(messages) == 0:
            messages = [{"role": "user", "content": prompt}]
        else:
            messages = list(messages)
            messages.append({"role": "user", "content": prompt})

        sending = _truncate_history(messages, history, has_system=False)

        ctx_hit = False
        null = False
        try:
            st = time.time()
            response = client.chat.completions.create(
                model="deepseek-reasoner", messages=sending, stream=False,
            )
            elapsed = time.time() - st
        except Exception as e:
            if "maximum context length" in str(e):
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

        messages.append({"role": "assistant", "content": text.strip()})

        return LLMResponse(
            text=text.strip(),
            raw_response=response,
            time_taken=elapsed,
            usage=_extract_usage(response) if response else None,
            messages=messages,
            context_window_exceeded=ctx_hit,
            null_response=null,
        )

    # -- multiple completions -------------------------------------------------

    def _query_multiple_chat(
        self, prompt: str, n: int, max_tokens: int, *, temperature: float,
    ) -> list[LLMResponse]:
        results: list[LLMResponse] = []
        collected = 0
        while collected < n:
            resp = self._query_chat(prompt, max_tokens, temperature=temperature)
            results.append(resp)
            collected += 1
            if collected < n:
                time.sleep(0.5)
        return results

    def _query_multiple_finetuned(
        self, prompt: str, n: int, max_tokens: int, *, temperature: float,
    ) -> list[LLMResponse]:
        client = self._get_client()
        model_name = self._model_id
        if model_name is None:
            raise ValueError("model_id is required for finetuned engine")
        try:
            response = client.completions.create(
                model=model_name,
                prompt=prompt,
                temperature=temperature,
                n=n,
                max_tokens=max_tokens,
                top_p=1,
                frequency_penalty=0,
                presence_penalty=0,
                stop=["[PLAN END]"],
            )
        except Exception:
            return [LLMResponse(null_response=True)]

        return [
            LLMResponse(text=(c.text or "").strip(), raw_response=response)
            for c in response.choices
        ]

    def _query_multiple_completion(
        self, prompt: str, n: int, max_tokens: int, *, temperature: float, stop: str | None,
    ) -> list[LLMResponse]:
        client = self._get_client()
        try:
            response = client.completions.create(
                model=self._engine,
                prompt=prompt,
                temperature=temperature,
                n=n,
                max_tokens=max_tokens,
                top_p=1,
                frequency_penalty=0,
                presence_penalty=0,
                stop=stop,
            )
        except Exception:
            return [LLMResponse(null_response=True)]

        return [
            LLMResponse(text=(c.text or "").strip(), raw_response=response)
            for c in response.choices
        ]


# -- module-level helpers -----------------------------------------------------

def _extract_usage(response: Any) -> dict | None:
    """Extract usage dict from an OpenAI response, if present."""
    if response is None:
        return None
    usage = getattr(response, "usage", None)
    if usage is None:
        return None
    d: dict[str, Any] = {
        "prompt_tokens": getattr(usage, "prompt_tokens", None),
        "completion_tokens": getattr(usage, "completion_tokens", None),
        "total_tokens": getattr(usage, "total_tokens", None),
    }
    # Reasoning token details (o1, o3)
    details = getattr(usage, "completion_tokens_details", None)
    if details is not None:
        d["completion_tokens_details"] = {
            "reasoning_tokens": getattr(details, "reasoning_tokens", None),
        }
    return d


def _truncate_history(
    messages: list[dict], history: int, *, has_system: bool,
) -> list[dict]:
    """Trim conversation history to *history* most recent turn-pairs.

    ``history == -1`` means keep everything.
    """
    if history == -1:
        return messages

    # How many messages form the "prefix" that is always kept?
    # With a system message the prefix is 2 (system + first user),
    # without it the prefix is 1 (first user).
    prefix_len = 2 if has_system else 1

    if len(messages) <= prefix_len:
        return messages

    if history <= 0:
        return messages[:prefix_len]

    # Keep prefix + last ``history * 2`` messages (user+assistant pairs).
    tail = messages[-(history * 2):]
    return messages[:prefix_len] + tail

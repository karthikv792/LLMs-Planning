"""Google LLM client (Gemini + legacy PaLM/Vertex AI).

Covers:
* Gemini models via ``google.genai`` (e.g. ``gemini-1.5-pro``)
* Gemini 2.5 thinking models (``gemini-2.5-thinking`` engine) —
  v1alpha API, thought/response separation, 70-second rate-limit sleep
* Legacy PaLM via Vertex AI (``palm`` engine, ``text-bison@001``)

No side effects at import time — all API clients are created lazily.
"""

from __future__ import annotations

import os
import time
from typing import Any

from .base import LLMResponse

_RATE_LIMIT_SLEEP = 45  # seconds between non-thinking Gemini calls
_THINKING_RATE_LIMIT_SLEEP = 70  # seconds between thinking-model calls


class GoogleClient:
    """Unified Google / Gemini / PaLM client."""

    def __init__(self, engine: str) -> None:
        self._engine = engine

    # -- engine helpers -------------------------------------------------------

    def _is_thinking(self) -> bool:
        return "gemini-2.5-thinking" in self._engine

    def _is_palm(self) -> bool:
        return self._engine == "palm"

    def _gemini_model_name(self) -> str:
        if self._is_thinking():
            return "gemini-2.5-pro-preview-03-25"
        return self._engine

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
        if self._is_palm():
            return self._query_palm(prompt, temperature=temperature)
        return self._query_gemini(prompt, max_tokens, temperature=temperature)

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
        if self._is_thinking():
            return self._query_with_history_thinking(
                prompt, messages, history=history,
            )
        raise NotImplementedError(
            "query_with_history is only supported for gemini-2.5-thinking"
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

    # -- internal implementations ---------------------------------------------

    def _query_gemini(
        self, prompt: str, max_tokens: int, *, temperature: float,
    ) -> LLMResponse:
        from google import genai  # lazy

        api_key = os.environ["GOOGLE_API_KEY"]
        genai.configure(api_key=api_key)

        model_name = self._gemini_model_name()
        model = genai.GenerativeModel(model_name)

        st = time.time()
        response = model.generate_content(
            prompt,
            generation_config=genai.types.GenerationConfig(
                candidate_count=1,
                max_output_tokens=max_tokens,
                temperature=temperature,
            ),
            stream=False,
        )
        elapsed = time.time() - st

        text = response.text or ""
        time.sleep(_RATE_LIMIT_SLEEP)

        return LLMResponse(
            text=text.strip(),
            raw_response=response,
            time_taken=elapsed,
        )

    def _query_palm(self, prompt: str, *, temperature: float) -> LLMResponse:
        import vertexai  # lazy
        from vertexai.language_models import TextGenerationModel

        vertexai.init(project="llm-planning")
        model = TextGenerationModel.from_pretrained("text-bison@001")

        st = time.time()
        response = model.predict(prompt, temperature=temperature)
        elapsed = time.time() - st

        text = response.text or ""
        return LLMResponse(
            text=text.strip(),
            raw_response=response,
            time_taken=elapsed,
        )

    def _query_with_history_thinking(
        self,
        prompt: str,
        messages: list[dict] | None,
        *,
        history: int,
    ) -> LLMResponse:
        from google import genai  # lazy
        from google.genai import types

        api_key = os.environ["GOOGLE_API_KEY"]
        client = genai.Client(
            api_key=api_key,
            http_options={"api_version": "v1alpha"},
        )

        model_name = self._gemini_model_name()

        # Build message list
        if messages is None or len(messages) == 0:
            messages = [{"role": "user", "content": prompt}]
        else:
            messages = list(messages)
            messages.append({"role": "user", "content": prompt})

        # Apply history truncation (no system message for thinking models)
        if history != -1 and len(messages) > 1:
            if history <= 0:
                sending = messages[:1]
            else:
                sending = messages[:1] + messages[-(history * 2):]
        else:
            sending = messages

        # Build Gemini chat history (all messages except the last)
        chat_history = []
        for msg in sending[:-1]:
            role = msg["role"] if msg["role"] == "user" else "model"
            chat_history.append(
                types.Content(role=role, parts=[types.Part(text=msg["content"])])
            )

        chat = client.chats.create(model=model_name, history=chat_history)

        ctx_hit = False
        null = False
        try:
            st = time.time()
            response = chat.send_message(sending[-1]["content"])
            elapsed = time.time() - st
        except Exception as e:
            if "context" in str(e).lower():
                ctx_hit = True
            elapsed = 0.0
            response = None

        # Separate thought from response text
        thought_text = ""
        response_text = ""
        if response is not None:
            for part in response.candidates[0].content.parts:
                if part.thought:
                    thought_text += part.text
                else:
                    response_text += part.text

        if not response_text or response_text.isspace():
            null = True
            response_text = ""

        time.sleep(_THINKING_RATE_LIMIT_SLEEP)

        messages.append({"role": "assistant", "content": response_text.strip()})

        return LLMResponse(
            text=response_text.strip(),
            raw_response=response,
            time_taken=elapsed,
            usage={"thought": thought_text} if thought_text else None,
            messages=messages,
            context_window_exceeded=ctx_hit,
            null_response=null,
        )

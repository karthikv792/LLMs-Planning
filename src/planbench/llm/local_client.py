"""Local model LLM client.

Covers:
* BLOOM  (``bloom`` engine) — via ``transformers``
* QwQ / Qwen models — via ``transformers`` with ``AutoModelForCausalLM``
* DeepSeek-R1 local — via ``vllm.LLM``

Models are loaded on first ``query()`` call (or can be pre-loaded via
``load_model()``).  GPU device mapping and 8-bit quantization are used
where appropriate.

No side effects at import time.
"""

from __future__ import annotations

import time
from typing import Any

from .base import LLMResponse


class LocalClient:
    """Client for locally-hosted transformer models."""

    def __init__(self, engine: str, *, model_path: str | None = None) -> None:
        self._engine = engine
        self._model_path = model_path
        self._model: Any | None = None
        self._tokenizer: Any | None = None

    def load_model(self) -> dict[str, Any]:
        """Load the model and tokenizer.

        Returns:
            Dict with ``model`` and ``tokenizer`` keys (for backward compat
            with callers that expect this shape).
        """
        if self._model is not None:
            return {"model": self._model, "tokenizer": self._tokenizer}

        if self._engine == "bloom":
            self._model, self._tokenizer = _load_bloom(self._model_path)
        elif "deepseek-r1" in self._engine and "local" in self._engine:
            self._model, self._tokenizer = _load_deepseek_r1_local(self._model_path)
        elif "qwq" in self._engine.lower():
            self._model, self._tokenizer = _load_qwq(self._model_path)
        elif "qwen" in self._engine.lower():
            self._model, self._tokenizer = _load_qwen(self._model_path)
        else:
            raise ValueError(f"Unknown local engine: {self._engine!r}")

        return {"model": self._model, "tokenizer": self._tokenizer}

    def set_model(self, model: Any, tokenizer: Any) -> None:
        """Inject a pre-loaded model/tokenizer (e.g. from caller)."""
        self._model = model
        self._tokenizer = tokenizer

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
        if self._model is None:
            self.load_model()

        if self._engine == "bloom":
            return self._query_bloom(prompt, max_tokens)
        if "deepseek-r1" in self._engine and "local" in self._engine:
            return self._query_vllm(prompt, max_tokens, temperature=temperature)
        return self._query_transformers(prompt, max_tokens, temperature=temperature)

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
        # Local models don't natively support multi-turn; concatenate history
        # into a single prompt.
        if messages is None or len(messages) == 0:
            full_prompt = prompt
            messages = [{"role": "user", "content": prompt}]
        else:
            messages = list(messages)
            messages.append({"role": "user", "content": prompt})
            full_prompt = "\n".join(m["content"] for m in messages)

        resp = self.query(full_prompt, max_tokens, temperature=temperature)
        messages.append({"role": "assistant", "content": resp.text})
        resp.messages = messages
        return resp

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

    def _query_bloom(self, prompt: str, max_tokens: int) -> LLMResponse:
        from transformers import StoppingCriteriaList  # lazy

        encoded_input = self._tokenizer(prompt, return_tensors="pt")
        st = time.time()
        output_sequences = self._model.generate(
            input_ids=encoded_input["input_ids"].cuda(),
            max_new_tokens=max_tokens,
            temperature=0,
            top_p=1,
        )
        elapsed = time.time() - st

        raw_text = self._tokenizer.decode(
            output_sequences[0], skip_special_tokens=True,
        )
        # Remove the original prompt from the output
        response_text = raw_text.replace(prompt, "")
        # Truncate at [PLAN END]
        lines = []
        for line in response_text.split("\n"):
            if "[PLAN END]" in line:
                break
            lines.append(line)
        text = "\n".join(lines)

        return LLMResponse(text=text.strip(), time_taken=elapsed)

    def _query_transformers(
        self, prompt: str, max_tokens: int, *, temperature: float,
    ) -> LLMResponse:
        inputs = self._tokenizer(prompt, return_tensors="pt").to(
            self._model.device
        )
        gen_kwargs: dict[str, Any] = {
            "max_new_tokens": max_tokens,
            "do_sample": temperature > 0,
        }
        if temperature > 0:
            gen_kwargs["temperature"] = temperature

        st = time.time()
        output_ids = self._model.generate(**inputs, **gen_kwargs)
        elapsed = time.time() - st

        # Decode only the new tokens
        new_tokens = output_ids[0][inputs["input_ids"].shape[1]:]
        text = self._tokenizer.decode(new_tokens, skip_special_tokens=True)

        return LLMResponse(text=text.strip(), time_taken=elapsed)

    def _query_vllm(
        self, prompt: str, max_tokens: int, *, temperature: float,
    ) -> LLMResponse:
        """Query a model loaded via vLLM (DeepSeek-R1 local)."""
        from vllm import SamplingParams  # lazy

        params = SamplingParams(
            max_tokens=max_tokens,
            temperature=temperature if temperature > 0 else 0.0,
        )
        st = time.time()
        outputs = self._model.generate([prompt], params)
        elapsed = time.time() - st

        text = outputs[0].outputs[0].text if outputs else ""
        return LLMResponse(text=text.strip(), time_taken=elapsed)


# -- model loading helpers ----------------------------------------------------

def _load_bloom(model_path: str | None) -> tuple[Any, Any]:
    import os
    from transformers import AutoTokenizer, AutoModelForCausalLM

    path = model_path or os.environ.get(
        "BLOOM_CACHE_DIR", "bigscience/bloom"
    )
    tokenizer = AutoTokenizer.from_pretrained(path)
    model = AutoModelForCausalLM.from_pretrained(
        path, load_in_8bit=True, device_map="auto",
    )
    return model, tokenizer


def _load_qwq(model_path: str | None) -> tuple[Any, Any]:
    from transformers import AutoTokenizer, AutoModelForCausalLM

    path = model_path or "Qwen/QwQ-32B-Preview"
    tokenizer = AutoTokenizer.from_pretrained(path)
    model = AutoModelForCausalLM.from_pretrained(
        path, device_map="auto",
    )
    return model, tokenizer


def _load_qwen(model_path: str | None) -> tuple[Any, Any]:
    from transformers import AutoTokenizer, AutoModelForCausalLM

    path = model_path or "Qwen/Qwen2.5-32B"
    tokenizer = AutoTokenizer.from_pretrained(path)
    model = AutoModelForCausalLM.from_pretrained(
        path, device_map="auto",
    )
    return model, tokenizer


def _load_deepseek_r1_local(model_path: str | None) -> tuple[Any, Any]:
    from vllm import LLM  # lazy

    path = model_path or "deepseek-ai/DeepSeek-R1"
    model = LLM(model=path)
    # vLLM doesn't use a separate tokenizer object in the same way
    return model, None

"""AWS Bedrock LLM client.

Covers:
* Claude models on Bedrock (``claude-3-sonnet_aws``, ``claude-3-opus_aws``,
  ``claude-3.5-sonnet_aws``)
* LLaMA models on Bedrock (``llama-3.1-405b_aws``)

Claude uses the Anthropic bedrock format; LLaMA uses a raw prompt
with ``<|begin_of_text|>`` tags and ``max_gen_len`` instead of
``max_tokens``.

No side effects at import time — the ``boto3`` client is created lazily.
"""

from __future__ import annotations

import json
import time
from typing import Any

from .base import LLMResponse

# Map short engine prefixes to full Bedrock model IDs.
_ENGINE_TO_MODEL: dict[str, str] = {
    "claude-3-sonnet": "anthropic.claude-3-sonnet-20240229-v1:0",
    "claude-3-opus": "anthropic.claude-3-opus-20240229-v1:0",
    "claude-3.5-sonnet": "anthropic.claude-3-5-sonnet-20240620-v1:0",
    "llama-3.1-405b": "meta.llama3-1-405b-instruct-v1:0",
}


class AWSBedrockClient:
    """AWS Bedrock client for Claude and LLaMA models."""

    def __init__(self, engine: str) -> None:
        self._engine = engine
        self._prefix = engine.split("_")[0]
        self._model_id = _ENGINE_TO_MODEL.get(self._prefix)
        if self._model_id is None:
            raise ValueError(f"Unknown AWS engine prefix: {self._prefix!r}")
        self._is_llama = self._prefix.startswith("llama")
        self._client: Any | None = None

    def _get_client(self) -> Any:
        if self._client is None:
            import boto3  # lazy
            from botocore.config import Config
            cfg = Config(read_timeout=1000)
            self._client = boto3.client(
                "bedrock-runtime", region_name="us-west-2", config=cfg,
            )
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
        if self._is_llama:
            return self._query_llama(prompt, max_tokens, temperature=temperature)
        return self._query_claude(prompt, max_tokens, temperature=temperature)

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
        if self._is_llama:
            return self._query_with_history_llama(
                prompt, messages, max_tokens, temperature=temperature, history=history,
            )
        raise NotImplementedError(
            "query_with_history for Claude on Bedrock is not implemented; "
            "use the direct Anthropic client instead."
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

    # -- Claude on Bedrock ----------------------------------------------------

    def _query_claude(
        self, prompt: str, max_tokens: int, *, temperature: float,
    ) -> LLMResponse:
        client = self._get_client()
        body = {
            "anthropic_version": "bedrock-2023-05-31",
            "max_tokens": max_tokens,
            "messages": [
                {
                    "role": "user",
                    "content": [{"type": "text", "text": prompt}],
                }
            ],
            "temperature": temperature,
        }
        try:
            st = time.time()
            response = client.invoke_model(
                modelId=self._model_id,
                body=json.dumps(body),
            )
            elapsed = time.time() - st
        except Exception:
            return LLMResponse(null_response=True)

        result = json.loads(response.get("body").read())
        output_list = result.get("content", [])
        text = output_list[0]["text"].strip() if output_list else ""

        return LLMResponse(
            text=text,
            raw_response=result,
            time_taken=elapsed,
            null_response=not text,
        )

    # -- LLaMA on Bedrock ----------------------------------------------------

    def _query_llama(
        self, prompt: str, max_tokens: int, *, temperature: float,
    ) -> LLMResponse:
        client = self._get_client()
        formatted_prompt = _llama_single_prompt(prompt)
        body = {
            "max_gen_len": max_tokens,
            "prompt": formatted_prompt,
            "temperature": temperature,
        }
        try:
            st = time.time()
            response = client.invoke_model(
                modelId=self._model_id,
                body=json.dumps(body),
            )
            elapsed = time.time() - st
        except Exception:
            return LLMResponse(null_response=True)

        result = json.loads(response.get("body").read())
        text = result.get("generation", "").strip()

        return LLMResponse(
            text=text,
            raw_response=result,
            time_taken=elapsed,
            null_response=not text,
        )

    def _query_with_history_llama(
        self,
        prompt: str,
        messages: list[dict] | None,
        max_tokens: int,
        *,
        temperature: float,
        history: int,
    ) -> LLMResponse:
        client = self._get_client()

        if messages is None or len(messages) == 0:
            messages = [
                {"role": "system", "content": "You are the planner assistant who comes up with correct plans."},
                {"role": "user", "content": prompt},
            ]
        else:
            messages = list(messages)
            messages.append({"role": "user", "content": prompt})

        # Truncate history
        if history != -1 and len(messages) > 2:
            if history <= 0:
                sending = messages[:2]
            else:
                sending = messages[:2] + messages[-(history * 2):]
        else:
            sending = messages

        formatted_prompt = llama_messages_to_single_prompt(sending)
        body = {
            "max_gen_len": max_tokens,
            "prompt": formatted_prompt,
            "temperature": temperature,
        }

        null = False
        try:
            st = time.time()
            response = client.invoke_model(
                modelId=self._model_id,
                body=json.dumps(body),
            )
            elapsed = time.time() - st
        except Exception:
            messages.append({"role": "assistant", "content": ""})
            return LLMResponse(
                messages=messages, null_response=True,
            )

        result = json.loads(response.get("body").read())
        text = result.get("generation", "").strip()
        if not text:
            null = True

        messages.append({"role": "assistant", "content": text})

        return LLMResponse(
            text=text,
            raw_response=result,
            time_taken=elapsed,
            messages=messages,
            null_response=null,
        )


# -- helpers ------------------------------------------------------------------

def llama_messages_to_single_prompt(messages: list[dict]) -> str:
    """Convert a message list to LLaMA 3 prompt format.

    Format::

        <|begin_of_text|><|start_header_id|>role<|end_header_id|>
        content<|eot_id|>
        ...
        <|start_header_id|>assistant<|end_header_id|>
    """
    prompt = "<|begin_of_text|>"
    for msg in messages:
        role = msg["role"]
        content = msg["content"]
        prompt += f"<|start_header_id|>{role}<|end_header_id|>\n{content}<|eot_id|>\n"
    prompt += "<|start_header_id|>assistant<|end_header_id|>"
    return prompt


def _llama_single_prompt(query: str) -> str:
    """Wrap a single query in LLaMA 3 prompt format."""
    return llama_messages_to_single_prompt([
        {"role": "system", "content": "You are the planner assistant who comes up with correct plans."},
        {"role": "user", "content": query},
    ])

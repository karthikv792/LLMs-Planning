"""Statistics and graph generation for PlanBench experiment results.

All formerly hardcoded constants (MODELS, MODEL_GROUPS, DOMAINS, TASKS,
font path, color palette, figure sizes) are now configurable via
``StatsConfig``.

From llm_planning_analysis/stats_generation.py.
"""

from __future__ import annotations

import json
import os
import subprocess
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any

import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from matplotlib.font_manager import fontManager, FontProperties


# ── Default configuration values ─────────────────────────────────────────

_DEFAULT_MODELS: dict[str, str] = {
    "o1-preview_chat": "O1 Preview",
    "gemini-2.5-thinking-pro": "Gemini 2.5 Thinking Pro",
    "gemini-2.5-thinking-flash": "Gemini 2.5 Thinking Flash",
    "llama-3.1-405b_aws": "LLAMA 3.1 405B",
    "o1-mini_chat": "O1 Mini",
    "gpt-4o-mini-2024-07-18_chat": "GPT-4o Mini",
    "gpt-4o_chat": "GPT-4o",
    "gpt-4-turbo_chat": "GPT-4 Turbo",
    "gpt-4_chat": "GPT-4",
    "gemini-1.5-pro": "Gemini 1.5 Pro",
    "gemini-1.5-flash": "Gemini 1.5 Flash",
    "claude-3.5-sonnet_aws": "Claude 3.5 Sonnet",
    "claude-3-opus": "Claude 3 Opus",
    "fast-downward": "Fast Downward",
}

_DEFAULT_MODEL_GROUPS: dict[str, list[str]] = {
    "o1 Models (LRMs)": ["o1-preview_chat", "o1-mini_chat"],
    "Gemini Models": ["gemini-2.5-thinking-pro", "gemini-2.5-thinking-flash"],
    "LLMs": [
        "llama-3.1-405b_aws",
        "gpt-4o-mini-2024-07-18_chat",
        "gpt-4o_chat",
        "gpt-4-turbo_chat",
        "gpt-4_chat",
        "gemini-1.5-pro",
        "gemini-1.5-flash",
        "claude-3.5-sonnet_aws",
        "claude-3-opus",
    ],
    "Fast Downward": ["fast-downward"],
}

_DEFAULT_DOMAINS: dict[str, str] = {
    "mystery_blocksworld": "Mystery Blocksworld",
    "blocksworld": "Blocksworld",
    "obfuscated_randomized_blocksworld": "Randomized Mystery Blocksworld",
    "logistics": "Logistics",
    "obfuscated_randomized_logistics": "Randomized Logistics",
}

_DEFAULT_TASKS: dict[str, str] = {
    "task_1_plan_generation_zero_shot": "Plan Generation Zero Shot",
    "task_1_plan_generation": "Plan Generation One Shot",
    "task_1_plan_generation_zero_shot_pddl": "Plan Generation Zero Shot PDDL",
}


@dataclass
class StatsConfig:
    """Configuration for statistics and graph generation.

    All fields have sensible defaults matching the original hardcoded values.
    Override any field to customise the analysis.
    """

    models: dict[str, str] = field(default_factory=lambda: dict(_DEFAULT_MODELS))
    model_groups: dict[str, list[str]] = field(
        default_factory=lambda: {k: list(v) for k, v in _DEFAULT_MODEL_GROUPS.items()}
    )
    domains: dict[str, str] = field(default_factory=lambda: dict(_DEFAULT_DOMAINS))
    tasks: dict[str, str] = field(default_factory=lambda: dict(_DEFAULT_TASKS))

    font_path: str | None = None
    fontscale: float = 2
    fontsize: int = 55
    legendfont: int = 45
    figsize: tuple[int, int] = (30, 28)
    results_dir: str = "results"
    graphs_dir: str = "graphs"

    def __post_init__(self) -> None:
        if self.font_path is None:
            self.font_path = str(
                Path(__file__).resolve().parents[3] / "fonts" / "Poppins-SemiBold.ttf"
            )

    @property
    def model_group_display(self) -> dict[str, list[str]]:
        """Model groups with display names instead of engine keys."""
        mg: dict[str, list[str]] = {}
        for group_name, engines in self.model_groups.items():
            mg[group_name] = [self.models[e] for e in engines if e in self.models]
        return mg


def compute_plan(domain: str, instance: str) -> None:
    """Run Fast Downward on a domain/instance pair."""
    fast_downward_path = os.getenv("FAST_DOWNWARD")
    assert os.path.exists(f"{fast_downward_path}/fast-downward.py")
    cmd = (
        f'{fast_downward_path}/fast-downward.py {domain} {instance}'
        f' --search "astar(lmcut())"'
    )
    result = subprocess.Popen(
        cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
        universal_newlines=True,
    )
    result.communicate()
    result.wait()


class StatsGenerator:
    """Configurable statistics and graph generator for PlanBench results."""

    def __init__(self, config: StatsConfig | None = None) -> None:
        self.cfg = config or StatsConfig()
        self._setup_font()
        self.colors = sns.color_palette("deep", 11)

    def _setup_font(self) -> None:
        if self.cfg.font_path and os.path.exists(self.cfg.font_path):
            fontManager.addfont(self.cfg.font_path)
            self._prop = FontProperties(fname=self.cfg.font_path)
        else:
            self._prop = FontProperties()

    # ── Data loading ─────────────────────────────────────────────────────

    def get_data(
        self, domain: str, model: str, task: str,
    ) -> tuple[dict | None, dict | None, dict | None]:
        """Load result JSON and extract plan-length statistics."""
        files = [
            f"{self.cfg.results_dir}/{domain}/{model}/{task}",
            f"{self.cfg.results_dir}/{domain}_3/{model}/{task}",
        ]
        files = [f for f in files if os.path.exists(f)]
        if not files:
            return None, None, None

        plan_length_corrects: dict[int, list[int]] = {}
        plan_length_optimal: dict[int, list[int]] = {}
        plan_length_all: dict[int, int] = {}

        for file in files:
            try:
                with open(file) as f:
                    data = json.load(f)
            except Exception:
                continue
            for inst in data["instances"]:
                if isinstance(inst["ground_truth_plan"], list):
                    inst["ground_truth_plan"] = "\n".join(inst["ground_truth_plan"])
                plan_len = len(
                    [i for i in inst["ground_truth_plan"].split("\n") if i]
                )
                if plan_len not in plan_length_corrects:
                    plan_length_corrects[plan_len] = []
                    plan_length_all[plan_len] = 0
                try:
                    plan_length_corrects[plan_len].append(int(inst["llm_correct"]))
                except KeyError:
                    try:
                        plan_length_corrects[plan_len].append(int(inst["correct"]))
                    except KeyError:
                        print(file, inst["instance_id"])
                        raise Exception("llm_correct not found")
                try:
                    llm_plan_len = len(
                        [i for i in inst["extracted_llm_plan"].split("\n") if i]
                    )
                except Exception:
                    continue
                try:
                    correct_key = (
                        "llm_correct" if "llm_correct" in inst else "correct"
                    )
                    if inst[correct_key] and llm_plan_len == plan_len:
                        plan_length_optimal.setdefault(plan_len, []).append(1)
                    else:
                        plan_length_optimal.setdefault(plan_len, []).append(0)
                except KeyError:
                    print(file, inst["instance_id"])
                    raise Exception("llm_correct not found")
                plan_length_all[plan_len] += 1

        return plan_length_corrects, plan_length_optimal, plan_length_all

    # ── Per-model graphs ─────────────────────────────────────────────────

    def get_stats(
        self,
        plan_length_corrects: dict[int, list[int]],
        plan_length_all: dict[int, int],
        domain: str,
        model: str,
        task: str,
        optimal: bool = False,
    ) -> None:
        """Generate a plan-length vs correct/optimal line plot for one model."""
        if optimal:
            df = pd.DataFrame(columns=["Optimal Plan Length", "Optimal"])
        else:
            df = pd.DataFrame(columns=["Optimal Plan Length", "Correct"])
        for plan_length in plan_length_corrects:
            if optimal:
                new_df = pd.DataFrame({
                    "Optimal Plan Length": plan_length,
                    "Optimal": plan_length_corrects[plan_length],
                })
            else:
                new_df = pd.DataFrame({
                    "Optimal Plan Length": plan_length,
                    "Correct": plan_length_corrects[plan_length],
                })
            df = pd.concat([df, new_df])

        sns.set_theme(style="darkgrid", font=self._prop.get_name())
        plt.figure(figsize=self.cfg.figsize)
        sns.set_context("poster", font_scale=self.cfg.fontscale)
        y_col = "Optimal" if optimal else "Correct"
        ax = sns.lineplot(
            data=df, x="Optimal Plan Length", y=y_col,
            linewidth=6, marker="o", errorbar=("ci", 95),
        )
        ax.set(xlabel="Plan Length", ylabel="No of Instances")
        task_base = task.split(".")[0]
        kind = "Optimal" if optimal else "Correct"
        plt.title(
            f"Plan Length vs {kind} Plans for {self.cfg.models[model]} on \n"
            f" {self.cfg.domains[domain]} - {self.cfg.tasks[task_base]}",
            fontsize=self.cfg.fontsize,
        )
        to_save = (
            f"{self.cfg.graphs_dir}/{domain}/{model}/{task_base}"
            f"/plan_length_vs_{kind.lower()}.png"
        )
        os.makedirs(os.path.dirname(to_save), exist_ok=True)
        plt.legend([f"{kind} Plans"], fontsize=self.cfg.legendfont)
        plt.savefig(to_save)
        print(f"Saved {to_save}")
        plt.close()

    # ── Multi-model comparison graphs ────────────────────────────────────

    def get_stats_graph(
        self,
        domains: list[str] | None = None,
        tasks: list[str] | None = None,
        models: list[str] | None = None,
    ) -> None:
        """Generate multi-model plan-length comparison graphs."""
        domains = domains or ["mystery_blocksworld", "blocksworld"]
        tasks = tasks or ["task_1_plan_generation_zero_shot.json"]
        models = models or [
            "o1-preview_chat", "gemini-2.5-thinking-pro", "gemini-2.5-thinking-flash",
        ]

        all_data: dict[tuple, dict] = {}
        for domain in domains:
            for model in models:
                for task in tasks:
                    key = (domain, task)
                    if key not in all_data:
                        all_data[key] = {}
                    plan_length_corrects, _, _ = self.get_data(domain, model, task)
                    if plan_length_corrects:
                        all_data[key][self.cfg.models[model]] = plan_length_corrects
                        all_data[key][self.cfg.models["fast-downward"]] = {
                            i: [1 for _ in range(len(plan_length_corrects[i]))]
                            for i in plan_length_corrects
                        }
                    else:
                        print(f"No data for {key} {model}")
                        if key in all_data:
                            del all_data[key]

        mg = self.cfg.model_group_display
        for key in all_data:
            df = pd.DataFrame(
                columns=["Plan Length", "Model Name", "Model Group", "% satisficing"]
            )
            for model_display in all_data[key]:
                model_group = None
                for k in mg:
                    if model_display in mg[k]:
                        model_group = k
                        break
                if model_group is None:
                    raise Exception(
                        f"Model {model_display} not found in any model group"
                    )
                for plan_length in all_data[key][model_display]:
                    vals = all_data[key][model_display][plan_length]
                    percent_correct = round(sum(vals) * 100 / len(vals), 2)
                    df = pd.concat([
                        df,
                        pd.DataFrame(
                            [[plan_length, model_display, model_group, percent_correct]],
                            columns=["Plan Length", "Model Name", "Model Group", "% satisficing"],
                        ),
                    ])

            task_base = key[1].split(".")[0]
            out_dir = f"{self.cfg.graphs_dir}/all_models/{key[0]}/{task_base}"
            os.makedirs(out_dir, exist_ok=True)
            df.to_csv(f"{out_dir}/all_data.csv", index=False)

            sns.set_theme(style="darkgrid", font=self._prop.get_name())
            plt.figure(figsize=self.cfg.figsize)
            sns.set_context("poster", font_scale=self.cfg.fontscale)
            palette = sns.color_palette(self.colors)
            sns.lineplot(
                data=df, x="Plan Length", y="% satisficing",
                hue="Model Name", style="Model Group",
                linewidth=8, palette=palette, markers=True,
            )
            plt.ylim(0, 105)
            plt.title(
                f"Optimal plan Length vs Correct Predictions for all Models on \n"
                f" {self.cfg.domains[key[0]]} - {self.cfg.tasks[task_base]}",
                fontsize=self.cfg.fontsize, pad=40,
            )
            to_save = f"{out_dir}/plan_length_vs_correct.png"
            plt.tight_layout()
            plt.savefig(to_save)
            plt.close()

    # ── Correct/optimal comparison ───────────────────────────────────────

    def get_new_graphs_optimal_correct(
        self,
        domains: list[str] | None = None,
        tasks: list[str] | None = None,
        models: list[str] | None = None,
    ) -> None:
        """Generate correct vs optimal comparison graphs."""
        domains = domains or [
            "mystery_blocksworld", "blocksworld", "obfuscated_randomized_blocksworld",
        ]
        tasks = tasks or ["task_1_plan_generation_zero_shot.json"]
        models = models or ["o1-preview_chat", "o1-mini_chat"]

        for model in models:
            df = pd.DataFrame(
                columns=["Plan Length", "Correct", "Optimal", "Model Name", "Domain (Zero shot)"]
            )
            for domain in domains:
                for task in tasks:
                    files = [
                        f"{self.cfg.results_dir}/{domain}/{model}/{task}",
                        f"{self.cfg.results_dir}/{domain}_3/{model}/{task}",
                    ]
                    for file in files:
                        try:
                            with open(file) as f:
                                data = json.load(f)
                        except Exception:
                            continue
                        for inst in data["instances"]:
                            if isinstance(inst["ground_truth_plan"], list):
                                inst["ground_truth_plan"] = "\n".join(
                                    inst["ground_truth_plan"]
                                )
                            plan_len = len(
                                [i for i in inst["ground_truth_plan"].split("\n") if i]
                            )
                            if "llm_correct" in inst:
                                correct = int(inst["llm_correct"]) * 100
                            elif "correct" in inst:
                                correct = int(inst["correct"]) * 100
                            else:
                                continue
                            try:
                                llm_plan_len = len(
                                    [i for i in inst["extracted_llm_plan"].split("\n") if i]
                                )
                            except Exception:
                                continue
                            optimal = 100 if (correct and llm_plan_len == plan_len) else 0
                            df = pd.concat([
                                df,
                                pd.DataFrame(
                                    [[plan_len, correct, optimal, self.cfg.models[model], self.cfg.domains[domain]]],
                                    columns=["Plan Length", "Correct", "Optimal", "Model Name", "Domain (Zero shot)"],
                                ),
                            ])

            new_df = pd.melt(
                df,
                id_vars=["Plan Length", "Model Name", "Domain (Zero shot)"],
                value_vars=["Correct", "Optimal"],
                var_name="Correct/Optimal",
            )
            sns.set_theme(style="darkgrid", font=self._prop.get_name())
            plt.figure(figsize=self.cfg.figsize)
            sns.set_context("poster", font_scale=self.cfg.fontscale)
            sns.lineplot(
                data=new_df, x="Plan Length", y="value",
                style="Correct/Optimal", hue="Domain (Zero shot)",
                linewidth=6, marker="o", palette=self.colors,
            )
            plt.title(
                f"Plan Length vs Correct/Optimal Predictions for {self.cfg.models[model]}",
                fontsize=self.cfg.fontsize, pad=40,
            )
            plt.xlabel("Plan Length", fontsize=self.cfg.fontsize)
            plt.ylabel("% satisficing/optimal", fontsize=self.cfg.fontsize, labelpad=20)
            plt.legend(fontsize=self.cfg.legendfont, loc="upper right")
            to_save = f"{self.cfg.graphs_dir}/{model}/plan_length_vs_correct_optimal.png"
            os.makedirs(os.path.dirname(to_save), exist_ok=True)
            plt.savefig(to_save)
            plt.close()

    # ── Logistics graphs ─────────────────────────────────────────────────

    def get_logistics_graphs(
        self,
        domains: list[str] | None = None,
        tasks: list[str] | None = None,
        models: list[str] | None = None,
    ) -> None:
        """Generate logistics-specific bar plots."""
        domains = domains or ["obfuscated_randomized_logistics"]
        tasks = tasks or ["task_1_plan_generation_zero_shot_pddl.json"]
        models = models or ["o1-preview_chat"]

        for model in models:
            df = pd.DataFrame(
                columns=["Plan Length", "Correct", "Optimal", "Model Name", "Domain (Zero shot)"]
            )
            for domain in domains:
                for task in tasks:
                    files = [
                        f"{self.cfg.results_dir}/{domain}/{model}/{task}",
                        f"{self.cfg.results_dir}/{domain}_3/{model}/{task}",
                    ]
                    for file in files:
                        try:
                            with open(file) as f:
                                data = json.load(f)
                        except Exception:
                            continue
                        for inst in data["instances"]:
                            if isinstance(inst["ground_truth_plan"], list):
                                inst["ground_truth_plan"] = "\n".join(
                                    inst["ground_truth_plan"]
                                )
                            plan_len = len(
                                [i for i in inst["ground_truth_plan"].split("\n") if i]
                            )
                            if "llm_correct" in inst:
                                correct = int(inst["llm_correct"]) * 100
                            elif "correct" in inst:
                                correct = int(inst["correct"]) * 100
                            else:
                                continue
                            try:
                                llm_plan_len = len(
                                    [i for i in inst["extracted_llm_plan"].split("\n") if i]
                                )
                            except Exception:
                                continue
                            optimal = 100 if (correct and llm_plan_len == plan_len) else 0
                            df = pd.concat([
                                df,
                                pd.DataFrame(
                                    [[plan_len, correct, optimal, self.cfg.models[model], self.cfg.domains[domain]]],
                                    columns=["Plan Length", "Correct", "Optimal", "Model Name", "Domain (Zero shot)"],
                                ),
                            ])

            new_df = pd.melt(
                df,
                id_vars=["Plan Length", "Model Name", "Domain (Zero shot)"],
                value_vars=["Correct"],
                var_name="Correct/Optimal",
            )
            figsize_scaled = tuple(i * 3.5 for i in self.cfg.figsize)
            sns.set_theme(style="darkgrid", font=self._prop.get_name())
            plt.figure(figsize=figsize_scaled)
            sns.set_context("poster", font_scale=self.cfg.fontscale * 2)
            sns.barplot(
                data=new_df, x="Plan Length", y="value",
                hue="Domain (Zero shot)", palette=self.colors[1:],
            )
            plt.title(
                f"Plan Length vs Correct/Optimal Predictions for {self.cfg.models[model]}",
                fontsize=self.cfg.fontsize, pad=40,
            )
            plt.xlabel("Plan Length", fontsize=self.cfg.fontsize)
            plt.ylabel("% satisficing/optimal", fontsize=self.cfg.fontsize, labelpad=20)
            plt.legend(fontsize=self.cfg.legendfont, loc=(1.05, 0.5))
            plt.tight_layout()
            to_save = f"{self.cfg.graphs_dir}/{model}/plan_length_vs_correct_optimal.png"
            os.makedirs(os.path.dirname(to_save), exist_ok=True)
            plt.savefig(to_save)
            plt.close()

    # ── Reasoning token analysis ─────────────────────────────────────────

    def reasoning_tokens_plan_length(
        self,
        result_file: str | None = None,
        domain_key: str = "mystery_blocksworld",
        task_key: str = "task_1_plan_generation_zero_shot",
        max_instance_id: int = 300,
    ) -> None:
        """Plot reasoning tokens vs plan length for a single result file."""
        if result_file is None:
            result_file = (
                f"{self.cfg.results_dir}/{domain_key}/o1-preview_chat"
                f"/{task_key}_run2.json"
            )
        with open(result_file) as f:
            data = json.load(f)

        df = pd.DataFrame(columns=["Plan Length", "Reasoning Tokens"])
        for inst in data["instances"]:
            if inst["instance_id"] > max_instance_id:
                continue
            if isinstance(inst["ground_truth_plan"], list):
                inst["ground_truth_plan"] = "\n".join(inst["ground_truth_plan"])
            plan_len = len(
                [i for i in inst["ground_truth_plan"].split("\n") if i]
            )
            reasoning_tokens = inst["response_object"]["usage"][
                "completion_tokens_details"
            ]["reasoning_tokens"]
            df = pd.concat([
                df,
                pd.DataFrame(
                    [[plan_len, reasoning_tokens]],
                    columns=["Plan Length", "Reasoning Tokens"],
                ),
            ])

        sns.set_theme(style="darkgrid", font=self._prop.get_name())
        plt.figure(figsize=self.cfg.figsize)
        sns.set_context("poster", font_scale=self.cfg.fontscale)
        sns.lineplot(
            data=df, x="Plan Length", y="Reasoning Tokens",
            linewidth=6, marker="o", errorbar=("ci", 95),
        )
        plt.xlabel("Plan Length", fontsize=self.cfg.fontsize)
        plt.ylabel("Average Reasoning Tokens", fontsize=self.cfg.fontsize, labelpad=20)
        plt.ylim(1000, 10000)
        plt.yticks(np.arange(2000, 12000, 2000))
        plt.title(
            f"Plan Length vs Reasoning Tokens for O1 Preview on \n"
            f"{self.cfg.domains[domain_key]} - {self.cfg.tasks[task_key]}",
            fontsize=self.cfg.fontsize, pad=40,
        )
        to_save = (
            f"{self.cfg.graphs_dir}/{domain_key}/o1-preview_chat"
            f"/{task_key}/plan_length_vs_reasoning_tokens.png"
        )
        os.makedirs(os.path.dirname(to_save), exist_ok=True)
        plt.savefig(to_save)
        plt.close()

    def obfuscated_reasoning_tokens_plan_length(
        self,
        result_file: str | None = None,
        domain_key: str = "obfuscated_randomized_blocksworld",
        task_key: str = "task_1_plan_generation_zero_shot",
    ) -> None:
        """Plot reasoning tokens for obfuscated domain results."""
        if result_file is None:
            result_file = (
                f"{self.cfg.results_dir}/{domain_key}/o1-preview_chat"
                f"/{task_key}_pddl.json"
            )
        with open(result_file) as f:
            data = json.load(f)

        df = pd.DataFrame(columns=["Plan Length", "Reasoning Tokens"])
        for inst in data["instances"]:
            if isinstance(inst["ground_truth_plan"], list):
                inst["ground_truth_plan"] = "\n".join(inst["ground_truth_plan"])
            plan_len = len(
                [i for i in inst["ground_truth_plan"].split("\n") if i]
            )
            try:
                reasoning_tokens = inst["response_object"]["usage"][
                    "completion_tokens_details"
                ]["reasoning_tokens"]
            except (KeyError, TypeError):
                continue
            df = pd.concat([
                df,
                pd.DataFrame(
                    [[plan_len, reasoning_tokens]],
                    columns=["Plan Length", "Reasoning Tokens"],
                ),
            ])

        sns.set_theme(style="darkgrid", font=self._prop.get_name())
        plt.figure(figsize=self.cfg.figsize)
        sns.set_context("poster", font_scale=self.cfg.fontscale)
        sns.lineplot(
            data=df, x="Plan Length", y="Reasoning Tokens",
            linewidth=6, marker="o", errorbar=("ci", 95),
        )
        plt.xlabel("Plan Length", fontsize=self.cfg.fontsize)
        plt.ylabel("Average Reasoning Tokens", fontsize=self.cfg.fontsize, labelpad=20)
        plt.legend(["Reasoning Tokens"], fontsize=self.cfg.legendfont, loc="upper left")
        plt.title(
            f"Plan Length vs Reasoning Tokens for O1 Preview on \n"
            f"{self.cfg.domains[domain_key]} - {self.cfg.tasks[task_key]} PDDL",
            fontsize=self.cfg.fontsize, pad=40,
        )
        to_save = (
            f"{self.cfg.graphs_dir}/{domain_key}/o1-preview_chat"
            f"/{task_key}/plan_length_vs_reasoning_tokens.png"
        )
        os.makedirs(os.path.dirname(to_save), exist_ok=True)
        plt.savefig(to_save)
        print(f"Saved {to_save}")
        plt.close()

    # ── New blocksworld extended analysis ────────────────────────────────

    def new_blocksworld_graphs(
        self,
        result_file: str | None = None,
        info_file: str | None = None,
    ) -> None:
        """Generate extended blocksworld analysis graphs (reasoning, correct, nodes)."""
        if result_file is None:
            result_file = (
                f"{self.cfg.results_dir}/../plan-bench/results/new_blocksworld"
                f"/o1-preview_chat/task_1_plan_generation_zero_shot_pddl.json"
            )
        if info_file is None:
            info_file = (
                f"{self.cfg.results_dir}/../plan-bench/instances/new_blocksworld"
                f"/new_blocksworld_info.json"
            )

        with open(result_file) as f:
            data1 = json.load(f)
        with open(info_file) as f:
            info = json.load(f)

        df1 = pd.DataFrame(columns=["Plan Length", "Reasoning Tokens"])
        df2 = pd.DataFrame(columns=["Plan Length", "% satisficing", "Optimal"])
        df5 = pd.DataFrame(columns=["Nodes Expanded", "Reasoning Tokens"])

        for i_inst in data1["instances"]:
            block, inst_id = i_inst["instance_id"].split("_")
            plan_len = int(block)
            reasoning_tokens = i_inst["response_object"]["usage"][
                "completion_tokens_details"
            ]["reasoning_tokens"]
            correct = int(i_inst["llm_correct"]) * 100
            df1 = pd.concat([
                df1,
                pd.DataFrame(
                    [[plan_len, reasoning_tokens]],
                    columns=["Plan Length", "Reasoning Tokens"],
                ),
            ])

            llm_plan_len = len(
                [x for x in i_inst["extracted_llm_plan"].split("\n") if x]
            )
            optimal = 100 if (correct and llm_plan_len == plan_len) else 0
            df2 = pd.concat([
                df2,
                pd.DataFrame(
                    [[plan_len, correct, optimal]],
                    columns=["Plan Length", "% satisficing", "Optimal"],
                ),
            ])
            nodes_expanded = info[block][f"instance-{inst_id}.pddl"]["states_info"]["expanded"]
            df5 = pd.concat([
                df5,
                pd.DataFrame(
                    [[nodes_expanded, reasoning_tokens]],
                    columns=["Nodes Expanded", "Reasoning Tokens"],
                ),
            ])

        mean_correct = df2["% satisficing"].mean()
        task_key = "task_1_plan_generation_zero_shot"
        figsize_wide = (40, 19)
        base_dir = f"{self.cfg.graphs_dir}/new_blocksworld/o1-preview_chat/{task_key}"
        os.makedirs(base_dir, exist_ok=True)

        # Reasoning tokens vs plan length
        sns.set_theme(style="darkgrid", font=self._prop.get_name())
        plt.figure(figsize=figsize_wide)
        sns.set_context("poster", font_scale=self.cfg.fontscale)
        sns.lineplot(
            data=df1, x="Plan Length", y="Reasoning Tokens",
            linewidth=6, marker="o", errorbar=("ci", 95),
        )
        plt.xlabel("Optimal plan length", fontsize=self.cfg.fontsize)
        plt.ylabel("Average Reasoning Tokens", fontsize=self.cfg.fontsize, labelpad=20)
        plt.xticks(np.arange(2, 41, 2))
        plt.ylim(1000, 10000)
        plt.yticks(np.arange(2000, 12000, 2000))
        plt.title(
            f"Optimal plan length vs Reasoning Tokens for O1 Preview on \n"
            f"Blocksworld - {self.cfg.tasks[task_key]}",
            fontsize=self.cfg.fontsize, pad=40,
        )
        plt.tight_layout()
        plt.savefig(f"{base_dir}/plan_length_vs_reasoning_tokens_bw.png")
        plt.close()

        # Plan length vs correct
        df2_melted = pd.melt(
            df2, id_vars=["Plan Length"],
            value_vars=["% satisficing"], var_name="variable",
        )
        sns.set_theme(style="darkgrid", font=self._prop.get_name())
        plt.figure(figsize=figsize_wide)
        sns.set_context("poster", font_scale=self.cfg.fontscale)
        sns.lineplot(
            data=df2_melted, x="Plan Length", y="value",
            hue="variable", linewidth=6, marker="o",
            style="variable", palette=self.colors,
        )
        plt.axhline(y=mean_correct, color=self.colors[0], linestyle="--", label="Avg Satisficing")
        plt.xlabel("Optimal plan length", fontsize=self.cfg.fontsize)
        plt.ylabel("% satisficing", fontsize=self.cfg.fontsize, labelpad=20)
        plt.legend(fontsize=self.cfg.legendfont, loc="upper right")
        plt.xticks(np.arange(2, 41, 2))
        plt.ylim(0, 105)
        plt.title(
            f"Optimal plan length vs Satisficing plans by O1 Preview on \n"
            f"Blocksworld - {self.cfg.tasks[task_key]}",
            fontsize=self.cfg.fontsize, pad=40,
        )
        plt.tight_layout()
        plt.savefig(f"{base_dir}/plan_length_vs_correct_bw_hard.png")
        plt.close()

        # Nodes expanded vs reasoning tokens
        sns.set_theme(style="darkgrid", font=self._prop.get_name())
        plt.figure(figsize=figsize_wide)
        sns.set_context("poster", font_scale=self.cfg.fontscale)
        sns.scatterplot(
            data=df5, x="Nodes Expanded", y="Reasoning Tokens",
            s=250, color=self.colors[0],
        )
        plt.xlabel("Nodes Expanded", fontsize=self.cfg.fontsize, labelpad=20)
        plt.ylabel("Average Reasoning Tokens", fontsize=self.cfg.fontsize, labelpad=20)
        plt.xticks(np.arange(0, df5["Nodes Expanded"].max() + 200000, 200000))
        plt.title(
            f"Nodes Expanded vs Reasoning Tokens for O1 Preview on \n"
            f"Blocksworld - {self.cfg.tasks[task_key]}",
            fontsize=self.cfg.fontsize, pad=40,
        )
        plt.tight_layout()
        plt.savefig(f"{base_dir}/nodes_expanded_vs_reasoning_tokens.png")
        plt.close()

    # ── Comparison utilities ─────────────────────────────────────────────

    def compare_llama_results(
        self,
        file1: str | None = None,
        file2: str | None = None,
    ) -> None:
        """Compare two llama result files for correctness differences."""
        if file1 is None:
            file1 = (
                f"{self.cfg.results_dir}/blocksworld/llama-3.1-405b_aws"
                f"/task_1_plan_generation_zero_shot.json"
            )
        if file2 is None:
            file2 = (
                f"{self.cfg.results_dir}/blocksworld/llama-3.1-405b_aws"
                f"/task_1_plan_generation_zero_shot_1.json"
            )
        with open(file1) as f:
            data1 = json.load(f)
        with open(file2) as f:
            data2 = json.load(f)
        for inst1, inst2 in zip(data1["instances"], data2["instances"]):
            if "llm_correct" not in inst1 or "llm_correct" not in inst2:
                print(inst1["instance_id"], inst2["instance_id"])
                continue
            if inst1["llm_correct"] != inst2["llm_correct"]:
                print(
                    inst1["instance_id"], inst2["instance_id"],
                    inst1["llm_correct"], inst2["llm_correct"],
                )

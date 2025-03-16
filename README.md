# LLMs and Planning

This repo has the code for three papers:
1. The code in 'plan-bench' subdirectory belongs to the paper ["PlanBench: An Extensible Benchmark for Evaluating Large Language Models on Planning and Reasoning about Change"](https://arxiv.org/abs/2206.10498)
2. The code in 'llm_planning_analysis' subdirectory belongs to the paper ["On the Planning Abilities of Large Language Models--A Critical Investigation"](https://arxiv.org/abs/2305.15771)
3. **NEW**: 'llm_planning_analysis' subdirectory also contains the code for the paper ["Planning in Strawberry Fields:
Evaluating and Improving the Planning and
Scheduling Capabilities of LRM o1"](https://arxiv.org/pdf/2410.02162)

# PlanBench Static Test Set Leaderboard

The leaderboard below shows the performance of the models on the PlanBench static test set with zero-shot prompting. Check out llm_planning_analysis/results/ folder for the detailed files. For Blocksworld Hard, the results are included in results/backprompting/ folder.

| Model Name | Model Type | Blocksworld - NL - 600 instances | Mystery Blocksworld - NL - 600 instances | Randomized Mystery Blocksworld - NL - 600 instances | Blocksworld Hard - PDDL - 110 instances |
|------------|------------|----------------------------------|----------------------------------------|--------------------------------------------------|----------------------------------------|
| Deepseek R1 | LRM | 99.1% | 54.1% | 25.8% | 53.6% |
| o1-mini | LRM | 56.6% | 19.1% | 3.5% | 10% |
| o1-preview | LRM | 97.8% | 52.8% | 37.3% | 23.65% |
| Claude-3.5 Sonnet | LLM | 54.8% | 0% | - | - |
| GPT-4o | LLM | 35.5% | 0% | - | - |
| LLaMA-3.1 405B | LLM | 62.6% | 0.8% | - | - |
| Claude 3 Opus | LLM | 59.3% | 0% | - | - |
| LLaMA-3 70B | LLM | 34.16% | 0% | - | - |
| GPT-4 | LLM | 34.6% | 0% | - | - |
| Gemini 1.5 Pro | LLM | 23.8% | - | - | - |

## Submitting to the Leaderboard

Kindly submit results of any new models by submitting a pull request with the result file and the leaderboard will be updated.

## Citation(s)

PlanBench - _NeurIPS 2023 Datasets and Benchmarks Track_:
```
@article{valmeekam2023planbench,
  title={Planbench: An extensible benchmark for evaluating large language models on planning and reasoning about change},
  author={Valmeekam, Karthik and Marquez, Matthew and Olmo, Alberto and Sreedharan, Sarath and Kambhampati, Subbarao},
  journal={Advances in Neural Information Processing Systems},
  volume={36},
  pages={38975--38987},
  year={2023}
}
```

On the Planning Abilities of Large Language Models - _NeurIPS 2023 Spotlight_:
```
@article{valmeekam2023planning,
  title={On the planning abilities of large language models-a critical investigation},
  author={Valmeekam, Karthik and Marquez, Matthew and Sreedharan, Sarath and Kambhampati, Subbarao},
  journal={Advances in Neural Information Processing Systems},
  volume={36},
  pages={75993--76005},
  year={2023}
}
```

Planning in Strawberry Fields - _A version to appear in TMLR_:
```
@article{valmeekam2024planning,
  title={Planning in Strawberry Fields: Evaluating and Improving the Planning and Scheduling Capabilities of LRM o1},
  author={Valmeekam, Karthik and Stechly, Kaya and Gundawar, Atharva and Kambhampati, Subbarao},
  journal={arXiv preprint arXiv:2410.02162},
  year={2024}
}
```

## Star History

[![Star History Chart](https://api.star-history.com/svg?repos=karthikv792/LLMs-Planning&type=Date)](https://www.star-history.com/#karthikv792/LLMs-Planning&Date)
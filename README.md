# LLMs and Planning

This repo has the code for three papers:
1. The code in 'plan-bench' subdirectory belongs to the paper ["PlanBench: An Extensible Benchmark for Evaluating Large Language Models on Planning and Reasoning about Change"](https://arxiv.org/abs/2206.10498)
2. The code in 'llm_planning_analysis' subdirectory belongs to the paper ["On the Planning Abilities of Large Language Models--A Critical Investigation"](https://arxiv.org/abs/2305.15771)
3. **NEW**: 'llm_planning_analysis' subdirectory also contains the code for the paper ["Planning in Strawberry Fields:
Evaluating and Improving the Planning and
Scheduling Capabilities of LRM o1"](https://arxiv.org/pdf/2410.02162)

# PlanBench Static Test Set Leaderboard

The leaderboard below shows the performance of the models on the PlanBench static test set with zero-shot prompting. Check out llm_planning_analysis/results/ folder for the detailed files. For Blocksworld Hard, the results are included in results/backprompting/ folder.

<link href="https://fonts.googleapis.com/css2?family=Google+Sans:wght@400;500&display=swap" rel="stylesheet">

<style>
.planbench-table {
    width: 100%;
    border-collapse: collapse;
    font-family: 'Google Sans', sans-serif;
    margin: 20px 0;
    background: white;
    border-radius: 8px;
    overflow: hidden;
    box-shadow: 0 1px 3px rgba(0,0,0,0.2);
}

.percentage-low {
    color: rgba(219, 68, 55, 0.8) !important; 
}

.percentage-mid {
    color: rgba(251, 188, 4, 0.8) !important;
}

.percentage-high {
    color: rgba(15, 157, 88, 0.8) !important; 
}

.planbench-table th {
    background: #174EA6;
    color: white;
    font-weight: 500;
    padding: 12px;
    text-align: left;
    cursor: pointer;
    transition: background-color 0.2s;
}

.planbench-table th:hover {
    background: #357abd;
}

.planbench-table td {
    padding: 12px;
    border-bottom: 1px solid #e0e0e0;
    color: #333;
}

.planbench-table tr:last-child td {
    border-bottom: none;
}
.planbench-table tr:hover {
    background-color: #f1f3f4;
}

.sort-arrow {
    display: inline-block;
    width: 0;
    height: 0;
    margin-left: 5px;
    vertical-align: middle;
}

.sort-arrow.asc {
    border-left: 4px solid transparent;
    border-right: 4px solid transparent;
    border-bottom: 4px solid white;
}

.sort-arrow.desc {
    border-left: 4px solid transparent;
    border-right: 4px solid transparent;
    border-top: 4px solid white;
}
</style>

<table class="planbench-table">
  <thead>
    <tr>
      <th onclick="sortTable(0)">Model Name<span class="sort-arrow"></span></th>
      <th onclick="sortTable(1)">Model Type<span class="sort-arrow"></span></th>
      <th onclick="sortTable(2)">Blocksworld - NL - 600 instances <span class="sort-arrow"></span></th>
      <th onclick="sortTable(3)">Mystery Blocksworld - NL - 600 instances<span class="sort-arrow"></span></th>
      <th onclick="sortTable(4)">Randomized Mystery Blocksworld - NL - 600 instances<span class="sort-arrow"></span></th>
      <th onclick="sortTable(5)">Blocksworld Hard - PDDL - 110 instances <span class="sort-arrow"></span></th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>Claude-3.5 Sonnet</td>
      <td><span style="background-color: rgba(219, 68, 55, 0.1); color: #A50E0E; padding: 4px 8px; border-radius: 12px;">LLM</span></td>
      <td>54.8%</td>
      <td>0%</td>
      <td>-</td>
      <td>-</td>
    </tr>
    <tr>
      <td>GPT-4o</td>
      <td><span style="background-color: rgba(219, 68, 55, 0.1); color: #A50E0E; padding: 4px 8px; border-radius: 12px;">LLM</span></td>
      <td>35.5%</td>
      <td>0%</td>
      <td>-</td>
      <td>-</td>
    </tr>
    <tr>
      <td>GPT-4</td>
      <td><span style="background-color: rgba(219, 68, 55, 0.1); color: #A50E0E; padding: 4px 8px; border-radius: 12px;">LLM</span></td>
      <td>34.6%</td>
      <td>0%</td>
      <td>-</td>
      <td>-</td>
    </tr>
    <tr>
      <td>LLaMA-3.1 405B</td>
      <td><span style="background-color: rgba(219, 68, 55, 0.1); color: #A50E0E; padding: 4px 8px; border-radius: 12px;">LLM</span></td>
      <td>62.6%</td>
      <td>0.8%</td>
      <td>-</td>
      <td>-</td>
    </tr>
    <tr>
      <td>LLaMA-3 70B</td>
      <td><span style="background-color: rgba(219, 68, 55, 0.1); color: #A50E0E; padding: 4px 8px; border-radius: 12px;">LLM</span></td>
      <td>34.16%</td>
      <td>0%</td>
      <td>-</td>
      <td>-</td>
    </tr>
    <tr>
      <td>Gemini 1.5 Pro</td>
      <td><span style="background-color: rgba(219, 68, 55, 0.1); color: #A50E0E; padding: 4px 8px; border-radius: 12px;">LLM</span></td>
      <td>23.8%</td>
      <td>-</td>
      <td>-</td>
      <td>-</td>
    </tr>
    <tr>
      <td>Claude 3 Opus</td>
      <td><span style="background-color: rgba(219, 68, 55, 0.1); color: #A50E0E; padding: 4px 8px; border-radius: 12px;">LLM</span></td>
      <td>59.3%</td>
      <td>0%</td>
      <td>-</td>
      <td>-</td>
    </tr>
    <!-- LRMs -->
    <tr>
      <td>o1-preview</td>
      <td><span style="background-color: rgba(251, 188, 4, 0.1); color: #E37400; padding: 4px 8px; border-radius: 12px;">LRM</span></td>
      <td>97.8%</td>
      <td>52.8%</td>
      <td>37.3%</td>
      <td>23.65%</td>
    </tr>
    <tr>
      <td>o1-mini</td>
      <td><span style="background-color: rgba(251, 188, 4, 0.1); color: #E37400; padding: 4px 8px; border-radius: 12px;">LRM</span></td>
      <td>56.6%</td>
      <td>19.1%</td>
      <td>3.5%</td>
      <td>10%</td>
    </tr>
    <tr>
      <td>Deepseek R1</td>
      <td><span style="background-color: rgba(251, 188, 4, 0.1); color: #E37400; padding: 4px 8px; border-radius: 12px;">LRM</span></td>
      <td>99.1%</td>
      <td>54.1%</td>
      <td>25.8%</td>
      <td>53.6%</td>
    </tr>
  </tbody>
</table>

<script>
function sortTable(n) {
    var table, rows, switching, i, x, y, shouldSwitch, dir, switchcount = 0;
    table = document.querySelector(".planbench-table");
    switching = true;
    dir = "asc";
    
    // Reset all arrows
    var arrows = table.getElementsByClassName("sort-arrow");
    for(var i = 0; i < arrows.length; i++) {
        arrows[i].className = "sort-arrow";
    }
    
    while (switching) {
        switching = false;
        rows = table.rows;
        
        for (i = 1; i < (rows.length - 1); i++) {
            shouldSwitch = false;
            x = rows[i].getElementsByTagName("TD")[n];
            y = rows[i + 1].getElementsByTagName("TD")[n];
            
            var xValue = x.innerHTML.toLowerCase();
            var yValue = y.innerHTML.toLowerCase();
            
            // If it's a percentage column, convert to number
            if(xValue.includes("%")) {
                xValue = parseFloat(xValue);
                yValue = parseFloat(yValue);
            }
            
            if (dir == "asc") {
                if (xValue > yValue) {
                    shouldSwitch = true;
                    break;
                }
            } else if (dir == "desc") {
                if (xValue < yValue) {
                    shouldSwitch = true;
                    break;
                }
            }
        }
        
        if (shouldSwitch) {
            rows[i].parentNode.insertBefore(rows[i + 1], rows[i]);
            switching = true;
            switchcount++;
        } else {
            if (switchcount == 0 && dir == "asc") {
                dir = "desc";
                switching = true;
            }
        }
    }
    
    // Update arrow
    arrows[n].className = "sort-arrow " + dir;
}
function setPercentageColors() {
    const cells = document.querySelectorAll('.planbench-table tbody td:nth-child(n+3)');
    cells.forEach(cell => {
        const value = cell.textContent.trim();
        if (value !== '-') {
            const percentage = parseFloat(value);
            if (!isNaN(percentage)) {
                cell.classList.remove('percentage-low', 'percentage-mid', 'percentage-high');
                if (percentage < 33) {
                    cell.classList.add('percentage-low');
                } else if (percentage < 66) {
                    cell.classList.add('percentage-mid');
                } else {
                    cell.classList.add('percentage-high');
                }
            }
        }
    });
}

document.addEventListener('DOMContentLoaded', function() {
    setPercentageColors();
    sortTable(3);
    sortTable(3);
});
</script>


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
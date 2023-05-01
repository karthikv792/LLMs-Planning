# LLM Planning Assessment Suite

### Requirements
- Linux
- Python 3.6+ 
  - Install required packages with `pip install -r requirements.txt`
- Fast Downward 
    1. Download from [here](https://www.fast-downward.org/ObtainingAndRunningFastDownward) 
    2. Assign path of the folder to the environment variable FAST_DOWNWARD `FAST_DOWNWARD=/path/to/fast_downward`
- Val 
    1. Download from [here](https://www.fast-downward.org/SettingUpVal) 
    2. Assign path of the folder to the environment variable VAL `VAL=/path/to/val`
- PR2Plan
    1. Download and compile obs-compiler from [here](https://sites.google.com/site/prasplanning/file-cabinet)
    2. Assign path of the folder to the environment variable PR2 `PR2=/path/to/pr2plan`
    3. Incase there are any issues in compiling the above, this executable can be used: https://github.com/karthikv792/RADAR-X/blob/master/planner/PR2/pr2plan
- LLM access - (currently GPT-3)

## Running the experiments on GPT-3
For Blocksworld:
```
python3 ReasoningTasks.py --task t1 --engine davinci --verbose False
```
The --task argument corresponds to the various tasks involved:
- t1 = Goal Directed Reasoning
- t2 = Goal Reformulation 
- t3 = Plan Reuse 
- t4 = Plan Generalization
- t5 = Optimal Planning
- t6 = Replanning 
- t7 = Plan Execution 

The --engine argument corresponds to the various GPT-3 engines

## Running the experiments on a different domain
1. Add a .yaml file containing the specifics of the domain (refer to the existing config file for blocksworld)
2. In ReasoningTasks.py, replace the path to the config file for the task that you chose to run
3. Use the same command as above to run the experiments.

## Running the experiments on a different LLM
1. To do so, replace the send_query_gpt3 function in utils/\_\_init__.py with the required LLM querying code.

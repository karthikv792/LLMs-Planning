import os
import subprocess
def compute_plan(domain, instance):
    fast_downward_path = os.getenv("FAST_DOWNWARD")
    # Remove > /dev/null to see the output of fast-downward
    assert os.path.exists(f"{fast_downward_path}/fast-downward.py")
    cmd = f"{fast_downward_path}/fast-downward.py {domain} {instance} --search \"astar(lmcut())\""
    result = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
    output, error = result.communicate()
    result.wait()

import json
import os
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
import pandas as pd
from matplotlib.font_manager import fontManager, FontProperties

#Download the font from https://fonts.google.com/specimen/Poppins
path = "/home/local/ASURITE/kvalmeek/Poppins/Poppins-SemiBold.ttf"
# path=''
fontManager.addfont(path)

prop = FontProperties(fname=path)
"""
$palette-name: 'Spectral-11'
$palette-color1: #9e0142;
$palette-color2: #d53e4f;
$palette-color3: #f46d43;
$palette-color4: #fdae61;
$palette-color5: #fee08b;
$palette-color6: #ffffbf;
$palette-color7: #e6f598;
$palette-color8: #abdda4;
$palette-color9: #66c2a5;
$palette-color10: #3288bd;
$palette-color11: #5e4fa2;
"""
# colors = ["#e60049", "#0bb4ff", "#50e991", "#e6d800", "#9b19f5", "#ffa300", "#dc0ab4", "#b3d4ff", "#00bfa0"]

# colors = ['#0D652D','#A50E0E','#E37400','#174EA6','#283044']
# colors = ['#40076E',
# '#1E049F',
# '#002DD1',
# '#7A070C',
# '#AC034B',
# '#DD00B3',
# '#5A8606',
# '#B9B101',
# '#E88500',
# '#05936F',
# '#00C648']
colors = sns.color_palette("deep", 11)
FONTSCALE = 2
FONTSIZE = 55
LEGENDFONT = 45
FIGSIZE = (30, 28)
MODELS = {
    "o1-preview_chat": "O1 Preview",
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
    "fast-downward": "Fast Downward"
}
MODEL_GROUPS = {
    "o1 Models (LRMs)": ["o1-preview_chat", "o1-mini_chat"],
    "LLMs": ["llama-3.1-405b_aws","gpt-4o-mini-2024-07-18_chat", "gpt-4o_chat","gpt-4-turbo_chat", "gpt-4_chat","gemini-1.5-pro", "gemini-1.5-flash","claude-3.5-sonnet_aws", "claude-3-opus"],
    "Fast Downward": ["fast-downward"]
}
MG = {}
for model_group in MODEL_GROUPS:
    MG[model_group] = []
    for model in MODEL_GROUPS[model_group]:
        MG[model_group].append(MODELS[model])
    
DOMAINS = {
    "mystery_blocksworld": "Mystery Blocksworld",
    "blocksworld": "Blocksworld",
    "obfuscated_randomized_blocksworld": "Randomized Mystery Blocksworld",
    "logistics": "Logistics",
    "obfuscated_randomized_logistics": "Randomized Logistics"
}
TASKS = {
    "task_1_plan_generation_zero_shot": "Plan Generation Zero Shot",
    "task_1_plan_generation": "Plan Generation One Shot",
    "task_1_plan_generation_zero_shot_pddl": "Plan Generation Zero Shot PDDL"
}
def get_data(domain, model, task):
    files = [f"results/{domain}/{model}/{task}", f"results/{domain}_3/{model}/{task}"]
    plan_length_all = {}
    plan_length_corrects = {}
    plan_length_optimal = {}
    for file in files:
        try:
            with open(file) as f:
                data = json.load(f)
        except:
            continue
        for inst in data["instances"]:
            if isinstance(inst["ground_truth_plan"], list):
                inst["ground_truth_plan"] = '\n'.join(inst["ground_truth_plan"])
            plan_len = len([i for i in inst["ground_truth_plan"].split('\n') if i])
            if plan_len not in plan_length_corrects:
                plan_length_corrects[plan_len] = []
                plan_length_all[plan_len] = 0
            try:
                plan_length_corrects[plan_len].append(int(inst["llm_correct"]))
            except:
                try:
                    plan_length_corrects[plan_len].append(int(inst["correct"]))
                except:
                    print(file, inst["instance_id"])
                    raise Exception("llm_correct not found")
            try:
                llm_plan_len = len([i for i in inst["extracted_llm_plan"].split('\n') if i])
            except:
                # raise Exception(f"Extracted llm plan not found in {file}")
                continue
            try:
                if inst["llm_correct"] and llm_plan_len == plan_len:
                    new_l = plan_length_optimal.get(plan_len, [])
                    new_l.append(1)
                    plan_length_optimal[plan_len] = new_l
                else:
                    new_l = plan_length_optimal.get(plan_len, [])
                    new_l.append(0)
                    plan_length_optimal[plan_len] = new_l
            except:
                try:
                    if inst["correct"] and llm_plan_len == plan_len:
                        new_l = plan_length_optimal.get(plan_len, [])
                        new_l.append(1)
                        plan_length_optimal[plan_len] = new_l
                    else:
                        new_l = plan_length_optimal.get(plan_len, [])
                        new_l.append(0)
                        plan_length_optimal[plan_len] = new_l
                except:
                    print(file, inst["instance_id"])
                    raise Exception("llm_correct not found")    
            plan_length_all[plan_len] += 1
    # Get the percentage of correct predictions
    # for key in plan_length_corrects:
    #     plan_length_corrects[key] = round(plan_length_corrects[key]*100 / plan_length_all[key], 2)
    # for key in plan_length_optimal:
    #     plan_length_optimal[key] = round(plan_length_optimal[key]*100 / plan_length_all[key], 2)
    # for key in plan_length_all:
    #     if key not in plan_length_corrects:
    #         plan_length_corrects[key] = 0
    #     if key not in plan_length_optimal:
    #         plan_length_optimal[key] = 0
    return plan_length_corrects, plan_length_optimal, plan_length_all
def get_stats(plan_length_corrects, plan_length_all, domain, model, task, optimal=False):
    #Convert the dictionary to a pandas dataframe long form
    # ["Optimal Plan Length", "Correct"]
    if optimal:
        df = pd.DataFrame(columns=["Optimal Plan Length", "Optimal"])
    else:
        df = pd.DataFrame(columns=["Optimal Plan Length", "Correct"])
    for plan_length in plan_length_corrects:
        if optimal:
            new_df = pd.DataFrame({"Optimal Plan Length": plan_length, "Optimal": plan_length_corrects[plan_length]})
            print(new_df)
            df = pd.concat([df, new_df])
        else:
            new_df = pd.DataFrame({"Optimal Plan Length": plan_length, "Correct": plan_length_corrects[plan_length]})
            print(new_df)
            df = pd.concat([df, new_df])
    # Seaborn graph for plan length vs correct
    sns.set_theme(style="darkgrid", font=prop.get_name())
    fig = plt.figure(figsize=FIGSIZE)
    sns.set_context("poster", font_scale=FONTSCALE)
    # 95% confidence interval
    ax = sns.lineplot(data=df, x="Optimal Plan Length", y="Optimal" if optimal else "Correct", linewidth=6, marker='o', errorbar=('ci', 95))
    # sns.lineplot(x=list(plan_length_all.keys()), y=[plan_length_all[key] for key in plan_length_all], linewidth=6, marker='o')
    if not optimal:
        ax.set(xlabel='Plan Length', ylabel='No of Instances')
        plt.title(f"Plan Length vs Correct Plans for {MODELS[model]} on \n {DOMAINS[domain]} - {TASKS[task.split('.')[0]]}", fontsize=FONTSIZE)
        to_save = f"graphs/{domain}/{model}/{task.split('.')[0]}/plan_length_vs_correct.png"
        plt.legend(["Correct Plans"], fontsize=LEGENDFONT)
    else:
        ax.set(xlabel='Plan Length', ylabel='No of Instances')
        plt.title(f"Plan Length vs Optimal Plans for {MODELS[model]} on \n {DOMAINS[domain]} - {TASKS[task.split('.')[0]]}", fontsize=FONTSIZE)
        to_save = f"graphs/{domain}/{model}/{task.split('.')[0]}/plan_length_vs_optimal.png"
        plt.legend(["Optimal Plans"], fontsize=LEGENDFONT)
    
    os.makedirs(os.path.dirname(to_save), exist_ok=True)
    plt.savefig(to_save)
    print(f"Saved {to_save}")
    plt.close()

def get_new_graphs_optimal_correct():
    domains = ["mystery_blocksworld", "blocksworld", "obfuscated_randomized_blocksworld"]
    tasks = ["task_1_plan_generation_zero_shot.json"]
    models = ["o1-preview_chat", "o1-mini_chat"]
    
    for model in models:
        df = pd.DataFrame(columns=["Plan Length", "Correct", "Optimal", "Model Name", "Domain (Zero shot)"])
        for domain in domains:
            for task in tasks:
                files = [f"results/{domain}/{model}/{task}", f"results/{domain}_3/{model}/{task}"]
                for file in files:
                    try:
                        with open(file) as f:
                            data = json.load(f)
                    except:
                        continue
                    for inst in data["instances"]:
                        if isinstance(inst["ground_truth_plan"], list):
                            inst["ground_truth_plan"] = '\n'.join(inst["ground_truth_plan"])
                        plan_len = len([i for i in inst["ground_truth_plan"].split('\n') if i])
                        if "llm_correct" in inst:
                            correct = int(inst["llm_correct"])*100
                        elif "correct" in inst:
                            correct = int(inst["correct"])*100
                        else:
                            continue
                        try:
                            llm_plan_len = len([i for i in inst["extracted_llm_plan"].split('\n') if i])
                        except:
                            continue
                        if correct and llm_plan_len == plan_len:
                            optimal = 100
                        else:
                            optimal = 0
                        df = pd.concat([df, pd.DataFrame([[plan_len, correct, optimal, MODELS[model], DOMAINS[domain]]], columns=["Plan Length", "Correct", "Optimal", "Model Name", "Domain (Zero shot)"])])
        mean_correct = df["Correct"].mean()
        mean_optimal = df["Optimal"].mean()
        new_df = pd.melt(df, id_vars=["Plan Length", "Model Name", "Domain (Zero shot)"], value_vars=["Correct", "Optimal"], var_name="Correct/Optimal")
        sns.set_theme(style="darkgrid", font=prop.get_name())
        fig = plt.figure(figsize=FIGSIZE)
        sns.set_context("poster", font_scale=FONTSCALE)
        ax = sns.lineplot(data=new_df, x="Plan Length", y="value", style="Correct/Optimal", hue="Domain (Zero shot)", linewidth=6, marker='o', palette=colors)
        # plt.axhline(y=mean_correct, color=colors[0], linestyle='--', label='Avg Correct')
        # plt.axhline(y=mean_optimal, color=colors[1], linestyle='--', label='Avg Optimal')
        plt.title(f"Plan Length vs Correct/Optimal Predictions for {MODELS[model]}", fontsize=FONTSIZE, pad=40)
        plt.xlabel('Plan Length', fontsize=FONTSIZE)
        plt.ylabel('% satisficing/optimal', fontsize=FONTSIZE, labelpad=20)
        plt.legend(fontsize=LEGENDFONT, loc='upper right')
        to_save = f"graphs/{model}/plan_length_vs_correct_optimal.png"
        os.makedirs(os.path.dirname(to_save), exist_ok=True)
        plt.savefig(to_save)
        plt.close()

def get_logistics_graphs():
    domains = ["obfuscated_randomized_logistics"]
    tasks = ["task_1_plan_generation_zero_shot_pddl.json"]
    models = ["o1-preview_chat"]
    for model in models:
        df = pd.DataFrame(columns=["Plan Length", "Correct", "Optimal", "Model Name", "Domain (Zero shot)"])
        for domain in domains:
            for task in tasks:
                files = [f"results/{domain}/{model}/{task}", f"results/{domain}_3/{model}/{task}"]
                for file in files:
                    try:
                        with open(file) as f:
                            data = json.load(f)
                    except:
                        continue
                    for inst in data["instances"]:
                        if isinstance(inst["ground_truth_plan"], list):
                            inst["ground_truth_plan"] = '\n'.join(inst["ground_truth_plan"])
                        plan_len = len([i for i in inst["ground_truth_plan"].split('\n') if i])
                        if "llm_correct" in inst:
                            correct = int(inst["llm_correct"])*100
                        elif "correct" in inst:
                            correct = int(inst["correct"])*100
                        else:
                            continue
                        try:
                            llm_plan_len = len([i for i in inst["extracted_llm_plan"].split('\n') if i])
                        except:
                            continue
                        if correct and llm_plan_len == plan_len:
                            optimal = 100
                        else:
                            optimal = 0
                        df = pd.concat([df, pd.DataFrame([[plan_len, correct, optimal, MODELS[model], DOMAINS[domain]]], columns=["Plan Length", "Correct", "Optimal", "Model Name", "Domain (Zero shot)"])])
        mean_correct = df["Correct"].mean()
        mean_optimal = df["Optimal"].mean()
        new_df = pd.melt(df, id_vars=["Plan Length", "Model Name", "Domain (Zero shot)"], value_vars=["Correct"], var_name="Correct/Optimal")
        sns.set_theme(style="darkgrid", font=prop.get_name())
        fig = plt.figure(figsize=tuple([i*3.5 for i in FIGSIZE]))
        sns.set_context("poster", font_scale=FONTSCALE*2)
        # ax = sns.lineplot(data=df, x="Plan Length", y="Correct", hue="Domain (Zero shot)", linewidth=6, marker='o', palette=colors)
        # ax = sns.scatterplot(data=new_df, x="Plan Length", y="value", style="Correct/Optimal", hue="Domain (Zero shot)", s=350, palette=colors)
        # ax = sns.lineplot(data=new_df, x="Plan Length", y="value", style="Correct/Optimal", hue="Domain (Zero shot)", linewidth=6, marker='o', palette=colors)

        #Make a bar plot
        ax = sns.barplot(data=new_df, x="Plan Length", y="value", hue="Domain (Zero shot)", palette=colors[1:])


        # plt.axhline(y=mean_correct, color=colors[0], linestyle='--', label='Avg Correct')
        # plt.axhline(y=mean_optimal, color=colors[1], linestyle='--', label='Avg Optimal')
        plt.title(f"Plan Length vs Correct/Optimal Predictions for {MODELS[model]}", fontsize=FONTSIZE, pad=40)
        plt.xlabel('Plan Length', fontsize=FONTSIZE)
        plt.ylabel('% satisficing/optimal', fontsize=FONTSIZE, labelpad=20)
        # Legend outside the plot
        plt.legend(fontsize=LEGENDFONT, loc=(1.05, 0.5))
        plt.tight_layout()
        to_save = f"graphs/{model}/plan_length_vs_correct_optimal.png"
        os.makedirs(os.path.dirname(to_save), exist_ok=True)
        plt.savefig(to_save)
        plt.close()
                        
                

def get_stats_graph():
    domains = ["mystery_blocksworld", "blocksworld", "obfuscated_randomized_blocksworld"]
    tasks = ["task_1_plan_generation_zero_shot.json", "task_1_plan_generation.json"]
    models = ["o1-preview_chat", "llama-3.1-405b_aws", "o1-mini_chat", "gpt-4o-mini-2024-07-18_chat", "gpt-4o_chat", "gpt-4-turbo_chat", "gpt-4_chat", "gemini-1.5-pro", "gemini-1.5-flash", "claude-3.5-sonnet_aws", "claude-3-opus"]
    all_data = {}
    for domain in domains:
        for model in models:
            for task in tasks:
                key = tuple([domain, task])
                if key not in all_data:
                    all_data[key] = {}
                if domain == "mystery_blocksworld" and "gemini" in model:
                    continue
                plan_length_corrects, plan_length_optimal, plan_length_all = get_data(domain, model, task)
                if plan_length_corrects:
                    # get_stats(plan_length_corrects, plan_length_all,domain, model, task)
                    all_data[key][MODELS[model]] = plan_length_corrects
                    print(plan_length_corrects)
                    # For Fast Downward
                    all_data[key][MODELS["fast-downward"]] = dict([(i, [1
                     for _ in range(len(plan_length_corrects[i]))]) for i in plan_length_corrects])
                # if plan_length_optimal:
                    # get_stats(plan_length_optimal, plan_length_all, domain, model, task, optimal=True)
                    # all_data[key][MODELS[model]] = plan_length_optimal
                    # # For Fast Downward
                    # all_data[key][MODELS["fast-downward"]] = dict([(i, 100) for i in plan_length_optimal])

    for key in all_data:
        print(all_data[key])
        # Get long form of all_data
        # ["plan_length", "model_name", "model_group", "% satisficing"]
        df = pd.DataFrame(columns=["Plan Length", "Model Name", "Model Group", "% satisficing"])
        for model in all_data[key]:
            model_group = None
            for k in MG:
                if model in MG[k]:
                    model_group = k
                    break
            if model_group is None:
                raise Exception(f"Model {model} not found in any model group")
            for plan_length in all_data[key][model]:
                #if df is NaN then continue
                # if df.empty:
                #     continue
                #use concat
                percent_correct = round(sum(all_data[key][model][plan_length])*100 / len(all_data[key][model][plan_length]), 2)
                df = pd.concat([df, pd.DataFrame([[plan_length, model, model_group, percent_correct]], columns=["Plan Length", "Model Name", "Model Group", "% satisficing"])])
        # print(df)
            # save to csv
            print(f'{key[0]}{key[1].split('.')[0]}')
            df.to_csv(f"graphs/all_models/{key[0]}/{key[1].split('.')[0]}/all_data.csv", index=False)    
        order = ["fast-downward", "o1-preview_chat", "o1-mini_chat", "llama-3.1-405b_aws", "claude-3.5-sonnet_aws", "claude-3-opus", "gpt-4o_chat","gpt-4o-mini-2024-07-18_chat", "gpt-4-turbo_chat", "gpt-4_chat", "gemini-1.5-pro", "gemini-1.5-flash"]
        # Sort the columns in the above order
        
        # print(df)
        
        
        sns.set_theme(style="darkgrid", font=prop.get_name())
        fig = plt.figure(figsize=FIGSIZE)
        sns.set_context("poster", font_scale=FONTSCALE)
        palette= sns.color_palette(colors)
        # dashes = ['-', '--', '-.', ':']
        #Using style parameter separate the lines based on model_group
        ax = sns.lineplot(data=df, x="Plan Length", y="% satisficing", hue="Model Name", style="Model Group", linewidth=8, palette=palette, markers=True)
        
        #all_data to csv
        # ax = sns.lineplot(data=df, linewidth=6, marker='o', palette=palette)    
        ax.set(xlabel='Optimal plan Length', ylabel='% satisficing')
        plt.ylim(0, 105)
        plt.title(f"Optimal plan Length vs Correct Predictions for all Models on \n {DOMAINS[key[0]]} - {TASKS[key[1].split('.')[0]]}", fontsize=FONTSIZE, pad=40)
        to_save = f"graphs/all_models/{key[0]}/{key[1].split('.')[0]}/plan_length_vs_correct.png"
        os.makedirs(os.path.dirname(to_save), exist_ok=True)
        # plt.legend(fontsize=LEGENDFONT, loc=(1.05, 0.5))
        plt.tight_layout()
        plt.savefig(to_save)
        # for model in all_data[key]:
        #     plan_length_corrects = all_data[key][model]
        #     # Sort the dictionary
        #     plan_length_corrects = dict(sorted(plan_length_corrects.items()))
        #     graphs[model] = plt.plot(list(plan_length_corrects.keys()), list(plan_length_corrects.values()), label=MODELS[model], linewidth=6, marker='o')
        # plt.legend(fontsize=LEGENDFONT)
        # plt.xlabel('Plan Length', fontsize=FONTSIZE)
        # plt.ylabel('% satisficing', fontsize=FONTSIZE)
        # plt.ylim(0, 105)
        # plt.title("Plan Length vs Correct Predictions for all Models on \n" + f"{DOMAINS[key[0]]} - {TASKS[key[1].split('.')[0]]}", fontsize=FONTSIZE)
        # to_save = f"graphs/all_models/{key[0]}/{key[1].split('.')[0]}/plan_length_vs_correct.png"
        # os.makedirs(os.path.dirname(to_save), exist_ok=True)
        # plt.savefig(to_save)

def compare_llama_results():
    file1 = 'results/blocksworld/llama-3.1-405b_aws/task_1_plan_generation_zero_shot.json'
    file2 = 'results/blocksworld/llama-3.1-405b_aws/task_1_plan_generation_zero_shot_1.json'
    with open(file1) as f:
        data1 = json.load(f)
    with open(file2) as f:
        data2 = json.load(f)
    for inst1, inst2 in zip(data1["instances"], data2["instances"]):
        if "llm_correct" not in inst1 or "llm_correct" not in inst2:
            print(inst1["instance_id"], inst2["instance_id"])
            continue
        if inst1["llm_correct"] != inst2["llm_correct"]:
            print(inst1["instance_id"], inst2["instance_id"], inst1["llm_correct"], inst2["llm_correct"])

def reasoning_tokens_plan_length():
    file1 = 'results/mystery_blocksworld/o1-preview_chat/task_1_plan_generation_zero_shot_run2.json'
    with open(file1) as f:
        data1 = json.load(f)
    df = pd.DataFrame(columns=["Plan Length", "Reasoning Tokens"])
    plan_len_tokens = {}
    for inst in data1["instances"]:
        if inst["instance_id"]>300:
            continue
        if isinstance(inst["ground_truth_plan"], list):
                inst["ground_truth_plan"] = '\n'.join(inst["ground_truth_plan"])
        plan_len = len([i for i in inst["ground_truth_plan"].split('\n') if i])
        reasoning_tokens = inst["response_object"]["usage"]["completion_tokens_details"]["reasoning_tokens"]
        df = pd.concat([df, pd.DataFrame([[plan_len, reasoning_tokens]], columns=["Plan Length", "Reasoning Tokens"])])
    # Get error bars for reasoning tokens
    #Find 14 plan length data points in df
    print(df["Plan Length"])
    


    # Sort the dictionary
    plan_len_tokens = dict(sorted(plan_len_tokens.items()))
    print(plan_len_tokens)
    sns.set_theme(style="darkgrid", font=prop.get_name())
    fig = plt.figure(figsize=FIGSIZE)
    sns.set_context("poster", font_scale=FONTSCALE)
    ax = sns.lineplot(data=df, x="Plan Length", y="Reasoning Tokens", linewidth=6, marker='o', errorbar=('ci', 95))
    plt.xlabel('Plan Length', fontsize=FONTSIZE)
    plt.ylabel('Average Reasoning Tokens', fontsize=FONTSIZE, labelpad=20)
    plt.ylim(1000, 10000)
    plt.yticks(np.arange(2000, 12000, 2000))
    # plt.legend(["Reasoning Tokens"], fontsize=LEGENDFONT, loc='upper left')
    plt.title("Plan Length vs Reasoning Tokens for O1 Preview on \n" + f"{DOMAINS['mystery_blocksworld']} - {TASKS['task_1_plan_generation_zero_shot']}", fontsize=FONTSIZE, pad=40)

    to_save = f"graphs/mystery_blocksworld/o1-preview_chat/task_1_plan_generation_zero_shot/plan_length_vs_reasoning_tokens.png"
    os.makedirs(os.path.dirname(to_save), exist_ok=True)
    plt.savefig(to_save)
    # plt.legend(fontsize=LEGENDFONT)
    # plt.xlabel('Plan Length', fontsize=FONTSIZE)
    # #Add padding to the y-label
    # plt.ylabel('Average Reasoning Tokens', fontsize=FONTSIZE, labelpad=20)
    
    # plt.title("Plan Length vs Reasoning Tokens for O1 Preview on \n" + f"{DOMAINS['mystery_blocksworld']} - {TASKS['task_1_plan_generation_zero_shot']}", fontsize=FONTSIZE, pad=40)
    # to_save = f"graphs/mystery_blocksworld/o1-preview_chat/task_1_plan_generation_zero_shot/plan_length_vs_reasoning_tokens.png"
    # os.makedirs(os.path.dirname(to_save), exist_ok=True)
    # plt.savefig(to_save)
    # print(f"Saved {to_save}")
def obfuscated_randomized_blocksworld_reasoning_tokens_plan_length():
    file1 = 'results/obfuscated_randomized_blocksworld/o1-preview_chat/task_1_plan_generation_zero_shot_pddl.json'
    with open(file1) as f:
        data1 = json.load(f)
    df = pd.DataFrame(columns=["Plan Length", "Reasoning Tokens"])
    for inst in data1["instances"]:
        if isinstance(inst["ground_truth_plan"], list):
                inst["ground_truth_plan"] = '\n'.join(inst["ground_truth_plan"])
        plan_len = len([i for i in inst["ground_truth_plan"].split('\n') if i])
        try:
            reasoning_tokens = inst["response_object"]["usage"]["completion_tokens_details"]["reasoning_tokens"]
        except:
            continue
        df = pd.concat([df, pd.DataFrame([[plan_len, reasoning_tokens]], columns=["Plan Length", "Reasoning Tokens"])])
    sns.set_theme(style="darkgrid", font=prop.get_name())
    fig = plt.figure(figsize=FIGSIZE)
    sns.set_context("poster", font_scale=FONTSCALE)
    ax = sns.lineplot(data=df, x="Plan Length", y="Reasoning Tokens", linewidth=6, marker='o', errorbar=('ci', 95))
    plt.xlabel('Plan Length', fontsize=FONTSIZE)
    plt.ylabel('Average Reasoning Tokens', fontsize=FONTSIZE, labelpad=20)
    plt.legend(["Reasoning Tokens"], fontsize=LEGENDFONT, loc='upper left')
    plt.title("Plan Length vs Reasoning Tokens for O1 Preview on \n" + f"{DOMAINS['obfuscated_randomized_blocksworld']} - {TASKS['task_1_plan_generation_zero_shot']} PDDL", fontsize=FONTSIZE, pad=40)
    to_save = f"graphs/obfuscated_randomized_blocksworld/o1-preview_chat/task_1_plan_generation_zero_shot/plan_length_vs_reasoning_tokens.png"
    os.makedirs(os.path.dirname(to_save), exist_ok=True)
    plt.savefig(to_save)
    print(f"Saved {to_save}")
def new_blocksworld_graphs():
    file1 = '../plan-bench/results/new_blocksworld/o1-preview_chat/task_1_plan_generation_zero_shot_pddl.json'
    with open(file1) as f:
        data1 = json.load(f)
    info_file = '../plan-bench/instances/new_blocksworld/new_blocksworld_info.json'
    with open(info_file) as f:
        info = json.load(f)
    df1 = pd.DataFrame(columns=["Plan Length", "Reasoning Tokens"])
    df2 = pd.DataFrame(columns=["Plan Length", "% satisficing", "Optimal"])
    # df4 = pd.DataFrame(columns=["Plan Length", "Optimal"])
    df5 = pd.DataFrame(columns=["Nodes Expanded", "Reasoning Tokens"])
    for i in data1["instances"]:
        block, id = i["instance_id"].split('_')
        plan_len = int(block)
        reasoning_tokens = i["response_object"]["usage"]["completion_tokens_details"]["reasoning_tokens"]
        correct = int(i["llm_correct"])*100
        df1 = pd.concat([df1, pd.DataFrame([[plan_len, reasoning_tokens]], columns=["Plan Length", "Reasoning Tokens"])])
        
        llm_plan_len = len([i for i in i["extracted_llm_plan"].split('\n') if i])
        if correct and llm_plan_len == plan_len:
            optimal = 100
        else:
            optimal = 0
        df2 = pd.concat([df2, pd.DataFrame([[plan_len, correct, optimal]], columns=["Plan Length", "% satisficing", "Optimal"])])
        nodes_expanded = info[block][f"instance-{id}.pddl"]["states_info"]["expanded"]
        df5 = pd.concat([df5, pd.DataFrame([[nodes_expanded, reasoning_tokens]], columns=["Nodes Expanded", "Reasoning Tokens"])])
            
    #Get mean of entire % satisficing column and plot that as a dotted line
    mean_correct = df2["% satisficing"].mean()
    mean_optimal = df2["Optimal"].mean()
    print(f"Mean correct: {mean_correct}, Mean optimal: {mean_optimal}")
    # REASONING TOKENS VS PLAN LENGTH
    FIGSIZE = (40, 19)
    sns.set_theme(style="darkgrid", font=prop.get_name())
    fig = plt.figure(figsize=FIGSIZE)
    sns.set_context("poster", font_scale=FONTSCALE)
    ax = sns.lineplot(data=df1, x="Plan Length", y="Reasoning Tokens", linewidth=6, marker='o', errorbar=('ci', 95))
    plt.xlabel('Optimal plan length', fontsize=FONTSIZE)
    plt.ylabel('Average Reasoning Tokens', fontsize=FONTSIZE, labelpad=20)
    # plt.legend(["Reasoning Tokens"], fontsize=LEGENDFONT, loc='upper left')
    plt.xticks(np.arange(2, 41, 2))
    plt.ylim(1000, 10000)
    plt.yticks(np.arange(2000, 12000, 2000))
    plt.title("Optimal plan length vs Reasoning Tokens for O1 Preview on \n" + f"Blocksworld - {TASKS['task_1_plan_generation_zero_shot']}", fontsize=FONTSIZE, pad=40)
    to_save = f"graphs/new_blocksworld/o1-preview_chat/task_1_plan_generation_zero_shot/plan_length_vs_reasoning_tokens_bw.png"
    os.makedirs(os.path.dirname(to_save), exist_ok=True)
    plt.tight_layout()
    plt.savefig(to_save)
    plt.close()


    df2 = pd.melt(df2, id_vars=["Plan Length"], value_vars=["% satisficing"], var_name="variable")

    # PLAN LENGTH VS CORRECT
    sns.set_theme(style="darkgrid", font=prop.get_name())
    fig = plt.figure(figsize=FIGSIZE)
    sns.set_context("poster", font_scale=FONTSCALE)
    ax = sns.lineplot(data=df2, x="Plan Length", y="value", hue="variable", linewidth=6, marker='o', style="variable", palette=colors)
    plt.axhline(y=mean_correct, color=colors[0], linestyle='--', label='Avg Satisficing')
    plt.xlabel('Optimal plan length', fontsize=FONTSIZE)
    plt.ylabel('% satisficing', fontsize=FONTSIZE, labelpad=20)
    plt.legend(fontsize=LEGENDFONT, loc='upper right')
    plt.xticks(np.arange(2, 41, 2))
    plt.ylim(0, 105)
    plt.title("Optimal plan length vs Satisficing plans by O1 Preview on \n" + f"Blocksworld - {TASKS['task_1_plan_generation_zero_shot']}", fontsize=FONTSIZE, pad=40)
    to_save = f"graphs/new_blocksworld/o1-preview_chat/task_1_plan_generation_zero_shot/plan_length_vs_correct_bw_hard.png"
    os.makedirs(os.path.dirname(to_save), exist_ok=True)
    plt.tight_layout()
    plt.savefig(to_save)
    plt.close()



   
    print(df5)
    # REASONING TOKENS VS NODES EXPANDED
    sns.set_theme(style="darkgrid", font=prop.get_name())
    fig = plt.figure(figsize=FIGSIZE)
    sns.set_context("poster", font_scale=FONTSCALE)
    ax = sns.scatterplot(data=df5, x="Nodes Expanded", y="Reasoning Tokens", s=250, color=colors[0])
    plt.xlabel('Nodes Expanded', fontsize=FONTSIZE, labelpad=20)
    plt.ylabel('Average Reasoning Tokens', fontsize=FONTSIZE, labelpad=20)
    plt.xticks(np.arange(0, df5["Nodes Expanded"].max()+200000, 200000))
    plt.title("Nodes Expanded vs Reasoning Tokens for O1 Preview on \n" + f"Blocksworld - {TASKS['task_1_plan_generation_zero_shot']}", fontsize=FONTSIZE, pad=40)
    to_save = f"graphs/new_blocksworld/o1-preview_chat/task_1_plan_generation_zero_shot/nodes_expanded_vs_reasoning_tokens.png"
    os.makedirs(os.path.dirname(to_save), exist_ok=True)
    plt.tight_layout()
    plt.savefig(to_save)
    plt.close()


if __name__ == "__main__":
    get_stats_graph()
    # reasoning_tokens_plan_length()
    # new_blocksworld_graphs()
    # obfuscated_randomized_blocksworld_reasoning_tokens_plan_length()
    # get_new_graphs_optimal_correct()
    # get_logistics_graphs()

    

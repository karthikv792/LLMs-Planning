import os
import pickle
import openai
import threading

import numpy as np
from utils import *
import pandas as pd
from tqdm import tqdm

############################ Threading ##############################

class threadGPT3(threading.Thread):
    def __init__(self, threadID, gpt3_engine, n_examples, max_sents, domain_examples, domain_examples_ids, save_tr_queries=True):
        threading.Thread.__init__(self)
        self.threadId = threadID
        self.gpt3_engine = gpt3_engine
        self.n_examples = n_examples
        self.max_sents = max_sents
        self.domain_examples = domain_examples
        self.domain_examples_ids = domain_examples_ids
        self.results = {"precision": [0, 0], "recall": [0, 0], "f1": [0, 0]}
        self.save_tr_queries = save_tr_queries

    def send_query_gpt3(self, query):
        max_token_err_flag = False
        try:
            response = openai.Completion.create(
                engine=self.gpt3_engine,
                prompt=query,
                temperature=gpt3_temp,
                max_tokens=gpt3_max_tokens,
                top_p=1,
                frequency_penalty=0,
                presence_penalty=0,
                stop=["TEXT:", "ACTIONS:"]
            )
        except:
            max_token_err_flag = True
        text_response = response["choices"][0]["text"] if not max_token_err_flag else ""

        return text_response, max_token_err_flag

    def run(self):
        for domain_name in DOMAINS:
            queries_file = open("data/queries_{}.txt".format(domain_name), "a+")
            out_file = open("data/{}_{}_{}.out".format(self.gpt3_engine, domain_name, self.n_examples), "a+")
            out_file.write(str(self.domain_examples_ids[domain_name]) + ": {} EXAMPLES\n".format(self.n_examples))
            domain = DOMAINS[domain_name]

            # precision, recall, f1: [actions, objects]
            train_samples_str = self.domain_examples[domain_name]

            cnt = 0
            pbar = tqdm(domain)
            for i, test_sample in enumerate(pbar):
                if i in self.domain_examples_ids[domain_name][1:]: continue
                cnt += 1

                query = train_samples_str.strip()

                if self.save_tr_queries:
                    queries_file.write(query)
                    queries_file.write("\n\n")

                test_data = get_data(test_sample, max_sents=200)
                text_str_test, _ = get_query_strs(test_data)

                query += tags[0] + text_str_test + tags[1]
                query = query.strip()

                test_true_dict = get_test_dict(test_data)

                # ----- Send query to GPT3 and check correct output -----
                gpt3_response, err_flag = self.send_query_gpt3(query)
                if err_flag or not count_unfinished_responses and tags[0].strip() not in gpt3_response:
                    if err_flag:
                        pbar.set_postfix({"max_tokens_error": True})
                        # print("[-]: Max tokens error, continuing ...")
                    else:
                        print("[-]: Tag not found, continuing ...")
                        # print(gpt3_response)
                    cnt -= 1
                    continue
                # -------------------------------------------------------

                # Get just the actions
                gpt3_acts_text = gpt3_response.split(tags[0].strip())[0]
                pred_acts, pred_objs = get_acts_objs(gpt3_acts_text)

                prec_a, rec_a, f1_a, true_objs = compute_f1_acts(test_true_dict, pred_acts, test_data)
                prec_o, rec_o, f1_o = compute_f1_objs(true_objs, pred_objs)

                self.results["precision"][0] += prec_a
                self.results["recall"][0] += rec_a
                self.results["f1"][0] += f1_a

                self.results["precision"][1] += prec_o
                self.results["recall"][1] += rec_o
                self.results["f1"][1] += f1_o

                out_file.write("\n{}: {:.4f}-{:.4f}-{:.4f}_{:.4f}-{:.4f}-{:.4f}\n{}\n".format(
                    i, prec_a, rec_a, f1_a, prec_o, rec_o, f1_o, gpt3_acts_text.strip()
                ))

                pbar.set_postfix({"f1_act": self.results["f1"][0] / cnt, "f1_obj": self.results["f1"][1] / cnt})

            ########### Averages ###########
            if cnt > 0:
                print("ENGINE: {} | DOMAIN: {} | N_EXAMPLES: {}\n{}".format(self.gpt3_engine, domain_name, self.n_examples, "-" * 28))
                print("[+]: AVERAGES over {} runs".format(cnt))
                for i, name in enumerate(["acts", "objs"]):
                    for op in ["precision", "recall", "f1"]:
                        self.results[op][i] /= cnt
                        self.results[op][i] = round(self.results[op][i], 4)

                    print("[{}-exs-{}]: Precision: {:.4f} | Recall: {:.4f} | F1: {:.4f}".format(
                        name, self.n_examples, self.results["precision"][i], self.results["recall"][i], self.results["f1"][i]
                    ))

                prec, rec, f1 = self.results["precision"], self.results["recall"], self.results["f1"]
                x = [list(DOMAINS.keys()).index(domain_name), ALL_ENGINES.index(self.gpt3_engine),
                     cnt, self.n_examples, self.max_sents, prec[0], rec[0], f1[0], prec[1], rec[1], f1[1]]

                ########### Save data ###########
                print("[+]: Saving data \n\n")
                cols = ["domain", "engine", "runs", "examples", "max_sents", "prec_acts", "rec_acts", "f1_acts",
                        "prec_objs", "rec_objs", "f1_objs"]
                new_data = pd.DataFrame(np.array(x).reshape(1, -1), columns=cols)
                df = pd.read_csv(results_file).append(new_data) if os.path.exists(results_file) else new_data
                df.to_csv(results_file, index=False)
                #################################
                out_file.write("\n\n")

if __name__ == '__main__':
    wiki = pickle.load(open("data/datasets/wikihow_labeled_text_data.pkl", "rb"))
    win = pickle.load(open("data/datasets/win2k_labeled_text_data.pkl", "rb"))
    cook = pickle.load(open("data/datasets/cooking_labeled_text_data.pkl", "rb"))
    _ = pickle.load(open("data/datasets/cooking_dependency.pkl", "rb"))

    # ================= Params =================
    random.seed(42)
    ALL_ENGINES = ["davinci", "curie", "babbage", "ada"]
    DOMAINS = {"win": win, "cook": cook, "wiki": wiki}
    MAX_SENTS = 100

    gpt3_temp = 0  # Set to 0 for reproducibility
    gpt3_max_tokens = 100
    selected_engines = ["ada"]

    results_file = "results.csv"
    openai.api_key = "" #os.environ["OPENAI_API_KEY"]
    n_examples_list = [1, 2]  # 1: Random, 2: Excl+Opt 3: Es+Ex+Op 4: Random +prev
    count_unfinished_responses = True
    tags = ["\n\nTEXT: \n", "\nACTIONS: \n"]
    if not os.path.exists('data'):
        os.makedirs('data')
    # ==========================================

    for n_examples in n_examples_list:
        max_sents = 10 if n_examples == 4 else MAX_SENTS
        max_sents = 15 if n_examples == 3 else max_sents
        domain_examples, domain_examples_ids = get_repr_examples(DOMAINS, max_sents, n_examples, tags)
        for i, gpt3_engine in enumerate(selected_engines):
            thread_name = "{}_{}".format(gpt3_engine, n_examples)
            print("[+]: Executing {}".format(thread_name))
            thread = threadGPT3(i, gpt3_engine, n_examples, max_sents, domain_examples, domain_examples_ids)
            thread.start()
            # thread.join()  # Comment/Uncomment for parallel execution
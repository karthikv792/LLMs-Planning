import random
import re
from tqdm import tqdm


def get_acts_objs(acts_text):
    acts = [act.split("(")[0].strip() for act in acts_text.split("),") if act.split("(")[0].strip() != ""]
    objs_list = re.findall(r'\((.*?)\)', acts_text)

    objs = []
    for obj in objs_list:
        objs += obj.split(",")

    return acts, objs


def compute_f1_acts(true_dict, preds, test_data):
    true_objs = []
    type = "acts"
    essential = true_dict["essential"][type].copy()
    optional = true_dict["optional"][type].copy()
    exclusive = true_dict["exclusive"][type].copy()
    words = test_data["words"]

    total_tagged = len(preds) if len(preds) > 0 else 1
    total_truth = len(essential) + len(exclusive)
    right_es, right_op, right_ex = 0, 0, 0
    for item in preds:
        if item in essential:
            right_es += 1
            true_objs += true_dict["essential"]["act_obj"][essential.index(item)][1]  # Add the corresponding objects
            essential.remove(item)  # Removes from the left

        elif item in optional:
            right_op += 1
            total_truth += 1
            true_objs += true_dict["optional"]["act_obj"][optional.index(item)][1]  # Add the corresponding objects
            optional.remove(item)

    for item_list in exclusive:
        item_list_w = [words[w_id] for w_id in item_list]
        intersect = set(item_list_w).intersection(preds)
        # There can only appear 1 exclusive action amongst the predictions
        if len(intersect) == 1:
            right_ex += 1
            i = item_list_w.index(list(intersect)[0])  # Find the object indices of the matched action
            act_objs = [words[w_id] for w_id in test_data["acts2objs"][item_list[i]]]
            true_objs += act_objs

    total_right = right_es + right_op + right_ex
    precision = total_right / total_tagged
    recall = total_right / total_truth
    f1 = 2 * precision * recall / (precision + recall) if (precision + recall) > 0 else 0.0

    return precision, recall, f1, true_objs


def compute_f1_objs(true, preds):
    total_right = 0
    total_truth = len(true)
    total_tagged = len(preds)
    true_cpy = true.copy()

    if total_truth == 0 or total_tagged == 0:
        return 0.0, 0.0, 0.0

    for item in preds:
        if item in true_cpy:
            total_right += 1
            true_cpy.remove(item)

    precision = total_right / total_tagged
    recall = total_right / total_truth
    f1 = 2 * precision * recall / (precision + recall) if (precision + recall) > 0 else 0.0

    return precision, recall, f1


def get_data(sample, max_sents):
    # Action | (Objs) | Type
    data = {
        "data_id": [],
        "data_str": [],
        "related": {},
        "acts2objs": {},
        "sents": [],
        "words": sample["words"],
        "word2sent": sample["word2sent"]
    }

    data_id = data["data_id"]
    data_str = data["data_str"]
    related = data["related"]
    acts2objs = data["acts2objs"]
    ws = sample["words"]

    # Get maximum word id of the last action in the last sentence
    last_w, max_sents = 0, min(max_sents, len(sample["sents"]))
    for s in sample["sents"][:max_sents]:
        last_w += len(s)
        data["sents"].append(s)

    for act_dict in sample["acts"]:
        act_idx = act_dict['act_idx']
        if act_idx > last_w - 1: break
        obj_idxs = act_dict['obj_idxs'][0]

        related[act_idx] = act_dict["related_acts"]
        data_id.append([act_idx, obj_idxs, act_dict['act_type']])

        act_w, objs_w = ws[act_idx].lower(), [ws[obj].lower() for obj in obj_idxs]
        data_str.append([act_w, objs_w, act_dict['act_type']])

        acts2objs[act_idx] = obj_idxs

    return data


def get_query_strs(data):
    text_str = ""
    for s in data["sents"]:
        text_str += " ".join(s) + ".\n"

    acts_str = ""
    for (act, objs, id) in data["data_str"]:
        acts_str += "{}({}), ".format(act, ",".join(objs))

    acts_str = acts_str[:-2]  # remove last comma
    return text_str, acts_str


def get_test_dict(data):
    test_dict = {
        "essential": {
            "act_obj": [],
            "acts": [],
            "objs": []
        },
        "optional": {
            "act_obj": [],
            "acts": [],
            "objs": []
        },
        "exclusive": {
            "act_obj": {},
            "acts": [],
            "objs": []
        },
    }

    related = data["related"]
    acts = set()
    for act in related:
        rel = related[act]
        if len(rel) > 0:
            aux = [act] + rel
            aux.sort()
            acts.add(tuple(aux))
    test_dict["exclusive"]["acts"] = list(acts)

    acts2objs = data["acts2objs"]
    for acts in test_dict["exclusive"]["acts"]:
        test_dict["exclusive"]["objs"].append([acts2objs[act] for act in acts])

    types = list(test_dict.keys())
    for (act, objs, type), (act_id, objs_id, _) in zip(data["data_str"], data["data_id"]):
        es_op_ex = test_dict[types[type - 1]]
        if types[type - 1] != "exclusive":
            es_op_ex["act_obj"].append([act, objs])
            es_op_ex["acts"].append(act)
            es_op_ex["objs"].append(objs)

    return test_dict


def calc_avg_num_sents(domains):
    for domain_name in domains:
        domain = domains[domain_name]
        ct = 0
        for i in domain:
            ct += len(i["sents"])
            si = 0
            for s in i["sents"]:
                si += len(s)
        print("AVG {} {} sents {}".format(domain_name, ct / len(domain), si / len(i["sents"])))


def get_repr_examples(DOMAINS, max_sents, n_examples, tags, rand=False):
    # Essential, exclusive, optional
    examples_ids = {"win": [], "cook": [], "wiki": []}
    examples_str = {"win": "", "cook": "", "wiki": ""}

    examples_remaining = n_examples
    if not rand:
        for d_name in DOMAINS:
            domain = DOMAINS[d_name]

            if n_examples == 1:
                examples_ids[d_name].append(random.randint(0, len(domain) - 1))
                continue

            # Max , id
            maxs = {"es": [0, 0], "ex": [0, 0], "op": [0, 0]}

            for i, sample in enumerate(tqdm(domain)):
                es, ex, op = 0, 0, 0
                for act in sample["acts"]:
                    es += 1 if act["act_type"] == 1 else 0
                    ex += 1 if act["act_type"] == 3 else 0
                    op += 1 if act["act_type"] == 2 else 0

                total_acts = es + ex + op
                cur_maxs = {"es": es / total_acts, "ex": ex / total_acts, "op": op / total_acts}
                for act_type in maxs:
                    max = maxs[act_type]
                    cur_max = cur_maxs[act_type]
                    if max[0] < cur_max:
                        max[0] = cur_max
                        max[1] = i

            for act_type in maxs:
                max, i = maxs[act_type][0], maxs[act_type][1]
                # print("{}-{}: max: {:.2}, ith: {}".format(d_name, act_type, max, i))
                examples_ids[d_name].append(i)
        examples_remaining = n_examples - 3

    if examples_remaining >= 1:
        for d_name in DOMAINS:
            domain = DOMAINS[d_name]
            new_example = random.randint(0, len(domain) - 1)
            while new_example in examples_ids[d_name]:
                new_example = random.randint(0, len(domain) - 1)
            examples_ids[d_name].append(new_example)
        examples_remaining -= 1

    # Get the corresponding TEXT and ACTION strings from the selected examples
    for d_name in DOMAINS:
        domain = DOMAINS[d_name]
        if n_examples == 2:
            examples_ids[d_name].pop(0)  # Manual remove of the essential max
        for i, ith_sample in enumerate(examples_ids[d_name]):
            data_sample = get_data(domain[ith_sample], max_sents)
            text_str, acts_str = get_query_strs(data_sample)
            examples_str[d_name] += tags[0] + text_str + tags[1] + acts_str

    return examples_str, examples_ids
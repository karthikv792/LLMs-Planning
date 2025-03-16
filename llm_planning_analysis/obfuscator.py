import yaml
import random
import string
import argparse
import os
import pddl
from model_parser.constants import *
from model_parser.parser_new import parse_model
from model_parser.writer_new import ModelWriter
from pddl.formatter import domain_to_string, problem_to_string
from pddl.logic.base import And, Not
from pddl.logic.effects import AndEffect
from pddl.logic import Predicate, constants, variables
from pddl.formatter import domain_to_string
from pddl import parse_domain, parse_problem
from tqdm import tqdm
from rich import print
import copy
STATE_TRACKING_TEXT = "The plan correctness is defined in terms of states resulting from executing the\
actions in the plan. An action is executable in a state when all its preconditions\
  hold in that state. The state resulting from the action execution consists of everything\
  in the previous state with the addition and deletion of add and delete effects of\
  the action. Plan correctness is defined as follows: if the first action in the plan\
  is applicable in the initial state, i.e., its preconditions are all present there;\
  and the second action is applicable in the state resulting from applying the first\
  action to the initial state, this process continues until the state resulting from\
  the application of the last action in the last but one state gives rise to the final\
  state where all the goals are satisfied."

ALL_WORDS = []
def random_mapping(config):
    action_mapping = {}
    for action in config["actions"]:
        if len(ALL_WORDS) == 0:
            print("Not enough words provided")
            return

        mapped_action = random.choice(ALL_WORDS)
        ALL_WORDS.remove(mapped_action)
        action_mapping[action] = mapped_action

    predicate_mapping = {}
    for predicate in config["predicates"]:
        if len(ALL_WORDS) == 0:
            print("Not enough words provided")
            return
        

        mapped_predicate = random.choice(ALL_WORDS)
        ALL_WORDS.remove(mapped_predicate)
        predicate_mapping[predicate] = mapped_predicate

    return action_mapping, predicate_mapping

def create_new_config(config, 
                      output_config_file_name, 
                      action_mapping, 
                      predicate_mapping,
                      translated_pddl_path,
                      translated_domain,obf_type):
    config["action_obfuscation"] = action_mapping

    new_actions = {}
    for action in action_mapping:
        old_action_format = config["actions"][action]
        new_action_name = action_mapping[action]
        new_action = new_action_name
        for _ in range(old_action_format.count("{}")):
            new_action += " {}"
        new_actions[new_action_name] = new_action

    config["actions"] = new_actions

    config["predicate_obfuscation"] = predicate_mapping

    new_predicates = {}
    for predicate in predicate_mapping:
        old_predicate_format = config["predicates"][predicate]
        new_predicate_name = predicate_mapping[predicate]
        new_predicate = new_predicate_name
        if old_predicate_format.count("{}") > 0:
            for _ in range(old_predicate_format.count("{}")):
                new_predicate += " {}"
            new_predicates[new_predicate_name] = new_predicate
        else:
            new_predicate += " {}"
            new_predicates[new_predicate_name] = new_predicate

        

    config["predicates"] = new_predicates

    standard_intro = create_intro_from_translated_domain(translated_domain)

    config["domain_intro"] = standard_intro

    config["domain_intro_cost"] = create_intro_from_translated_domain(translated_domain, is_cost = True)

    config["domain_intro_state_tracking"] = standard_intro + STATE_TRACKING_TEXT
    # if 'domain_intro_state_tracking' in config:
    #     del config['domain_intro_state_tracking']

    config["domain_name"] = "obfuscated_" + obf_type + "_" + config["domain_name"]
    
    config["domain_file"] = translated_pddl_path + "/generated_domain.pddl"

    config["instance_dir"] = translated_pddl_path + "/generated_basic"

    config["generalized_instance_dir"] = translated_pddl_path + "/generated"
    
    config["encoded_objects"] = {'o': 'object_{}'}

    with open(output_config_file_name, "w") as f:
        yaml.dump(config, f)
    

def create_intro_from_translated_domain(translated_domain, is_cost = False):
    intro = "I am playing with a set of objects. Here are the actions I can do"
    intro += "\n\n"
    for action in translated_domain.actions:
        action_text = action.name.capitalize()
        for parameter in action.parameters:
            action_text += f" {parameter.name}"

        action_text += "."

        if is_cost:
            action_text += f" It takes one minute to {action.name}."
        
        intro += action_text
        intro += "\n"
    intro += "\n"

    intro += "I have the following restrictions on my actions:"

    intro += "\n"

    for action in translated_domain.actions:
        if action.precondition != None:

            intro += f"To perform {action.name} action, the following facts need to be true:"

            if isinstance(action.precondition, pddl.logic.base.And):
                for i, precon in enumerate(action.precondition.operands):
                    is_last = i == len(action.precondition.operands) - 1
                    if is_last:
                        precon_warning = f" and {precon.name}"
                    else:
                        precon_warning = f" {precon.name}"
                    for parameter in precon.terms:
                        precon_warning += f" {parameter.name}"
                    if not is_last:
                        precon_warning += ","
                    intro += precon_warning
            else:
                precon_warning = action.precondition.name
                for parameter in action.precondition.terms:
                    precon_warning += f" {parameter.name}"
                intro += precon_warning
        
        intro += "\n"

        add_effects = []
        del_effects = []
        for effect in action.effect.operands:
            if isinstance(effect, pddl.logic.base.Not):
                del_effect = effect.argument
                del_effect_string = del_effect.name
                for parameter in del_effect.terms:
                    del_effect_string += f" {parameter.name}"
                del_effects.append(del_effect_string)
            else:
                add_effect_string = effect.name
                for parameter in effect.terms:
                    add_effect_string += f" {parameter.name}"
                add_effects.append(add_effect_string)

        intro += f"Once {action.name} is performed the following facts will be true:"
        
        for i, effect in enumerate(add_effects):
            is_last = i == len(add_effects) - 1
            if is_last and i > 0:
                intro += " and"
            intro += f" {effect}"
            if not is_last:
                intro += ","

        intro += "\n"

        intro += f"Once {action.name} is performed the following facts will be false:"

        for i, effect in enumerate(del_effects):
            is_last = i == len(del_effects) - 1
            if is_last and i > 0:
                intro += " and"
            intro += f" {effect}"
            if not is_last:
                intro += ","

        intro += "\n"

    return intro

def translate_domain_pddl(original_domain_file, 
                          action_mapping, 
                          predicate_mapping,
                          translated_domain_dir,
                          translated_domain):
    domain = pddl.parse_domain(original_domain_file)
    for act in list(domain.actions):
        i = 1
        for params in act.parameters:
            params._name = f"ob_{i}"
            i += 1

    new_predicates = []

    for predicate in domain.predicates:
        new_predicates.append(pddl.logic.Predicate(predicate_mapping[predicate.name.lower()], *predicate.terms))

    new_actions = []

    for action in domain.actions:
        action_parameter_name_mapping = {}
        action_parameters = []
        for i, action_parameter in enumerate(action.parameters):
            new_name = f"object_{i}"
            action_parameter_name_mapping[action_parameter.name] = new_name
            action_parameters.append(pddl.logic.terms.Variable(new_name))
        updated_preconditions = translate_formula(action.precondition, predicate_mapping, action_parameter_name_mapping)
        updated_effects = translate_formula(action.effect, predicate_mapping, action_parameter_name_mapping)
        new_actions.append(pddl.core.Action(action_mapping[action.name.lower()], action_parameters, updated_preconditions, updated_effects))

    new_domain = pddl.core.Domain(name = "obfuscated-" + domain.name, requirements = domain.requirements, predicates = new_predicates, actions = new_actions)

    os.makedirs(translated_domain_dir, exist_ok=True)

    with open(f"{translated_domain_dir}/{translated_domain}", "w") as f:
        f.write(domain_to_string(new_domain))

    return new_domain

def translate_formula(formula, predicate_mapping, action_parameter_mapping):
    if isinstance(formula, pddl.logic.Predicate):
        new_terms = []
        for term in formula.terms:
            new_terms.append(pddl.logic.terms.Variable(action_parameter_mapping[term.name]))
        return pddl.logic.Predicate(predicate_mapping[formula.name.lower()], *new_terms)
    
    if isinstance(formula, pddl.logic.base.Not):
        return pddl.logic.base.Not(translate_formula(formula.argument, predicate_mapping, action_parameter_mapping))
    
    # Formula is a joining formula (like an and or an or) and has children

    new_operands = []
    for child_formula in formula.operands:
        new_operands.append(translate_formula(child_formula, predicate_mapping, action_parameter_mapping))

    if isinstance(formula, pddl.logic.base.And):
        return pddl.logic.base.And(*new_operands)
    elif isinstance(formula, pddl.logic.base.Or):
        return pddl.logic.base.Or(*new_operands)
    elif isinstance(formula, pddl.logic.effects.AndEffect):
        return pddl.logic.effects.AndEffect(*new_operands)


def replace_instance(domain,instance, action_mapping, predicate_mapping):
    model = parse_model(domain, instance)
    new_model = {}
    #DOMAIN
    new_model[DOMAIN] = {}
    for action in model[DOMAIN]:
        #Map action name
        new_action = action_mapping[action.lower()]
        new_model[DOMAIN][new_action] = {}
        for key, value in model[DOMAIN][action].items():
            if key in ['adds', 'dels', 'pos_prec']:
                new_value = []
                for item in value:
                    new_item = [predicate_mapping[item[0].lower()], item[1]]
                    new_value.append(new_item)
                new_model[DOMAIN][new_action][key] = new_value
            else:
                new_model[DOMAIN][new_action][key] = value
    #Predicates
    new_predicates = []
    for predicate in model[PREDICATES]:
        new_predicate = [predicate_mapping[predicate[0].lower()], predicate[1]]
        new_predicates.append(new_predicate)
    new_model[PREDICATES] = new_predicates

    #Objects
    obj_number = 0
    obj_mapping = {}
    new_objects = []
    for obj in model[CONSTANTS]:
        new_obj = f'object_{obj_number}'
        obj_mapping[obj[0]] = new_obj
        new_objects.append([new_obj, 'object'])
        obj_number+=1
    new_model[CONSTANTS] = new_objects

    #INSTANCE 
    new_model[INSTANCE] = {}
    #INIT - No functional predicates
    new_init = {FUNCTIONS:[], PREDICATES:[]}
    for predicate in model[INSTANCE][INIT][PREDICATES]:
        new_predicate = [predicate_mapping[predicate[0].lower()],
                        [obj_mapping[obj] for obj in predicate[1]]]
        new_init[PREDICATES].append(new_predicate)
    new_model[INSTANCE][INIT] = new_init
    #GOAL
    new_goal = []
    for predicate in model[INSTANCE][GOAL]:
        new_predicate = [predicate_mapping[predicate[0].lower()],
                        [obj_mapping[obj] for obj in predicate[1]]]
        new_goal.append(new_predicate)
    new_model[INSTANCE][GOAL] = new_goal

    for key in set(model.keys()).difference(new_model.keys()):
        new_model[key] = model[key]
    return new_model

def convert_instances(original_domain_file, original_instance, action_obfuscation, predicate_obfuscation, obf_type, domain_name):
    domain = parse_domain(original_domain_file)
    problem = parse_problem(original_instance)
    all_new_words = copy.deepcopy(ALL_WORDS)

    domain._name = domain_name
    for act in domain.actions:
        # print(type(act.precondition), act._effect, act.terms)
        act._name = action_obfuscation[act._name.lower()]
        i = 1
        for params in act.parameters:
            params._name = f"ob_{i}"
            i += 1
        if isinstance(act.precondition, Predicate):
            act.precondition._name = predicate_obfuscation[act.precondition._name.lower()]
        elif isinstance(act.precondition, And):
            for pred in act.precondition.operands:
                pred._name = predicate_obfuscation[pred._name.lower()]
        # print(act.effect, type(act.effect), dir(act.effect))
        if isinstance(act.effect, Predicate):
            act.effect._name = predicate_obfuscation[act.effect._name.lower()]
        elif isinstance(act.effect, AndEffect):
            for pred in act.effect.operands:
                if isinstance(pred, Predicate):
                    pred._name = predicate_obfuscation[pred._name.lower()]
                elif isinstance(pred, Not):
                    pred.argument._name = predicate_obfuscation[pred.argument._name.lower()]
                
    for pred in domain.predicates:
        pred._name = predicate_obfuscation[pred._name.lower()]
        # print(pred.terms, pred.arity)
    
    # print(domain_to_string(domain))

    
    # print(dir(problem))
    problem._domain_name = domain._name
    problem._name = random.choice(all_new_words)
    all_new_words.remove(problem._name)
    i=1
    
    if obf_type == "deceptive":
        for o in problem.objects:
            o._name = f"o{i}"
            i += 1
    else:
        for o in problem.objects:
            o._name = random.choice(all_new_words)
            all_new_words.remove(o._name)
    if isinstance(problem.goal, And):
        for pred in problem.goal.operands:
            pred._name = predicate_obfuscation[pred._name.lower()]
    else:
        problem.goal._name = predicate_obfuscation[problem.goal._name.lower()]
    if isinstance(problem.init, frozenset):
        for pred in problem.init:
            pred._name = predicate_obfuscation[pred._name.lower()]
    else:
        problem.init._name = predicate_obfuscation[problem.init._name.lower()]

    return domain_to_string(domain), problem_to_string(problem)
def _create_instances(config_original, config_obfuscated, action_mapping, predicate_mapping, n_instances, generalization_instances=False, obf_type=""):
    if generalization_instances:
        original_instance_dir = config_original["generalized_instance_dir"]
        obfuscated_instance_dir = config_obfuscated["generalized_instance_dir"]
    else:
        original_instance_dir = config_original["instance_dir"]
        obfuscated_instance_dir = config_obfuscated["instance_dir"]
    original_instance_template = f'./instances/{original_instance_dir}/{config_original["instances_template"]}'
    original_domain_file = f'./instances/{config_original["domain_file"]}'

    os.makedirs(f'./instances/{obfuscated_instance_dir}', exist_ok=True)
    obfuscated_instance_template = f'./instances/{obfuscated_instance_dir}/{config_obfuscated["instances_template"]}'
    obfuscated_domain_file = f'./instances/{config_obfuscated["domain_file"]}'
    domain_name = ALL_WORDS[0]
    ALL_WORDS.remove(domain_name)
    
    for i in tqdm(range(1, n_instances+1)):
        original_instance = original_instance_template.format(i)
        # obfuscated_model = replace_instance(original_domain_file, original_instance, action_mapping, predicate_mapping)

        #Write obfuscated instance
        # writer = ModelWriter(obfuscated_model, domain_name=config_obfuscated["domain_name"], problem_name=f"instance-{i}")
        # writer.write_files(obfuscated_domain_file, obfuscated_instance_template.format(i))
        obfuscated_domain, obfuscated_instance = convert_instances(original_domain_file, original_instance, action_mapping, predicate_mapping, obf_type, domain_name)
        with open(obfuscated_domain_file, "w") as f:
            f.write(obfuscated_domain)
        with open(obfuscated_instance_template.format(i), "w") as f:
            f.write(obfuscated_instance)

def create_obfuscated_instances(config_original, output_filename, generalization_instances=False, obf_type=""):
    assert os.path.exists(output_filename), f"Cannot find {output_filename}"
    with open(output_filename, "r") as f:
        config_obfuscated = yaml.safe_load(f)
    action_mapping = config_obfuscated["action_obfuscation"]
    predicate_mapping = config_obfuscated["predicate_obfuscation"]
    n_instances = config_original["n_instances"]
    _create_instances(config_original, config_obfuscated, action_mapping, predicate_mapping, n_instances, generalization_instances=False,  obf_type=obf_type)
    if generalization_instances:
        _create_instances(config_original, config_obfuscated, action_mapping, predicate_mapping, n_instances, generalization_instances=True,  obf_type=obf_type)

    

def random_as_mapping(config):
    total_actions = len(config["actions"])
    total_predicates = len(config["predicates"])
    all_new_words = []
    action_mapping = {}
    for action in config["actions"]:
        while True:
            number_of_as = random.randint(3, total_actions+total_predicates+5)
            new_action = "a"*number_of_as
            if new_action not in all_new_words:
                all_new_words.append(new_action)
                break
        action_mapping[action] = new_action

    predicate_mapping = {}
    for predicate in config["predicates"]:
        while True:
            number_of_as = random.randint(3, total_actions+total_predicates+5)
            new_predicate = "a"*number_of_as
            if new_predicate not in all_new_words:
                all_new_words.append(new_predicate)
                break
        predicate_mapping[predicate] = new_predicate

    return action_mapping, predicate_mapping


    



if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--config", type=str, required=True)
    parser.add_argument("--update_extra", action="store_true")
    parser.add_argument("--randomized_obfuscation", action="store_true")
    parser.add_argument("--all_as_mapping", action="store_true")
    parser.add_argument("--output_filename", type=str, default="")
    parser.add_argument("--words_filename", type=str, default="obfuscate/random_words_1.txt")
    parser.add_argument("--seed", type=int, default=0)
    parser.add_argument("--instances_pddl_subpath", type=str, default="obfuscated_[DOMAIN]")
    parser.add_argument("--generalization", action="store_true")
    parser.add_argument("--not_random_as", action="store_true")
    args = parser.parse_args()

    random.seed(args.seed)
    input_filename = f"configs/{args.config}.yaml"
    assert os.path.exists(input_filename), f"Cannot find {input_filename}"
    with open(input_filename, "r") as f:
        config = yaml.safe_load(f)
    print("Creating vocabulary...")
    if args.randomized_obfuscation:
        obf_type = "randomized"
        while len(ALL_WORDS) < 100:
            x = ''.join(random.choices(string.ascii_lowercase, k=1)+random.choices(string.ascii_lowercase + string.digits, k=15))
            if x not in ALL_WORDS:
                ALL_WORDS.append(x)
    elif args.all_as_mapping:
        obf_type = "all_as"
        if not args.not_random_as:
            while len(ALL_WORDS) < 100:
                y = random.randint(3, 105)
                x = "a"*y
                if x not in ALL_WORDS:
                    ALL_WORDS.append(x)
        else:
            ALL_WORDS = ["a"*i for i in range(3, 105)]
    else:
        obf_type = "deceptive"
        with open(args.words_filename, "r") as f:
            for line in f.readlines():
                if line != "":
                    ALL_WORDS.append(line.strip())
        if len(ALL_WORDS) < 100:
            raise ValueError("Not enough words provided")
    print("Created vocabulary")
    """
    You can create a vocabulary from the WordNet corpus
    from nltk.corpus import wordnet as wn
    words = []
    for synset in wn.all_synsets(wn.NOUN):
        words.append(synset.name().split(".")[0])
    """

    action_mapping, predicate_mapping = random_mapping(config)

    if "[DOMAIN]" in args.instances_pddl_subpath:
        if args.not_random_as:
            pddl_path = args.instances_pddl_subpath.replace("[DOMAIN]", obf_type+'_not_random_'+args.config)
        else:
            pddl_path = args.instances_pddl_subpath.replace("[DOMAIN]", obf_type+'_'+args.config)
    else:
        pddl_path = args.instances_pddl_subpath

    new_domain = translate_domain_pddl("instances/" + config["domain_file"], 
                          action_mapping, 
                          predicate_mapping, 
                          "instances/" + pddl_path, 
                          "generated_domain.pddl")
    # create_intro_from_domain("instances/" + config["domain_file"])

    if args.output_filename != "":
        output_filename = args.output_filename
    else:
        output_filename = f"configs/obfuscated_{obf_type}_{args.config}.yaml"

    create_new_config(config,
                      output_filename, 
                      action_mapping, 
                      predicate_mapping,
                      pddl_path,
                      new_domain, obf_type)
    with open(input_filename, "r") as f:
        config = yaml.safe_load(f)
    create_obfuscated_instances(config, output_filename, args.generalization, obf_type)

    if args.update_extra:
        input_extra_filename = f"configs/{args.config}_3.yaml"
        if args.output_filename != "":
            output_extra_filename = args.output_filename.replace(".", "_3.")
        else:
            output_extra_filename = f"configs/obfuscated_{obf_type}_{args.config}_3.yaml"

        with open(input_extra_filename, "r") as f:
            config = yaml.load(f)
    
        create_new_config(config, 
                          output_extra_filename, 
                          action_mapping, 
                          predicate_mapping,
                          pddl_path,
                          new_domain)

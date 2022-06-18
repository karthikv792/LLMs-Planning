"""
1. Parse grounded domain
2. generate a plan
3. take subset of actions
4.
"""
from model_parser.parser_new import parse_model
from model_parser.writer_new import ModelWriter
from model_parser.constants import *
import os
import random
import re
from copy import deepcopy

random.seed(10)


class Executor:
    def __init__(self, domain, problem):
        self.pr_domain, self.pr_problem = self.ground_domain(domain, problem)
        # self.pr_domain, self.pr_problem = domain, problem
        self.model = parse_model(self.pr_domain, self.pr_problem)
        self.plan, self.cost = self.get_plan(self.pr_domain, self.pr_problem)
        self.init_state = self.get_sets(self.model[INSTANCE][INIT][PREDICATES])
        self.goal_state = self.get_sets(self.model[INSTANCE][GOAL])
        self.final_state, self.all_preds, self.not_true_preds, self.prefix, self.replanning_init = [None] * 5
        self.final_state_dict = {}



    def replanning_domain_specific(self, harder=0, domain='blocksworld'):
        if domain == 'blocksworld':
            self.random_prefix_execution()
            while not any(['holding' in i for i in self.final_state]):
                self.random_prefix_execution()
            # make that block stack onto a random block
            self.replanning_init = self.final_state.copy()
            all_blocks = set()
            to_remove = set()
            for i in self.replanning_init:
                if 'handempty' in i:
                    continue
                if 'clear' in i:
                    all_blocks.add(i.split('_')[-1])
                if 'holding' in i:
                    current_block = i.split('_')[1]
                    to_remove.add(i)
            selected_block = random.choice(sorted(list(all_blocks)))
            to_remove.add('clear_' + selected_block)
            to_add = {'on_' + current_block + '_' + selected_block, 'handempty', 'clear_' + current_block}
            self.replanning_init = self.replanning_init.union(to_add)
            self.replanning_init = self.replanning_init.difference(to_remove)
            dict_to_send = {'to_add': to_add, 'to_remove': to_remove}
            print("REPLANNING INIT", self.replanning_init)
            print("REPLANNING DICT", dict_to_send)
            return dict_to_send


        else:
            raise Exception("Domain not supported")


    def replanning(self, harder=0):
        """
        1. Execute a random prefix of a plan and get the resulting state
        2. Regress the suffix of the plan from the goal and get the resulting (partial) state
        3. Two ways
            i. Make the problem harder by removing some of the preds from the prefix-state
            ii. Make the problem easier by adding some of the preds in the suffix-state into the prefix-state
        :return:
        """
        self.random_prefix_execution(replan=True)
        regress_state = self.regress(harder)
        if harder:
            this_much_harder = random.choice(range(1, len(regress_state) + 1))
            to_remove = set(random.choices(list(regress_state), k=this_much_harder))
            self.replanning_init = self.final_state.difference(to_remove)
            return to_remove
        else:
            this_much_easier = random.choice(range(1, len(regress_state) + 1))
            # print("-----------------", self.final_state, regress_state, this_much_easier, self.prefix)
            to_add = set(random.choices(list(regress_state), k=this_much_easier))
            self.replanning_init = self.final_state.union(to_add)
            return to_add

    def regress(self, harder):
        curr_state = self.goal_state
        suffix = self.plan[self.prefix:][::-1]
        if harder:
            for act in suffix:
                act = act.upper()
                # goal - effects u precond
                act_adds = self.get_sets(self.model[DOMAIN][act][ADDS])
                act_dels = self.get_sets(self.model[DOMAIN][act][DELS])
                act_pos_precs = self.get_sets(self.model[DOMAIN][act][POS_PREC])
                try:
                    act_neg_precs = self.get_sets(self.model[DOMAIN][act][NEG_PREC])
                except Exception as e:
                    act_neg_precs = set([])
                curr_state = curr_state.difference(act_adds.union(act_dels))
                curr_state = curr_state.union(act_neg_precs.union(act_pos_precs))
        else:
            rand_suff = random.choice(range(len(suffix)))
            print("SUFFIX", rand_suff, len(suffix))
            for act in suffix[:rand_suff]:
                act = act.upper()
                act_adds = self.get_sets(self.model[DOMAIN][act][ADDS])
                act_dels = self.get_sets(self.model[DOMAIN][act][DELS])
                act_pos_precs = self.get_sets(self.model[DOMAIN][act][POS_PREC])
                try:
                    act_neg_precs = self.get_sets(self.model[DOMAIN][act][NEG_PREC])
                except Exception as e:
                    act_neg_precs = set([])
                curr_state = curr_state.difference(act_adds.union(act_dels))
                curr_state = curr_state.union(act_neg_precs.union(act_pos_precs))
        regress_state = set()
        # print(curr_state)
        # # DOMAIN DEPENDENCY
        for i in curr_state:
            # if 'clear' not in i:
            #     regress_state.add(i)
            regress_state.add(i)
        # print(regress_state)
        return regress_state

    def random_prefix_execution(self, replan=False):
        print("PLAN", self.plan)
        self.prefix = random.choice(range(1, len(self.plan)))
        self.final_state = self.get_final_state(self.init_state, 0, self.prefix)
        self.all_preds = self.get_sets(self.model[PREDICATES])
        self.not_true_preds = self.all_preds.difference(self.final_state)
        for i in self.final_state:
            self.final_state_dict[i] = "Yes"
        for i in self.not_true_preds:
            self.final_state_dict[i] = "No"

    def complete_plan_execution(self):
        self.prefix = len(self.plan)
        self.final_state = self.get_final_state(self.init_state, 0, self.prefix)
        self.all_preds = self.get_sets(self.model[PREDICATES])
        self.not_true_preds = self.all_preds.difference(self.final_state)
        for i in self.final_state:
            self.final_state_dict[i] = "Yes"
        for i in self.not_true_preds:
            self.final_state_dict[i] = "No"

    def get_final_state(self, curr_state, start, end):
        initial_state = curr_state

        for act in self.plan[start:end]:
            act = act.upper()
            act_adds = self.get_sets(self.model[DOMAIN][act][ADDS])
            act_dels = self.get_sets(self.model[DOMAIN][act][DELS])
            initial_state = initial_state.union(act_adds)
            initial_state = initial_state.difference(act_dels)

        return initial_state

    def get_plan(self, domain, problem):
        """
        Executes FD and returns a random prefix of the plan
        :param domain:
        :param problem:
        :return:
        """
        fd_path = os.getenv("FAST_DOWNWARD")
        # Remove > /dev/null to see the output of fast-downward
        CMD_FD = f"{fd_path}/fast-downward.py {domain} {problem} --search \"astar(lmcut())\" >/dev/null 2>&1"
        os.system(CMD_FD)
        # USE SAS PLAN to get actions
        plan = []
        cost = 0
        try:
            with open('sas_plan') as f:
                for line in f:
                    if ';' not in line:
                        plan.append((line.strip()[1:-1].strip()))
                    else:
                        cost_group = re.search(r'\d+', line)
                        if cost_group:
                            cost = int(cost_group.group())
            if cost == 0:
                cost = len(plan)
        except FileNotFoundError:
            return 'No plan found', 0
        # os.remove('sas_plan')
        return plan, cost

    def get_sets(self, list_of_preds):
        return set([i[0] for i in list_of_preds])

    def ground_domain(self, domain, problem):
        pr2_path = os.getenv("PR2")
        # Remove > /dev/null to see the output of PR2
        CMD_PR2 = f"{pr2_path}/pr2plan -d {domain}  -i {problem} -o blank_obs.dat >/dev/null 2>&1"
        os.system(CMD_PR2)
        pr_domain = 'pr-domain.pddl'
        pr_problem = 'pr-problem.pddl'
        self.remove_explain(pr_domain, pr_problem)
        return pr_domain, pr_problem

    def remove_explain(self, domain, problem):
        try:
            cmd = 'cat {0} | grep -v "EXPLAIN" > pr-problem.pddl.tmp && mv pr-problem.pddl.tmp {0}'.format(problem)
            os.system(cmd)
            cmd = 'cat {0} | grep -v "EXPLAIN" > pr-domain.pddl.tmp && mv pr-domain.pddl.tmp {0}'.format(domain)
            os.system(cmd)
        except FileNotFoundError:
            raise Exception('[ERROR] Removing "EXPLAIN" from pr-domain and pr-problem files.')

    def get_new_instance(self, change_goal, change_init):
        if change_goal:
            goal = []
            for i in self.final_state:
                goal.append([i, []])
            new_model = deepcopy(self.model)
            new_model[INSTANCE][GOAL] = goal
            writer = ModelWriter(new_model)
            writer.write_files('pr-new-domain.pddl', 'pr-new-problem.pddl')
        if change_init:
            # print(self.init_state, self.replanning_init)
            init = []
            for i in self.replanning_init:
                init.append([i, []])
            new_model = deepcopy(self.model)
            # print(new_model[INSTANCE][INIT][PREDICATES], init)
            new_model[INSTANCE][INIT][PREDICATES] = init
            writer = ModelWriter(new_model)
            writer.write_files('pr-new-domain.pddl', 'pr-new-problem.pddl')


if __name__ == "__main__":
    domain = 'instances/ipc_domain.pddl'
    problem = 'instances/instance-2.pddl'
    exec = executor(domain, problem)
    print("\n")
    exec.replanning(0)
    print("PLAN: ", exec.plan)
    print("INITIAL STATE: ", exec.init_state)
    print("After Plan Execution (A.P.E.) STATE: ", exec.final_state)
    print("GOAL STATE: ", exec.goal_state)
    print("NOT TRUE PREDS: ", exec.not_true_preds)
    exec.get_new_instance(False, True)
    print(exec.get_plan('pr-new-domain.pddl', 'pr-new-problem.pddl'))

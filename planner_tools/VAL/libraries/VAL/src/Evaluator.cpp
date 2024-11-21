// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#include "Evaluator.h"
#include "InstPropLinker.h"
#include "State.h"
#include "TypedAnalyser.h"
#include "Validator.h"
#include "instantiation.h"
#include "ptree.h"

using namespace VAL;

namespace Inst {

  Evaluator::Evaluator(Validator *v, const VAL::State *s,
                       Inst::instantiatedOp *op, bool im)
      : vld(v),
        value(true),
        env(toEnv(op)),
        f(op->getEnv()),
        state(s),
        equality(VAL::current_analysis->pred_tab.symbol_probe("=")),
        ignoreMetrics(im),
        context(true){};

  void Evaluator::visit_preference(preference *p){};

  void Evaluator::visit_simple_goal(simple_goal *s) {
    //	cout << "Evaluating " << *s << " in state " << *state << "\n";

    if (EPS(s->getProp()->head)->getParent() == this->equality) {
      //	cout << "Got equality\n";
      value = ((*f)[s->getProp()->args->front()] ==
               (*f)[s->getProp()->args->back()]);

      if (s->getPolarity() == E_NEG) {
        value = !value;
      };
      return;
    };

    // Evaluate the goal in the state
    const SimpleProposition *sp = vld->pf.buildLiteral(s->getProp(), env);
    value = state->evaluate(sp);
    if (s->getPolarity() == E_NEG) {
      value = !value;
    };
    //	cout << "Value determined as: " << value << "\n";
  };

  void Evaluator::visit_qfied_goal(qfied_goal *q) {
    cout << "Evaluator (line 70): Not handling quantified goals yet!\n";

    for (var_symbol_list::const_iterator i = q->getVars()->begin();
         i != q->getVars()->end(); ++i) {
      cout << "Got: "
           << static_cast< const IDsymbol< var_symbol > * >(*i)->getId()
           << "\n";
    };
  };

  void Evaluator::visit_conj_goal(conj_goal *c) {
    value = true;
    for (goal_list::const_iterator i = c->getGoals()->begin();
         i != c->getGoals()->end(); ++i) {
      (*i)->visit(this);
      if (!value) return;
    };
  };

  void Evaluator::visit_disj_goal(disj_goal *d) {
    value = false;
    for (goal_list::const_iterator i = d->getGoals()->begin();
         i != d->getGoals()->end(); ++i) {
      (*i)->visit(this);
      if (value) return;
    };
  };

  void Evaluator::visit_timed_goal(timed_goal *t) {
    t->getGoal()->visit(this);
  };

  void Evaluator::visit_imply_goal(imply_goal *ig) {
    context = !context;
    ig->getAntecedent()->visit(this);
    context = !context;
    if (value) {
      ig->getConsequent()->visit(this);
    } else
      value = true;
  };

  void Evaluator::visit_neg_goal(neg_goal *ng) {
    context = !context;
    ng->getGoal()->visit(this);
    context = !context;
    value = !value;
  };

  void Evaluator::visit_event(event *op) {
    value = true;
    op->precondition->visit(this);
  };

  void Evaluator::visit_process(process *op) {
    value = true;
    op->precondition->visit(this);
  };

  void Evaluator::visit_comparison(comparison *comp) {
    // Note: Could avoid dual visits by doing parallel evaluation with and
    // without
    // metrics
    if (ignoreMetrics) {
      value = context;
    };
    // Problem: Really this works for continuous change - should do proper
    // evaluations for discretely changing values.
    double eval = state->evaluate(comp->getLHS(), env) -
                  state->evaluate(comp->getRHS(), env);
    double tooSmall = 0.00001;

    switch (comp->getOp()) {
      case E_GREATER:
        value = (eval > -tooSmall);
        if (value && eval <= 0) {
          cout << "Sloppy evaluation disagrees: " << eval << " > 0\n";
        };
        return;

      case E_GREATEQ:
        value = (eval >= -tooSmall);
        if (value && eval < 0) {
          cout << "Sloppy evaluation disagrees: " << eval << " >= 0\n";
        };
        return;

      case E_LESS:
        value = (eval < tooSmall);
        if (value && eval >= 0) {
          cout << "Sloppy evaluation disagrees: " << eval << " < 0\n";
        };
        return;

      case E_LESSEQ:
        value = (eval <= tooSmall);
        if (value && eval > 0) {
          cout << "Sloppy evaluation disagrees: " << eval << " <= 0\n";
        };
        return;

      case E_EQUALS:
        value = (eval < tooSmall && eval > -tooSmall);
        if (value && eval != 0) {
          cout << "Sloppy evaluation disagrees: " << eval << " = 0\n";
        };
        return;

      default:
        return;
    };
  };

  void Evaluator::visit_action(action *op) {
    value = true;
    op->precondition->visit(this);
  };

  void Evaluator::visit_durative_action(durative_action *da) {
    value = true;
    da->precondition->visit(this);
  };

};  // namespace Inst

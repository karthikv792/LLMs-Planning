// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#ifndef __EVALUATOR
#define __EVALUATOR
#include "Environment.h"
#include "VisitController.h"
#include "ptree.h"
#include <map>
#include <set>
#include <vector>

namespace VAL {
  class State;
  class FastEnvironment;
  class Validator;

};  // namespace VAL

namespace Inst {

  class instantiatedOp;

  class Evaluator : public VAL::VisitController {
   protected:
    VAL::Validator *vld;

    bool value;
    VAL::Environment env;
    VAL::FastEnvironment *f;

    const VAL::State *state;

    VAL::pred_symbol *equality;

    bool ignoreMetrics;
    bool context;

   public:
    static void setInitialState();

    Evaluator(VAL::Validator *v, const VAL::State *s, Inst::instantiatedOp *op,
              bool im = false);

    virtual void visit_simple_goal(VAL::simple_goal *);
    virtual void visit_qfied_goal(VAL::qfied_goal *);
    virtual void visit_conj_goal(VAL::conj_goal *);
    virtual void visit_disj_goal(VAL::disj_goal *);
    virtual void visit_timed_goal(VAL::timed_goal *);
    virtual void visit_imply_goal(VAL::imply_goal *);
    virtual void visit_neg_goal(VAL::neg_goal *);
    virtual void visit_comparison(VAL::comparison *);
    virtual void visit_preference(VAL::preference *);
    virtual void visit_event(VAL::event *e);
    virtual void visit_process(VAL::process *p);
    virtual void visit_action(VAL::action *o);
    virtual void visit_durative_action(VAL::durative_action *da);
    bool operator()() { return value; };
  };

};  // namespace Inst

#endif

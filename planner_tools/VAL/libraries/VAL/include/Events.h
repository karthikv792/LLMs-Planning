// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#include "Action.h"
#include "Plan.h"
#include "State.h"
#include "ptree.h"

#include <vector>
using std::vector;

#ifndef __EVENTS
#define __EVENTS

namespace VAL {

  class Events {
   private:
    vector< event * > ungroundEvents;
    vector< process * > ungroundProcesses;

    set< string > triggeredEvents;  // triggered events at a given time point
    set< const StartAction * > triggeredProcesses;  // triggered processes at a
                                                    // given time point
    set< string > untriggeredProcesses;  // untriggered processes at a given
                                         // time
                                         // point

    // active processes
    map< const StartAction *, pair< const Proposition *, const_symbol_list * > >
        activeProcesses;

    vector< const Action * > eventsForMutexCheck;  // these are actions that are
                                                   // triggered the same
    // time as an action, stored here for extra mutex
    // checks

    // to be deleted after plan is complete
    vector< const Action * > oldTriggeredEvents;

    double lastHappeningTime;
    bool ctsEventTriggered;

   public:
    Events(const operator_list *ops);
    ~Events();

    string getName(operator_ *op, const_symbol_list *csl) const;
    vector< const Action * > getTriggeredEvents() const {
      return oldTriggeredEvents;
    };
    bool triggerInitialEvents(Validator *v, double firstHappeningTime);
    bool triggerEventsOnInterval(Validator *v, bool init);
    bool triggerDiscreteEvents(Validator *v, bool init);
    bool triggerDiscreteProcesses(Validator *v);
    bool triggerContinuousEvents(Validator *v, bool init);
    bool hasEvents() const {
      return (!(ungroundEvents.empty()) || !(ungroundProcesses.empty()));
    };
    bool isTriggered(event *eve, const_symbol_list *csl) const;
    bool isProcessActive(process *pro, const_symbol_list *csl) const;
    bool isTriggered(const Action *act) const;
    bool isProcessTriggered(const StartAction *sa) const;
    bool isProcessUntriggered(process *pro, const_symbol_list *csl) const;
    void updateHappeningTime(double t);
    void updateEventsForMutexCheck(Validator *v);
    const vector< const Action * > &getEventsForMutexCheck() const {
      return eventsForMutexCheck;
    };
  };

  bool checkPreconditionsAreNotSatisfied(
      const State *s, const vector< const Action * > &events);
  const vector< const_symbol_list * > getParametersDiscreteInitialFinal(
      goal *g, operator_ *op, Validator *v);
  const vector< const_symbol_list * > getParametersDiscreteFinal(
      goal *g, operator_ *op, const State &s);
  const vector< const_symbol_list * > getParametersCtsFinal(goal *g,
                                                            operator_ *op,
                                                            Validator *v);
  const vector< const_symbol_list * > getParametersDiscrete(goal *g,
                                                            operator_ *op,
                                                            const State &s,
                                                            bool neg = false);
  const vector< const_symbol_list * > getParametersCts(goal *g, operator_ *op,
                                                       Validator *v,
                                                       bool neg = false,
                                                       bool atAPoint = false);
  const vector< const_symbol_list * > getParameters(goal *g, operator_ *op,
                                                    Validator *v, bool discrete,
                                                    bool neg = false,
                                                    bool atAPoint = false);
  const vector< const_symbol_list * > getParametersList(
      goal *g, operator_ *op, Validator *v,
      const vector< const_symbol_list * > &lop, bool neg, bool discrete,
      bool atAPoint = false);
  const vector< const_symbol_list * > defineUndefinedParameters(
      const_symbol_list *csl, var_symbol_list *variables, Validator *vld,
      const set< var_symbol * > &svs);
  const vector< const_symbol_list * > defineUndefinedParameters(
      const vector< const_symbol_list * > &vcsl, operator_ *op, Validator *vld,
      const set< var_symbol * > &svs);
  const vector< const_symbol_list * > defineUndefinedParametersPropVar(
      const_symbol_list *csl, operator_ *op, Validator *vld, goal *g, bool dp,
      bool neg, const set< var_symbol * > &svs, bool compError = false);
  const vector< const_symbol_list * > defineUndefinedParametersPropVar(
      const vector< const_symbol_list * > &vcsl, operator_ *op, Validator *vld,
      goal *g, bool dp, bool neg, const set< var_symbol * > &svs,
      bool compError = false);
  const vector< const_symbol_list * > checkParametersProp(
      const vector< const_symbol_list * > &vcsl, operator_ *op, Validator *vld,
      goal *g, bool neg, bool dp, bool compError = false);
  string getParameter(const SimpleProposition *sp, var_symbol *vs,
                      const simple_goal *sg);
  string getParameter(const FuncExp *fe, var_symbol *vs, const func_term *pne);
  bool undefinedParameterExists(const_symbol_list *csl);
  const vector< const_symbol_list * > removeRepeatedParameters(
      const vector< const_symbol_list * > &vcsl);
  void getPNEs(const expression *e, set< const func_term * > &pnes);
  const set< var_symbol * > getVariables(const expression *e);
  const set< var_symbol * > getVariables(const simple_goal *sg);
  const set< var_symbol * > getVariables(const qfied_goal *qg);
  const set< var_symbol * > getVariables(const operator_ *op);
  void addToListOfParameters(vector< const_symbol_list * > &vcsl,
                             const vector< const_symbol_list * > &lop,
                             const_symbol_list *csl);
  const expression *copyExpression(const expression *e);
  const goal *copyGoal(const goal *g);
  const vector< const_symbol_list * > getCopyCSL(
      const vector< const_symbol_list * > &lop);
  void deleteParameters(vector< const_symbol_list * > &vcsl);
  const_symbol_list *newBlankConstSymbolList(var_symbol_list *parameters,
                                             Validator *v);

};  // namespace VAL

#endif

// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#ifndef __PINGUPLANGEN
#define __PINGUPLANGEN

#include "TimSupport.h"
#include "TypedAnalyser.h"
#include "VisitController.h"
#include "ptree.h"
#include <iostream>
#include <map>
#include <set>
#include <string>

using std::cout;
using std::map;
using std::pair;
using std::set;
using std::string;
using namespace TIM;

namespace VAL {

  struct PinguPosition {
    float first;
    float second;
    int di;

    PinguPosition(float f1, float f2, int d) : first(f1), second(f2), di(d){};
  };

  struct PinguAction {
    string name;
    int x;
    int y;

    PinguAction(string n, int x1, int y1) : name(n), x(x1), y(y1){};
  };

  class PinguPlanGen : public VisitController {
   private:
    map< string, pair< float, float > > position;
    set< string > midairLocs;
    map< string, string > bounceLocs;
    map< string, int > recordDirection;
    set< string > blocked;
    string lastAt;
    int count;

    map< int, int > lastActionTime;
    map< string, int > whoIsAt;
    map< string, int > lastActAt;
    map< string, string > lastActWas;
    map< string, string > path;
    int pingu;
    int lastMoved;
    bool mustDelay;

    void doAction(string, plan_step *);
    void doBomb(plan_step *, string, string);
    void doBridge(string, string);
    void doMine(string, string);
    void doBash(string, string);
    int findDirection(string);
    PinguPosition getPosition(string);
    void doThis(string, string);

   public:
    PinguPlanGen(char *name);

    virtual void visit_effect_lists(effect_lists *el) {
      for (pc_list< simple_effect * >::iterator i = el->add_effects.begin();
           i != el->add_effects.end(); ++i) {
        (*i)->visit(this);
      };

      for (pc_list< assignment * >::iterator i = el->assign_effects.begin();
           i != el->assign_effects.end(); ++i) {
        (*i)->visit(this);
      };

      for (pc_list< timed_effect * >::iterator i = el->timed_effects.begin();
           i != el->timed_effects.end(); ++i) {
        (*i)->visit(this);
      };
    };

    virtual void visit_simple_effect(simple_effect *se);

    virtual void visit_plan_step(plan_step *p);
  };

};  // namespace VAL

#endif

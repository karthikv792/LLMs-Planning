// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#include <iostream>

#include "valLib.h"

#include "ptree.h"

#include "FlexLexer.h"
#include <fstream>

#include <iomanip>
#include <sstream>
#include <vector>
#include <cmath>

#include "VisitController.h"

using std::ifstream;
using std::map;
using std::ofstream;
using std::stringstream;
using std::vector;
using std::nearbyint;

namespace VAL {

  extern parse_category *top_thing;
  extern analysis an_analysis;
  extern yyFlexLexer *yfl;
}  // namespace VAL

extern int yyparse();
extern int yydebug;

using namespace VAL;
using namespace std;

class PlanBit {
 private:
  const plan_step *p;
  bool start;
  int act;

 public:
  PlanBit(const plan_step *p1, bool c, int a) : p(p1), start(c), act(a) {}

  void write(ostream &o) const {
    o << fixed << setprecision(3);
    if (start) {
      o << "start " << p->op_sym->getName() << " ";
      for (const_symbol_list::const_iterator i = p->params->begin();
           i != p->params->end(); ++i) {
        o << (*i)->getName() << " ";
      }
      o << "@ " << p->start_time;
    } else {
      o << "end " << act << " @ " << p->start_time + p->duration;
    }
    // if(p->duration_given) o << "[" << p->duration << "]";
  }
};

ostream &operator<<(ostream &o, const PlanBit &p) {
  p.write(o);
  return o;
}

class PlanProcessor : public VisitController {
 private:
  map< double, vector< PlanBit > > planbits;
  int x;

  double round(double d)
  {
    // Be better to round according to epsilon, but here we round to nearest 10000th (which assumes differences at 1/10th standard epsilon
    // are actually intentional - this matters for high accuracy in continuous effects.
    return nearbyint(d*10000)/10000.0;
  }
  
 public:
  PlanProcessor() : x(0){};

  virtual void visit_plan_step(plan_step *p) {
    ++x;
    planbits[round(p->start_time)].push_back(PlanBit(p, true, x));

    if (p->duration_given) {
      planbits[round(p->start_time + p->duration)].push_back(PlanBit(p, false, x));
    }
  };

  void show() {
    for (map< double, vector< PlanBit > >::const_iterator i = planbits.begin();
         i != planbits.end(); ++i) {
      for (vector< PlanBit >::const_iterator j = i->second.begin();
           j != i->second.end(); ++j) {
        cout << *j << "\n";
      }
      cout << "x\n";
    }
  }
};

int main(int argc, char *argv[]) {
  current_analysis = &an_analysis;
  ifstream plan(argv[1]);

  yfl = new yyFlexLexer(&plan, &cout);

  yydebug = 0;
  yyparse();
  delete yfl;
  if (top_thing) {
    PlanProcessor p;
    top_thing->visit(&p);
    p.show();
  }

  return 0;
}

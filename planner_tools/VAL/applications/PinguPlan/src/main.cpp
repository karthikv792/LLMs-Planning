// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#include "FlexLexer.h"
#include "FuncAnalysis.h"
#include "PinguPlanGenerator.h"
#include "TIM.h"
#include "ptree.h"
#include <fstream>
#include <iostream>

using namespace std;
using namespace TIM;
using namespace VAL;
// using namespace Inst;

namespace VAL {

  extern yyFlexLexer *yfl;
}
extern int yyparse();

plan *getPlan(char *name) {
  plan *the_plan = 0;

  ifstream planFile(name);
  if (!planFile) {
    cout << "Bad plan file!\n";
    return the_plan;
  };

  yfl = new yyFlexLexer(&planFile, &cout);
  yyparse();
  delete yfl;

  the_plan = dynamic_cast< plan * >(top_thing);

  return the_plan;
};

int main(int argc, char *argv[]) {
  FAverbose = false;
  performTIMAnalysis(&argv[1]);

  plan *thePlan = getPlan(argv[3]);
  PinguPlanGen ppg(argv[4]);

  current_analysis->the_problem->initial_state->visit(&ppg);
  cout << "(pingus-plan\n(actions\n";
  thePlan->visit(&ppg);
  cout << "))\n";
};

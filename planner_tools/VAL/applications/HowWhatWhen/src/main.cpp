// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#include "AbstractGraph.h"
#include "FuncAnalysis.h"
#include "HowAnalyser.h"
#include "TIM.h"
#include "ptree.h"
#include <iostream>

using namespace TIM;
using namespace VAL;
// using namespace Inst;

int main(int argc, char *argv[]) {
  FAverbose = false;
  performTIMAnalysis(&argv[1]);

  HowAnalyser ha;

  current_analysis->the_domain->predicates->visit(&ha);
  current_analysis->the_domain->ops->visit(&ha);
  current_analysis->the_problem->initial_state->visit(&ha);
  ha.completeGraph();
};

// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#include "DebugWriteController.h"
#include "SimpleEval.h"
#include "TIM.h"
#include "instantiation.h"
#include "ptree.h"
#include "typecheck.h"
#include <FlexLexer.h>
#include <cstdio>
#include <fstream>
#include <iostream>

using std::cerr;
using std::ifstream;

using namespace TIM;
using namespace Inst;
using namespace VAL;

int main(int argc, char *argv[]) {
  performTIMAnalysis(&argv[1]);

  SimpleEvaluator::setInitialState();
  for (operator_list::const_iterator os =
           current_analysis->the_domain->ops->begin();
       os != current_analysis->the_domain->ops->end(); ++os) {
    cout << (*os)->name->getName() << "\n";
    instantiatedOp::instantiate(*os, current_analysis->the_problem, *theTC);
    cout << instantiatedOp::howMany() << " so far\n";
  };
  instantiatedOp::createAllLiterals(current_analysis->the_problem, theTC);
  instantiatedOp::filterOps(theTC);
  cout << instantiatedOp::howMany() << "\n";
  instantiatedOp::writeAll(cout);

  cout << "\nList of all literals:\n";

  instantiatedOp::writeAllLiterals(cout);
}

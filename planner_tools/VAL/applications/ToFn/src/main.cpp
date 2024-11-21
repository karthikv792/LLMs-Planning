// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#include "FastEnvironment.h"
#include "SASActions.h"
#include "SimpleEval.h"
#include "ToFunction.h"
#include "instantiation.h"

using namespace SAS;

int main(int argc, char *argv[]) {
  performTIMAnalysis(&argv[1]);
  use_sasoutput = true;
  FunctionStructure fs;
  fs.normalise();
  fs.initialise();

  fs.processActions();
  fs.buildLayers();

  fs.setUpInitialState();
  int level = 0;
  while (fs.growOneLevel()) {
    ++level;
    cout << "Built level: " << level << "\n";
  };
};

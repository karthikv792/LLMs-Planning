// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#ifndef __TIM
#define __TIM

#include "TimSupport.h"

namespace VAL {
  extern TypeChecker *theTC;
};

namespace TIM {
  extern TIMAnalyser *TA;

  void performTIMAnalysis(char *argv[]);

  void TIMstage1(char *argv[]);
  void TIMstage2();

  template < class PROC >
  void performTIMAnalysis(char *argv[], PROC proc) {
    TIMstage1(argv);

    proc();

    TIMstage2();
  }

};  // namespace TIM

#endif

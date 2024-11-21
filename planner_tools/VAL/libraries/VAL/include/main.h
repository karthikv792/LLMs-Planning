// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#ifndef __MAIN
#define __MAIN

#include <iostream>
using std::ostream;

namespace VAL {

  extern bool Verbose;
  extern bool ContinueAnyway;
  extern bool ErrorReport;
  extern bool InvariantWarnings;
  extern bool LaTeX;
  extern bool LaTeXRecord;
  extern ostream *report;
  extern int NoGraphPoints;
  extern bool makespanDefault;
  extern bool stepLengthDefault;

};  // namespace VAL

#endif

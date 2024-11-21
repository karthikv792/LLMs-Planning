// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#include "Utils.h"
#include "Action.h"
#include "FuncExp.h"
#include "Polynomial.h"
#include <sstream>
#include <string>

namespace VAL {

  void replaceSubStrings(string &s, string s1, string s2) {
    size_t pos = s.find(s1);
    size_t subPos = pos;
    size_t size = s.size();

    for (size_t count = 1; count < size; ++count) {
      if (subPos != string::npos)
        s.replace(pos, s1.size(), s2);
      else
        break;

      subPos = (s.substr(pos + s2.size(), string::npos)).find(s1);
      pos = pos + s2.size() + subPos;
    };
  };

  // change any action names etc that LaTeX will not like
  void latexString(string &s) { replaceSubStrings(s, "_", "\\_"); };

};  // namespace VAL

// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#ifndef __HYPSP
#define __HYPSP

#include <ostream>

namespace VAL {

  class GoalHypothesisSpace {
   public:
    virtual ~GoalHypothesisSpace(){};
    virtual void write(std::ostream& o) const {
      std::cout << "Goal Hypothesis Space:\n<< >>\n";
    };
  };

  std::ostream& operator<<(std::ostream& o, const GoalHypothesisSpace& g) {
    g.write(o);
    return o;
  };

}  // namespace VAL

#endif

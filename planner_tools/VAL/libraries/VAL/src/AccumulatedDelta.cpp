// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#include "AccumulatedDelta.h"
#include "State.h"

namespace VAL {
  AccumulatedDelta::AccumulatedDelta() {
    // ctor
  }

  AccumulatedDelta::~AccumulatedDelta() {
    // dtor
  }

  void AccumulatedDelta::notifyChanged(const State *s, const Happening *h) {
    set< const FuncExp * > chPNEs = s->getChangedPNEs();
    changedPNEs.insert(chPNEs.begin(), chPNEs.end());

    set< const SimpleProposition * > chLits = s->getChangedLiterals();
    changedLiterals.insert(chLits.begin(), chLits.end());
  }

  void AccumulatedDelta::reset() {
    changedPNEs.clear();
    changedLiterals.clear();
  }
}  // namespace VAL

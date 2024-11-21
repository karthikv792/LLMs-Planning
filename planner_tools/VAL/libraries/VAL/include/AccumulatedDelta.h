// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#ifndef ACCUMULATEDDELTA_H
#define ACCUMULATEDDELTA_H

#include "FuncExp.h"
#include "Plan.h"
#include "Proposition.h"
#include "StateObserver.h"
#include <set>

using std::set;
namespace VAL {
  class AccumulatedDelta : public StateObserver {
   public:
    AccumulatedDelta();
    virtual ~AccumulatedDelta();

    void notifyChanged(const State *, const Happening *);
    void reset();
    const set< const FuncExp * > &getChangedPNEs() const {
      return changedPNEs;
    };
    const set< const SimpleProposition * > &getChangedLiterals() const {
      return changedLiterals;
    };

   private:
    set< const FuncExp * > changedPNEs;
    set< const SimpleProposition * > changedLiterals;
  };
}  // namespace VAL

#endif  // ACCUMULATEDDELTA_H

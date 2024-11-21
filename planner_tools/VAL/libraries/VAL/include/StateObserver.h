// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#ifndef __STATEOBS
#define __STATEOBS

namespace VAL {

  class Happening;
  class EffectsRecord;

  class StateObserver {
   public:
    virtual ~StateObserver(){};
    virtual void notifyChanged(const State *s, const Happening *h){};
  };

}  // namespace VAL
#endif

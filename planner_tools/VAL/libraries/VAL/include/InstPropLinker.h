// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#ifndef __LITERALPROPLINK
#define __LITERALPROPLINK

namespace VAL {
  class SimpleProposition;
  class Environment;
};  // namespace VAL

namespace Inst {

  class Literal;
  class instantiatedOp;

  Literal *toLiteral(const VAL::SimpleProposition *);
  VAL::Environment toEnv(instantiatedOp *op);

};  // namespace Inst

#endif

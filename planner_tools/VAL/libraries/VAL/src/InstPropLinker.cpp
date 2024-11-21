// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#include "InstPropLinker.h"

#include "Proposition.h"
#include "instantiation.h"

#include "Environment.h"
#include "FastEnvironment.h"
#include "ptree.h"

using namespace VAL;

namespace Inst {

  Literal *toLiteral(const VAL::SimpleProposition *sp) {
    int id = -1;
    for (parameter_symbol_list::const_iterator i = sp->getProp()->args->begin();
         i != sp->getProp()->args->end(); ++i) {
      if (const var_symbol *vs = dynamic_cast< const var_symbol * >(*i)) {
        id = std::max(
            id, static_cast< const IDsymbol< var_symbol > * >(vs)->getId());
      };
    };
    FastEnvironment *fe = new FastEnvironment(id + 1);
    for (parameter_symbol_list::const_iterator i = sp->getProp()->args->begin();
         i != sp->getProp()->args->end(); ++i) {
      if (const var_symbol *vs = dynamic_cast< const var_symbol * >(*i)) {
        (*fe)[vs] =
            const_cast< VAL::const_symbol * >(sp->getEnv()->find(vs)->second);
      };
    };

    CreatedLiteral *cl = new CreatedLiteral(sp->getProp(), fe);

    Literal *res = instantiatedOp::getLiteral(cl);
    if (res != cl) {
      delete cl;
    };
    return res;
  };

  Environment toEnv(instantiatedOp *op) {
    Environment e;
    for (var_symbol_list::const_iterator i = op->forOp()->parameters->begin();
         i != op->forOp()->parameters->end(); ++i) {
      if (const var_symbol *vs = dynamic_cast< const var_symbol * >(*i)) {
        e[vs] = (*(op->getEnv()))[vs];
      };
    };
    return e;
  };

};  // namespace Inst

// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#ifndef __OWNERSHIP
#define __OWNERSHIP

#include "ptree.h"
#include <map>

namespace VAL {

  class Validator;
  class Action;
  class FuncExp;
  struct Environment;
  class SimpleProposition;
  class expression;

  using std::map;
  using std::pair;

  enum ownership { E_PRE, E_PPRE, E_NPRE, E_ADD, E_DEL, E_ASSIGNMENT, E_DELPRE, E_ADDNPRE };

  class Ownership {
   private:
    map< const SimpleProposition *, pair< const Action *, ownership > >
        propOwner;

    Validator *vld;
    map< const FuncExp *, pair< const Action *, ownership > > FEOwner;

   public:
    Ownership(Validator *v) : vld(v){};

    bool markOwnedPrecondition(const Action *a, const SimpleProposition *p,
                               ownership o);
    bool markOwnedPreconditionFEs(const Action *a, const expression *e,
                                  const Environment &bs);
    bool ownsForAdd(const Action *a, const SimpleProposition *p);
    bool ownsForDel(const Action *a, const SimpleProposition *p);
    bool markOwnedEffectFE(const Action *a, const FuncExp *fe, assign_op aop,
                           const expression *e, const Environment &bs);
  };

};  // namespace VAL

#endif

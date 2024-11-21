// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#ifndef __CGA
#define __CGA

#include <map>
#include <set>

#include "ToFunction.h"

namespace SAS {

  class CausalGraph {
   public:
    typedef pair< const VAL::pddl_type *, int > Var;
    typedef std::set< Var > Vars;
    typedef std::map< Var, Vars > Graph;

   private:
    FunctionStructure fs;

    Graph dependencies;
    Graph dependents;

   public:
    CausalGraph();
    const Vars &getDependencies(Var p) { return dependencies[p]; };
    const Vars &getDependents(Var p) { return dependents[p]; };
    void add(Var, Var);
    void write(std::ostream &o) const;
  };

  inline std::ostream &operator<<(std::ostream &o, const CausalGraph &cg) {
    cg.write(o);
    return o;
  };

}  // namespace SAS

#endif

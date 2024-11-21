// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#include "CausalGraph.h"
#include "SASActions.h"
#include "TimSupport.h"
#include "ToFunction.h"
#include "VisitController.h"
#include "ptree.h"
#include <set>

using namespace std;
using namespace VAL;

namespace SAS {

  CausalGraph::CausalGraph() {
    fs.normalise();
    fs.initialise();
    fs.processActions();

    for (FunctionStructure::iterator i = fs.begin(); i != fs.end(); ++i) {
      cout << *(i->second);
      set< const ValueRep * > pres;
      set< const ValueRep * > posts;
      for (SASActionTemplate::iterator j = i->second->precondsBegin();
           j != i->second->precondsEnd(); ++j) {
        pres.insert(j->second.begin(), j->second.end());
      };
      for (SASActionTemplate::iterator j = i->second->postcondsBegin();
           j != i->second->postcondsEnd(); ++j) {
        posts.insert(j->second.begin(), j->second.end());
      };
      for (set< const ValueRep * >::iterator j = posts.begin();
           j != posts.end(); ++j) {
        for (set< const ValueRep * >::iterator k = pres.begin();
             k != pres.end(); ++k) {
          add(Var((*j)->getType(), (*j)->getSegment()),
              Var((*k)->getType(), (*k)->getSegment()));
        };
      };
    };
  };

  void CausalGraph::add(Var e, Var p) {
    if (e != p) {
      dependencies[e].insert(p);
      dependents[p].insert(e);
    };
  };

  void CausalGraph::write(ostream &o) const {
    for (Graph::const_iterator i = dependencies.begin();
         i != dependencies.end(); ++i) {
      o << i->first.first->getName() << "_" << i->first.second << ":\n";
      for (Vars::iterator j = i->second.begin(); j != i->second.end(); ++j) {
        o << "\t" << j->first->getName() << "_" << j->second << "\n";
      };
    };
  };

}  // namespace SAS

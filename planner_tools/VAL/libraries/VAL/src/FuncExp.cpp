// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#include "FuncExp.h"
#include "LibLink.h"
#include "RobustAnalyse.h"
#include "State.h"
#include "main.h"
#include "random.h"

#ifdef SUPPORTEXTERNALS
using ExternalSupport::LibLoader;
#endif

//#define map std::map
namespace VAL {

  bool FuncExp::isExternal() const { return cd != 0; }

  double FuncExp::getExternalValue(const State *s) const {
    vector< string > as;
    for (unsigned int i = 1; i <= fe->getArgs()->size(); ++i) {
      as.push_back(getParameter(i));
    }
#ifdef SUPPORTEXTERNALS
    return LibLoader::makeCall(cd, fe->getFunction()->getName(), as);
#else
    cerr << "External solver linkage not supported in this version!\n";
    throw SyntaxTooComplex();
#endif
  }

  double FuncExp::evaluate(const State *s) const {
    double ans = s->evaluateFE(this);

    if (JudderPNEs && hasChangedCtsly) {
      ans +=
          RobustPNEJudder *
          (1 - 2 * getRandomNumberUniform());  // if not robustness testing this
                                               // change will not be activated
    };
    return ans;
  };

  string FuncExp::getParameter(int paraNo) const {
    int parameterNo = 1;
    for (parameter_symbol_list::const_iterator i = fe->getArgs()->begin();
         i != fe->getArgs()->end(); ++i) {
      if (paraNo == parameterNo) {
        if (dynamic_cast< const var_symbol * >(*i)) {
          return bindings.find(dynamic_cast< const var_symbol * >(*i))
              ->second->getName();
        } else {
          return (*i)->getName();
        };
      };
      ++parameterNo;
    };

    return "";
  };

  bool FuncExp::checkConstantsMatch(const parameter_symbol_list *psl) const {
    const_symbol *aConst;

    parameter_symbol_list::const_iterator ps = psl->begin();  // from event
    for (parameter_symbol_list::const_iterator i =
             fe->getArgs()->begin();  // from func
         i != fe->getArgs()->end(); ++i) {
      if (dynamic_cast< const const_symbol * >(*ps)) {
        if (const var_symbol *aVariable =
                dynamic_cast< const var_symbol * >(*i)) {
          aConst =
              const_cast< const_symbol * >(bindings.find(aVariable)->second);
        } else {
          aConst = const_cast< const_symbol * >(
              dynamic_cast< const const_symbol * >(*i));
        };

        if (*ps != aConst) return false;
      };

      ++ps;
    };

    return true;
  };

  ostream &operator<<(ostream &o, const FuncExp &fe) {
    fe.write(o);
    return o;
  };

  void FuncExp::setChangedCtsly() { hasChangedCtsly = true; };

  Environment FuncExpFactory::nullEnv;

  FuncExpFactory::~FuncExpFactory() {
    for (map< string, const FuncExp * >::const_iterator i = funcexps.begin();
         i != funcexps.end(); ++i)
      delete const_cast< FuncExp * >(i->second);
  };

};  // namespace VAL

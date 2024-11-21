// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#include <LibLink.h>
#ifdef SUPPORTEXTERNALS
extern "C" {

#include <dlfcn.h>
}
#include "Exceptions.h"
#include "FuncAnalysis.h"
#include "State.h"
#include "ptree.h"
#include <sstream>
#include <string>

using std::string;
using std::stringstream;
using VAL::class_def;

namespace ExternalSupport {

  void *LibLoader::getLib(class_def *cd) {
    static LibLoader lld;

    void *h = cd->getLib();
    if (!h) {
      stringstream s;
#ifndef LINUX
      s << cd->name->getName() << ".dll";
#else
      s << cd->name->getName() << ".so";
#endif
      h = dlopen(s.str().c_str(), RTLD_LAZY);
      lld.handles.push_back(h);  // This is to close them later - and it is here
                                 // to avoid contaminating the ptree code.
      cd->setLib(h);
    }
    return h;
  }

  LibLoader::~LibLoader() {
    // cout << "I got called\n";
    for (vector< void * >::iterator i = handles.begin(); i != handles.end();
         ++i) {
      // cout << "Closing the library\n";
      dlclose(*i);
    }
  }

  // This will need another argument if we want to pass the state to it....
  typedef double (*Callback)(const char **, int);

  struct LocIt {
    const char **myIt;
    LocIt(const char **cp) : myIt(cp){};
    LocIt &operator++() {
      // cout << "Moving on to " << *myIt << "\n";
      ++myIt;
      return *this;
    };
    VAL::const_symbol *operator*() {
      // cout << "Extracting value: " << *myIt << "\n";
      // cout << "Plan to return " << *Inst::getConst(*myIt) << "\n";
      return VAL::current_analysis->const_tab.symbol_get(*myIt);
    };
    bool operator==(const LocIt &i) { return myIt == i.myIt; };
  };

  class ExpEvaluator : public VAL::VisitController {
   protected:
    bool undefined;
    double nvalue;  // Used for numeric values.

   public:
    ExpEvaluator(){};

    virtual void visit_plus_expression(VAL::plus_expression *s);
    virtual void visit_minus_expression(VAL::minus_expression *s);
    virtual void visit_mul_expression(VAL::mul_expression *s);
    virtual void visit_div_expression(VAL::div_expression *s);
    virtual void visit_uminus_expression(VAL::uminus_expression *s);
    virtual void visit_int_expression(VAL::int_expression *s);
    virtual void visit_float_expression(VAL::float_expression *s);

    double getValue() { return nvalue; }
  };

  void ExpEvaluator::visit_plus_expression(VAL::plus_expression *s) {
    s->getLHS()->visit(this);
    double x = nvalue;
    s->getRHS()->visit(this);
    nvalue += x;
  };

  void ExpEvaluator::visit_minus_expression(VAL::minus_expression *s) {
    s->getLHS()->visit(this);
    double x = nvalue;
    s->getRHS()->visit(this);
    nvalue -= x;
  };

  void ExpEvaluator::visit_mul_expression(VAL::mul_expression *s) {
    s->getLHS()->visit(this);
    double x = nvalue;
    s->getRHS()->visit(this);
    nvalue *= x;
  };

  void ExpEvaluator::visit_div_expression(VAL::div_expression *s) {
    s->getRHS()->visit(this);
    double x = nvalue;
    s->getLHS()->visit(this);
    if (x != 0) {
      nvalue /= x;
    };
  };

  void ExpEvaluator::visit_uminus_expression(VAL::uminus_expression *s) {
    s->getExpr()->visit(this);
    nvalue = -nvalue;
  };

  void ExpEvaluator::visit_int_expression(VAL::int_expression *s) {
    nvalue = s->double_value();
  };

  void ExpEvaluator::visit_float_expression(VAL::float_expression *s) {
    nvalue = s->double_value();
  };

  bool match(const VAL::func_term *ft, const VAL::func_symbol *fs,
             const vector< const VAL::const_symbol * > &as) {
    if (ft->getFunction() != fs) return false;

    list< VAL::parameter_symbol * >::const_iterator j = ft->getArgs()->begin();
    for (vector< const VAL::const_symbol * >::const_iterator i = as.begin();
         i != as.end(); ++i, ++j) {
      if (*i != *j) return false;
    }
    return true;
  }

  // This mechanism doesn't allow hierarchical dependencies in the external
  // calls
  // yet...
  double getValueInState(const char *args[], int n) {
    const VAL::func_symbol *fs =
        VAL::current_analysis->func_tab.symbol_get(args[0]);

    vector< const VAL::const_symbol * > as;

    for (int i = 1; i < n; ++i) {
      as.push_back(VAL::current_analysis->const_tab.symbol_get(args[i]));
    }

    for (VAL::pc_list< VAL::assignment * >::const_iterator i =
             VAL::current_analysis->the_problem->initial_state->assign_effects
                 .begin();
         i != VAL::current_analysis->the_problem->initial_state->assign_effects
                  .end();
         ++i) {
      if (match((*i)->getFTerm(), fs, as)) {
        ExpEvaluator eexp;
        (*i)->getExpr()->visit(&eexp);

        return eexp.getValue();
      }
    }
    throw VAL::BadAccessError();
  }

  // Pass in state for a context...
  double LibLoader::makeCall(class_def *cd, const string &nm,
                             vector< string > args) {
    // cout << "About to call library...\n";
    const char *argvs[args.size()];
    for (unsigned int i = 0; i < args.size(); ++i) {
      argvs[i] = args[i].c_str();
    }
    void *h = getLib(cd);
    if (!h) {
      cout << "Failed to find and load library\n";
    }

    // I think we pass the state to this for extended functionality.
    double (*fn)(Callback, const char **);
    // cout << "Want to call " << nm.c_str() << "\n";
    *(void **)(&fn) = dlsym(h, nm.c_str());
    return (*fn)(&getValueInState, argvs);
  }

}  // namespace ExternalSupport
#endif

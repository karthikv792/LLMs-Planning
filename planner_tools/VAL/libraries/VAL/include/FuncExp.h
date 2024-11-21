// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#include "Exceptions.h"
#include <algorithm>
#include <iostream>
#include <stdlib.h>

#include "Environment.h"
#include "Utils.h"
#include "ptree.h"

#ifndef __FUNCEXP
#define __FUNCEXP

//#define map std::map
using std::cerr;
using std::map;

namespace VAL {

  class State;
  extern bool LaTeX;

  struct Matcher {
    const class_symbol *csym;

    Matcher(const class_symbol *c) : csym(c){};

    bool operator()(const class_def *cd) { return cd->name == csym; };
  };

  class FuncExp {
   private:
    const Environment &bindings;
    const func_term *fe;

    bool hasChangedCtsly;  // for testing robustness w.r.t. numerical accuracy
    class_def *cd;

   public:
    FuncExp(const func_term *f, const Environment &bs)
        : bindings(bs), fe(f), hasChangedCtsly(false), cd(0) {
      const class_func_term *cft = dynamic_cast< const class_func_term * >(fe);
      if (cft) {
        list< class_def * >::iterator cdi =
            std::find_if(current_analysis->the_domain->classes->begin(),
                         current_analysis->the_domain->classes->end(),
                         Matcher(cft->getClass()));
        if (cdi != current_analysis->the_domain->classes->end()) {
          cd = *cdi;
        } else {
          cerr << "Use of undefined class " << cd->name->getName() << "\n";
          throw UndefinedClassError();
        }
      }
    };

    int getArity() const { return fe->getArgs()->size(); }
    double evaluate(const State *s) const;
    string getName() const { return fe->getFunction()->getName(); };
    string getParameter(int paraNo) const;
    bool checkConstantsMatch(const parameter_symbol_list *psl) const;
    void setChangedCtsly();
    const Environment *getEnv() { return &bindings; };

    bool isExternal() const;
    double getExternalValue(const State *s) const;

    void write(ostream &o) const {
      string st = "(" + fe->getFunction()->getName();
      for (parameter_symbol_list::const_iterator i = fe->getArgs()->begin();
           i != fe->getArgs()->end(); ++i) {
        if (dynamic_cast< const var_symbol * >(*i)) {
          st += " " + bindings.find(dynamic_cast< const var_symbol * >(*i))
                          ->second->getName();
        } else {
          st += " " + (*i)->getName();
        };
      };
      st += ")";

      if (LaTeX) latexString(st);
      o << st;
    };
  };

  ostream &operator<<(ostream &o, const FuncExp &fe);

  class FuncExpFactory {
   private:
    static Environment nullEnv;
    map< string, const FuncExp * > funcexps;

   public:
    const FuncExp *buildFuncExp(const func_term *f) {
      string s(f->getFunction()->getName());

      for (parameter_symbol_list::const_iterator i = f->getArgs()->begin();
           i != f->getArgs()->end(); ++i) {
        s += " ";
        s += (*i)->getName();
      };
      map< string, const FuncExp * >::const_iterator i1 = funcexps.find(s);
      if (i1 != funcexps.end()) return i1->second;
      const FuncExp *p = funcexps[s] = new FuncExp(f, nullEnv);
      return p;
    };
    const FuncExp *buildFuncExp(const func_term *f, const Environment &bs) {
      string s(f->getFunction()->getName());
      for (parameter_symbol_list::const_iterator i = f->getArgs()->begin();
           i != f->getArgs()->end(); ++i) {
        s += " ";
        if (dynamic_cast< const var_symbol * >(*i)) {
          map< const var_symbol *, const const_symbol * >::const_iterator j =
              bs.find(dynamic_cast< const var_symbol * >(*i));
          if (j != bs.end()) {
            s += j->second->getName();
          } else {
            cout << "Error: could not find parameter "
                 << dynamic_cast< const var_symbol * >(*i)->getName() << "\n";
            exit(-1);
          };
        } else {
          s += (*i)->getName();
        };
      };
      map< string, const FuncExp * >::const_iterator i1 = funcexps.find(s);
      if (i1 != funcexps.end()) return i1->second;
      const FuncExp *p = funcexps[s] = new FuncExp(f, bs);
      return p;
    };

    ~FuncExpFactory();
  };

};  // namespace VAL

#endif

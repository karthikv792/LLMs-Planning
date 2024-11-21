// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#include <map>
#include <vector>

namespace VAL {

  class var_symbol;
  class const_symbol;
  class Validator;

};  // namespace VAL

//#undef vector
//#undef map

using std::map;
using std::vector;

#ifndef __MYENVIRONMENT
#define __MYENVIRONMENT

//#define vector std::vector

namespace VAL {

  template < class T >
  bool operator!=(T &t1, T &t2) {
    return !(t1 == t2);
  };

  struct Environment : public map< const var_symbol *, const const_symbol * > {
    static map< Validator *, vector< Environment * > > copies;

    double duration;

    Environment *copy(Validator *v) const {
      Environment *e = new Environment(*this);
      copies[v].push_back(e);
      // cout << "Copy of "<<this<<" to "<<e<<"\\\\\n";
      return e;
    };

    static void collect(Validator *v) {
      for (vector< Environment * >::iterator i = copies[v].begin();
           i != copies[v].end(); ++i)
        delete *i;
      copies[v].clear();

      // cout << "Deleting the copies of enviroments here!\\\\\n";
    };
  };

  template < class TI >
  struct EnvironmentParameterIterator {
    Environment *env;
    TI pi;

    EnvironmentParameterIterator(Environment *f, TI p) : env(f), pi(p){};

    // Having to cast the const is not good...currently we are forced to do it
    // in
    // order to interact with Cascader, but should look at fixing it.
    const_symbol *operator*() {
      if (const_symbol *s = const_cast< const_symbol * >(
              dynamic_cast< const const_symbol * >(*pi))) {
        return s;
      };
      return const_cast< const_symbol * >(
          (*env)[dynamic_cast< const var_symbol * >(*pi)]);
    };

    EnvironmentParameterIterator &operator++() {
      ++pi;
      return *this;
    };

    bool operator==(const EnvironmentParameterIterator< TI > &li) const {
      return pi == li.pi;
    };

    bool operator!=(const EnvironmentParameterIterator< TI > &li) const {
      return pi != li.pi;
    };
  };

  template < class TI >
  EnvironmentParameterIterator< TI > makeIterator(Environment *f, TI p) {
    return EnvironmentParameterIterator< TI >(f, p);
  };

};  // namespace VAL

#endif

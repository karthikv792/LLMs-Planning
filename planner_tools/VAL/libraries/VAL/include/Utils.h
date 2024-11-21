// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#include <sstream>
#include <string>

#ifndef __VALUTILS
#define __VALUTILS

using std::ostringstream;
using std::string;

namespace VAL {

  template < typename T >
  struct ToStringer {
    string operator()(T d) {
      ostringstream aStringStream;
      aStringStream << d;
      return aStringStream.str();
    };
  };

  template < typename T >
  struct ToStringer< T * > {
    string operator()(T *d) {
      ostringstream aStringStream;
      aStringStream << *d;
      return aStringStream.str();
    };
  };

  template < typename T >
  string toString(T d) {
    ToStringer< T > ts;
    return ts(d);
  };

  void replaceSubStrings(string &s, string s1, string s2);

  void latexString(string &s);

};  // namespace VAL

#endif

// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#ifndef LIBLINK_H_INCLUDED
#define LIBLINK_H_INCLUDED
#ifdef SUPPORTEXTERNALS

#include <ptree.h>
#include <string>
#include <vector>

using std::string;
using std::vector;

using VAL::class_def;

namespace ExternalSupport {

  class LibLoader {
   private:
    vector< void * > handles;
    static void *getLib(class_def *cd);

   public:
    static double makeCall(class_def *cd, const string &nm,
                           vector< string > args);
    ~LibLoader();
  };

};  // namespace ExternalSupport

#endif
#endif  // LIBLINK_H_INCLUDED

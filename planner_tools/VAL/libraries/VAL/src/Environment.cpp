// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#include "Environment.h"

namespace VAL {

  map< Validator *, vector< Environment * > > Environment::copies =
      map< Validator *, vector< Environment * > >();
};

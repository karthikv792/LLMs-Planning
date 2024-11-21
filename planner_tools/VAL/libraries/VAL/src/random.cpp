// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#include "random.h"

namespace VAL {

  NormalGen Generators::randomNumberNormGenerator = NormalGen();
  UniformGen Generators::randomNumberUniGenerator = UniformGen(0, 0, 1);

  // return a random number with norm prob over -1 to 1
  double getRandomNumberNormal() {
    double randomNumber;
    do {
      randomNumber = Generators::randomNumberNormGenerator() * 0.25;
    } while (randomNumber > 1.0 || randomNumber < -1.0);

    // cout << randomNumber << " \\\\\n";
    return randomNumber;
  };

  // return a random number with uniform prob over 0 to 1
  double getRandomNumberUniform() {
    // double randomNumber = double(rand()) / double(RAND_MAX);
    double randomNumber = Generators::randomNumberUniGenerator();

    return randomNumber;
  };

  double getRandomNumberPsuedoNormal() {
    int noToAverage = 4;
    double total = 0;

    for (int i = 1; i <= noToAverage; ++i) {
      total += getRandomNumberUniform();
    };

    return total / noToAverage;
  };

};  // namespace VAL

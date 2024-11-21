// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#ifndef SIM__EXCEPTIONS
#define SIM__EXCEPTIONS
#include <exception>
using std::exception;

namespace VAL {

  struct UndefinedClassError : public exception {
    const char* what() const throw() {
      return "Function in undefined class expression!";
    }
  };

  struct UndefinedPolyError : public exception {
    const char* what() const throw() {
      return "Attempt to access undefined polynomial!";
    };
  };

  struct HighOrderDiffEqunError : public exception {
    const char* what() const throw() {
      return "Higher order differential equations of this nature are not "
             "handled!";
    };
  };

  struct DiffEqunError : public exception {
    const char* what() const throw() {
      return "Differential equations of this nature are not handled!";
    };
  };

  struct NumError : public exception {
    const char* what() const throw() {
      return "Numerical error in calculation of Primitive Numerical Values!";
    };
  };

  struct InfiniteRootsError : public exception {
    const char* what() const throw() {
      return "Attempt to find roots of constant value zero!";
    };
  };

  struct PolyRootError : public exception {
    const char* what() const throw() {
      return "The roots of this polynomial cannot be found!";
    };
  };

  struct ApproxPolyError : public exception {
    const char* what() const throw() {
      return "An approximate polynomial to this continuous function cannot be "
             "found!";
    };
  };

  struct InvariantError : public exception {
    const char* what() const throw() {
      return "This validator cannot handle invariants of this nature!";
    };
  };

  struct InvariantDisjError : public exception {
    const char* what() const throw() {
      return "The intervals satisfying a disjunct of an invariant cannot be "
             "found!";
    };
  };

  struct InvalidIntervalsError : public exception {
    const char* what() const throw() {
      return "This collection of intervals is invalid!";
    };
  };

  struct DerivedPredicateError : public exception {
    const char* what() const throw() {
      return "Problem with derived predicates!";
    };
  };

  struct TemporalDAError : public exception {
    const char* what() const throw() {
      return "Conditional effects cannot depend on future events!";
    };
  };

  struct BadOperator : public exception {
    const char* what() const throw() { return "Bad operator in plan!"; };
  };

  struct SyntaxTooComplex : public exception {
    const char* what() const throw() {
      return "Syntax not handled by this validator!";
    };
  };

  struct UnrecognisedCondition : public exception {
    const char* what() const throw() {
      return "Unrecognised exception - unexpected situation!";
    };
  };

  struct BadAccessError : public exception {
    const char* what() const throw() {
      return "Attempt to access undefined numeric expression!";
    };
  };

  struct ParseFailure : public exception {
    const char* what() const throw() {
      return "Parser failed to read file!\n";
    };
  };

  struct TypeException : public exception {
    const char* what() const throw() { return "Error in type-checking!\n"; };
  };

};  // namespace VAL

#endif

// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#ifndef __INSTANTIATION
#define __INSTANTIATION
#include "FastEnvironment.h"
#include <algorithm>
#include <iterator>
#include <map>
#include <vector>

using std::ostream_iterator;
class operator_;
class problem;
class TypeChecker;
class pddl_type;
class const_symbol;

using std::deque;
using std::map;
using std::vector;

#include "TypedAnalyser.h"

class instantiatedOp;
class Literal;
typedef deque< instantiatedOp * > OpStore;

struct LiteralParameterIterator {
  FastEnvironment *env;
  parameter_symbol_list::const_iterator pi;

  LiteralParameterIterator(FastEnvironment *f,
                           parameter_symbol_list::const_iterator p)
      : env(f), pi(p){};

  const_symbol *operator*() { return (*env)[*pi]; };

  LiteralParameterIterator &operator++() {
    ++pi;
    return *this;
  };

  bool operator==(LiteralParameterIterator &li) { return pi == li.pi; };

  bool operator!=(LiteralParameterIterator &li) { return pi != li.pi; };
};

class Literal {
 private:
  const proposition *prop;
  FastEnvironment *env;

 public:
  Literal(const proposition *p, FastEnvironment *e) : prop(p), env(e){};

  struct LiteralParametersOutput {
    const FastEnvironment &bindings;

    LiteralParametersOutput(const FastEnvironment &bs) : bindings(bs){};
    string operator()(const parameter_symbol *v) const {
      return bindings[v]->getName();
    };
  };

  void write(ostream &o) const {
    o << "  " << prop->head->getName() << "_";
    transform(prop->args->begin(), prop->args->end(),
              ostream_iterator< string >(o, "_"),
              LiteralParametersOutput(*env));
  };

  const pred_symbol *getHead() const { return prop->head; };

  LiteralParameterIterator begin() {
    return LiteralParameterIterator(env, prop->args->begin());
  };
  LiteralParameterIterator end() {
    return LiteralParameterIterator(env, prop->args->end());
  };
};

ostream &operator<<(ostream &o, const Literal &io);

class LiteralStore {
 private:
  typedef map< const pred_symbol *, CascadeSet< const_symbol * > > PredMap;

  PredMap literals;
  deque< Literal * > allLits;

 public:
  void write(ostream &o) const {
    for (deque< Literal * >::const_iterator i = allLits.begin();
         i != allLits.end(); ++i) {
      o << **i << "\n";
    };
  };
  void write(instantiatedOp *op, ostream &o) const;

  void insert(Literal *lit);
};

class instantiatedOp {
 private:
  const operator_ *op;
  FastEnvironment *env;

  static OpStore instOps;

  static map< pddl_type *, vector< const_symbol * > > values;

  struct ActionParametersOutput {
    const FastEnvironment &bindings;

    ActionParametersOutput(const FastEnvironment &bs) : bindings(bs){};
    string operator()(const var_symbol *v) const {
      return bindings[v]->getName();
    };
  };

  static LiteralStore literals;
  LiteralStore pres;
  LiteralStore adds;
  LiteralStore dels;

 public:
  instantiatedOp(const operator_ *o, FastEnvironment *e) : op(o), env(e){};
  static void instantiate(const operator_ *op, const problem *p,
                          TypeChecker &tc);
  ~instantiatedOp() { delete env; };

  void write(ostream &o) const {
    o << "  " << op->name->getName() << "_";
    transform(op->parameters->begin(), op->parameters->end(),
              ostream_iterator< string >(o, "_"), ActionParametersOutput(*env));
  };

  static void writeAll(ostream &o);
  static int howMany() { return instOps.size(); };

  static void createAllLiterals(problem *p);
  void collectLiterals();
  static void writeAllLiterals(ostream &o);
  void writePres(ostream &o) const;
  void writeAdds(ostream &o) const;
  void writeDels(ostream &o) const;
};

ostream &operator<<(ostream &o, const instantiatedOp &io);

#endif

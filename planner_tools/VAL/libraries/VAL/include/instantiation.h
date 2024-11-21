// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#ifndef __INSTANTIATION
#define __INSTANTIATION
#include "FastEnvironment.h"
#include "SimpleEval.h"
#include <algorithm>
#include <assert.h>
#include <iterator>
#include <map>
#include <vector>

using std::for_each;
using std::insert_iterator;
using std::ostream_iterator;
using std::unary_function;

namespace VAL {
  class operator_;
  class problem;
  class TypeChecker;
  class pddl_type;
  class const_symbol;
}  // namespace VAL

using namespace VAL;

using std::deque;
using std::map;
using std::vector;

#include "TypedAnalyser.h"

namespace Inst {

  bool varFree(const VAL::parameter_symbol_list *pl);

  class instantiatedOp;
  class instantiatedDrv;

  template < typename S, typename V >
  class GenStore;

  class Literal;
  class PNE;

  typedef GenStore< VAL::pred_symbol, Literal > LiteralStore;
  typedef GenStore< VAL::func_symbol, PNE > PNEStore;

  class instantiatedDrvUtils {
   public:
    struct index {
      VAL::pred_symbol *first;
      const VAL::derivation_rule *second;

      index(VAL::pred_symbol *a, const VAL::derivation_rule *b)
          : first(a), second(b){};
    };

   private:
    struct indexLT {
      bool operator()(const index &a, const index &b) const {
        VAL::pred_symbol *afirst =
            VAL::current_analysis->pred_tab.symbol_get(a.first->getName());
        VAL::pred_symbol *bfirst =
            VAL::current_analysis->pred_tab.symbol_get(b.first->getName());
        if (afirst < bfirst) return true;
        if (afirst > bfirst) return false;

        if (a.second < b.second) return true;
        return false;
      };
    };
    static set< index, indexLT > definitive;
    static bool initDefinitive;

   public:
    static index *purify(const index *);
  };

  template < class T >
  class FlexiblePrint {
   private:
    const char *before;
    const char *after;
    ostream_iterator< T > os;

   public:
    FlexiblePrint(ostream &o, const char *b, const char *a)
        : before(b), after(a), os(ostream_iterator< T >(o)){};
    FlexiblePrint(const FlexiblePrint &f)
        : before(f.before), after(f.after), os(f.os){};

    virtual FlexiblePrint &operator=(const FlexiblePrint &f) {
      os = f.os;
      before = f.before;
      after = f.after;
      return *this;
    }

    virtual FlexiblePrint &operator=(const T &f) {
      os = before;
      os = f;
      os = after;
      return *this;
    }

    virtual FlexiblePrint &operator*() { return *this; };

    virtual FlexiblePrint &operator++() {
      os++;
      return *this;
    };

    virtual FlexiblePrint &operator++(int) {
      ++os;
      return *this;
    };
  };

  class PNE {
   private:
    /** @brief A unique integer identifier for the PNE. */
    int id;

    /** @brief A unique integer identifier for non-static PNEs.
     *
     * If a PNE is static (never changed by any effects), its state ID is
     * -1 - its value is not stored during search.  Otherwise, it has a
     * unique identifier.
     */
    int stateID;

    const VAL::func_term *func;
    VAL::FastEnvironment *env;

    const VAL::func_term *realisation;

   protected:
    friend class instantiatedOp;
    friend class GenStore< VAL::func_symbol, PNE >;

    void setID(const int &x) { id = x; }

    void setStateID(const int &x) { stateID = x; }

   public:
    PNE(const VAL::func_term *f, VAL::FastEnvironment *e)
        : id(0), stateID(-1), func(f), env(e), realisation(0) {
      if (varFree(func->getArgs())) {
        realisation = func;
      };
    };

    const VAL::func_term *toFuncTerm() {
      if (!realisation) {
        VAL::parameter_symbol_list *pl = new VAL::parameter_symbol_list();
        for (VAL::parameter_symbol_list::const_iterator i =
                 func->getArgs()->begin();
             i != func->getArgs()->end(); ++i) {
          pl->push_back((*env)[*i]);
        };
        realisation = new VAL::func_term(
            const_cast< VAL::func_symbol * >(func->getFunction()), pl);
      };
      return realisation;
    };

    struct PNEParametersOutput
        : unary_function< const VAL::parameter_symbol *, string > {
      const VAL::FastEnvironment &bindings;

      PNEParametersOutput(const VAL::FastEnvironment &bs) : bindings(bs){};
      string operator()(const VAL::parameter_symbol *v) const {
        return bindings[v]->getName();
      };
    };

    void write(ostream &o) const {
      o << "(" << func->getFunction()->getName();
      transform(func->getArgs()->begin(), func->getArgs()->end(),
                FlexiblePrint< string >(o, " ", ""), PNEParametersOutput(*env));
      o << ")";
    };

    const VAL::func_symbol *getHead() const { return func->getFunction(); };

    const VAL::func_term *getFunc() const { return func; };

    VAL::LiteralParameterIterator< VAL::parameter_symbol_list::const_iterator >
        begin() {
      return makeIterator(env, func->getArgs()->begin());
    };
    VAL::LiteralParameterIterator< VAL::parameter_symbol_list::const_iterator >
        end() {
      return makeIterator(env, func->getArgs()->end());
    };
    const VAL::parameter_symbol *operator[](int n) {
      VAL::LiteralParameterIterator<
          VAL::parameter_symbol_list::const_iterator >
          i = begin();
      for (; n > 0; --n, ++i)
        ;
      return *i;
    };
    // int getID() const {return id;};
    // void setID(int x) {id = x;};
    int getStateID() const { return stateID; }

    /** @brief @deprecated  Use <code>getGlobalID()</code> */
    // int getID() const {
    //    return id;
    //}

    int getGlobalID() const { return id; }
  };

  ostream &operator<<(ostream &o, const PNE &p);

  class Literal {
   protected:
    /** @brief A unique integer identifier for the fact. */
    int id;

    /** @brief A unique integer identifier for non-static facts.
     *
     * If a fact is static (never changed by any effects or TILs), its state ID
     * is -1 - its value is not stored during search.  Otherwise, it has a
     * unique identifier.
     */
    int stateID;

    const VAL::proposition *prop;
    VAL::FastEnvironment *env;

    const VAL::proposition *realisation;

   protected:
    friend class instantiatedOp;
    friend class GenStore< VAL::pred_symbol, Literal >;

    void setID(const int &x) { id = x; }

    void setStateID(const int &x) { stateID = x; }

   public:
    Literal(const VAL::proposition *p, VAL::FastEnvironment *e)
        : id(0), stateID(-1), prop(p), env(e), realisation(0) {
      if (varFree(prop->args)) {
        realisation = prop;
      };
    };

    VAL::FastEnvironment *getEnv() const { return env; }

    const VAL::proposition *toProposition() {
      if (!realisation) {
        VAL::parameter_symbol_list *pl = new VAL::parameter_symbol_list;
        for (VAL::parameter_symbol_list::iterator i = prop->args->begin();
             i != prop->args->end(); ++i) {
          pl->push_back((*env)[*i]);
        };
        realisation = new VAL::proposition(prop->head, pl);
      };
      return realisation;
    };

    struct LiteralParametersOutput {
      const VAL::FastEnvironment &bindings;

      LiteralParametersOutput(const VAL::FastEnvironment &bs) : bindings(bs){};
      string operator()(const VAL::parameter_symbol *v) const {
        return bindings[v]->getName();
      };
    };

    void write(ostream &o) const {
      o << "(" << prop->head->getName();
      transform(prop->args->begin(), prop->args->end(),
                FlexiblePrint< string >(o, " ", ""),
                LiteralParametersOutput(*env));
      o << ")";
    };

    const VAL::pred_symbol *getHead() const { return prop->head; };

    const VAL::proposition *getProp() const { return prop; };

    VAL::LiteralParameterIterator< VAL::parameter_symbol_list::iterator >
        begin() {
      return makeIterator(env, prop->args->begin());
    };
    VAL::LiteralParameterIterator< VAL::parameter_symbol_list::iterator >
        end() {
      return makeIterator(env, prop->args->end());
    };
    VAL::parameter_symbol *operator[](int n) {
      VAL::LiteralParameterIterator< VAL::parameter_symbol_list::iterator > i =
          begin();
      for (; n > 0; --n, ++i)
        ;
      return *i;
    };
    //	int getID() const {return id;};
    //	void setID(int x) {id = x;};

    int getGlobalID() const { return id; }

    /** @brief @deprecated  Use <code>getGlobalID()</code> */
    // int getID() const {
    //    return id;
    //}

    int getStateID() const { return stateID; }
    virtual ~Literal(){};
  };

  struct CreatedLiteral : public Literal {
    CreatedLiteral(const VAL::proposition *p, VAL::FastEnvironment *e)
        : Literal(p, e){};

    ~CreatedLiteral() { delete env; };
  };

  ostream &operator<<(ostream &o, const Literal &io);

  template < typename S >
  struct Purifier {
    const S *operator()(const S *s) { return s; };
  };

  using VAL::current_analysis;
  using VAL::func_symbol;
  using VAL::pred_symbol;

  template <>
  struct Purifier< pred_symbol > {
    const pred_symbol *operator()(const pred_symbol *p) {
      return current_analysis->pred_tab.symbol_get(p->getName());
    };
  };

  template <>
  struct Purifier< instantiatedDrvUtils::index > {
    const instantiatedDrvUtils::index *operator()(
        const instantiatedDrvUtils::index *p) {
      return instantiatedDrvUtils::purify(p);
    };
  };

  template <>
  struct Purifier< func_symbol > {
    const func_symbol *operator()(const func_symbol *f) {
      return current_analysis->func_tab.symbol_get(f->getName());
    };
  };

  template < typename S, typename V >
  class GenStore {
   private:
    typedef map< const S *, CascadeMap< VAL::const_symbol *, V > > PredMap;

    PredMap literals;
    deque< V * > allLits;

    Purifier< S > purify;

    bool locked;

   public:
    GenStore() : locked(false) {}

    void preventFurtherModification() { locked = true; }

    void write(ostream &o) const {
      for (typename deque< V * >::const_iterator i = allLits.begin();
           i != allLits.end(); ++i) {
        if (*i) o << **i << "\n";
      };
    };

    V *insert(V *lit) {
      assert(!locked);
      V *&str =
          literals[purify(lit->getHead())].forceGet(lit->begin(), lit->end());

      if (str == 0) {
        str = lit;
        allLits.push_back(lit);
        lit->setID(allLits.size() - 1);
        return 0;
      }
      return str;
    };

    V *find(V *lit) {
      V *&str =
          literals[purify(lit->getHead())].myGet(lit->begin(), lit->end());
      return str;
    };

    set< V * > allContents(const S *p) {
      set< V * > slits;
      for (typename CascadeMap< VAL::const_symbol *, V >::iterator i =
               literals[purify(p)].begin();
           i != literals[purify(p)].end(); ++i) {
        slits.insert(*i);
      };
      slits.erase((V *)0);
      return slits;
    };

    typedef typename deque< V * >::iterator iterator;
    typedef typename deque< V * >::const_iterator const_iterator;

    iterator begin() { return allLits.begin(); };
    iterator end() { return allLits.end(); };
    const_iterator begin() const { return allLits.begin(); };
    const_iterator end() const { return allLits.end(); };

    size_t size() const { return allLits.size(); };
    V *operator[](int x) const { return allLits[x]; };
    template < typename TI >
    V *get(S *s, TI b, TI e) {
      return literals[purify(s)].get(b, e);
    };

    template < typename TI >
    V *find(S *s, TI b, TI e) {
      return literals[purify(s)].myGet(b, e);
    };

    void erase(const V *v) {
      literals[purify(v->getHead())].myGet(v->begin(), v->end()) = 0;
      allLits[v->getID()] = 0;
    }

    void clearUp() {
      allLits.erase(std::remove(allLits.begin(), allLits.end(), ((V *)0)),
                    allLits.end());
    }

    void clear() {
      iterator itr = begin();
      const iterator itrEnd = end();

      for (; itr != itrEnd; ++itr) delete *itr;

      literals.clear();
      allLits.clear();
    }
  };

  class instantiatedOp;
  class instantiatedDrv;

  typedef GenStore< VAL::operator_symbol, instantiatedOp > OpStore;
  typedef GenStore< instantiatedDrvUtils::index, instantiatedDrv > DrvStore;

  class LitStoreEvaluator : public PrimitiveEvaluator {
   private:
    LiteralStore &literals;

   public:
    LitStoreEvaluator(bool &v, bool &u, bool &w, bool &x, LiteralStore &lits)
        : PrimitiveEvaluator(v, u, w, x), literals(lits){};

    virtual void evaluateSimpleGoal(VAL::FastEnvironment *f,
                                    VAL::simple_goal *s);
  };

  template <>
  class PrimitiveEvaluatorConstructor< LitStoreEvaluator > {
   private:
    LiteralStore &literals;

   public:
    PrimitiveEvaluatorConstructor(LiteralStore &lits) : literals(lits){};
    PrimitiveEvaluator *operator()(bool &v, bool &u, bool &w, bool &x) {
      return new LitStoreEvaluator(v, u, w, x, literals);
    }
  };

  typedef PrimitiveEvaluatorConstructor< LitStoreEvaluator > LSE;

  class instantiatedOp {
   private:
    int id;
    const VAL::operator_ *op;
    VAL::FastEnvironment *env;

    static OpStore instOps;

    static map< VAL::pddl_type *, vector< VAL::const_symbol * > > &values;

    struct ActionParametersOutput {
      const VAL::FastEnvironment &bindings;

      ActionParametersOutput(const VAL::FastEnvironment &bs) : bindings(bs){};
      string operator()(const VAL::var_symbol *v) const {
        return bindings[v]->getName();
      };
    };

    static LiteralStore &literals;
    static PNEStore &pnes;

    static int nonStaticLiteralCount;
    static int nonStaticPNECount;

    static bool staticFactsAndLiteralsHaveBeenGivenIDs;

   public:
#ifndef NDEBUG
    static const VAL::operator_ *insistOnOp;
    static vector< VAL::const_symbol * > insistOnOpParameters;
    static const instantiatedOp *opBeforeFiltering;
#endif

    instantiatedOp(const VAL::operator_ *o, VAL::FastEnvironment *e)
        : id(0), op(o), env(e){};
    static void instantiate(const VAL::operator_ *op, const VAL::problem *p,
                            VAL::TypeChecker &tc);
    ~instantiatedOp() { delete env; };

    /** @brief  Erase any ground operators whose preconditions are trivially
     * unreachable. */
    static void filterOps(VAL::TypeChecker *const);

    /** @brief  Assign unique identifiers to non-static literals and PNEs.
     *
     * @see Literal::stateID , PNE::stateID
     */
    static void assignStateIDsToNonStaticLiteralsAndPNEs();

    /** @brief  Erase the given instantiated operator. */
    static void opErase(const instantiatedOp *o) { instOps.erase(o); }

    /** @brief  Print the name of the instantiated operator.
     *
     *  The printed name is of the form <code>(operatorname param1 param2
     * param3)</code>, and is suitable for inclusion in plans to be parsed by
     * the validator.
     */
    void write(ostream &o) const {
      o << "(" << op->name->getName();
      transform(op->parameters->begin(), op->parameters->end(),
                FlexiblePrint< string >(o, " ", ""),
                ActionParametersOutput(*env));
      o << ")";
    };

    /** @brief  Return the number of parameters of the operator. */
    int arity() const { return op->parameters->size(); };

    /** @brief  Return the object used as the ith parameter of this instantiated
     * operator. */
    const VAL::const_symbol *getArg(int i) const {
      VAL::var_symbol_list::const_iterator a = op->parameters->begin();
      for (; i > 0; --i, ++a)
        ;
      return (*env)[*a];
    };

    /** @brief  Print the names of all the instantiated operators. */
    static void writeAll(ostream &o);

    /** @brief  Return the number of instantiated operators. */
    static int howMany() { return instOps.size(); };

    /** @brief  Return the number of ground <code>Literal</code>s (including
     * static literals). */
    static int howManyLiteralsOfAnySort() { return literals.size(); }

    /** @brief  Return the number of ground non-static <code>Literal</code>s.
     *
     * This function should only be called after a call to
     * <code>instantiatedOp::assignStateIDsToNonStaticLiteralssAndPNEs()</code>
     * has been made.
     */
    static int howManyNonStaticLiterals() {
      assert(staticFactsAndLiteralsHaveBeenGivenIDs);
      return nonStaticLiteralCount;
    }

    /** @brief  Return the number of ground <code>PNE</code>s (including static
     * PNEs). */
    static int howManyPNEsOfAnySort() { return pnes.size(); }

    /** @brief  Return the number of ground non-static <code>PNE</code>s.
     *
     * This function should only be called after a call to
     * <code>instantiatedOp::assignStateIDsToNonStaticLiteralssAndPNEs()</code>
     * has been made.
     */
    static int howManyNonStaticPNEs() {
      assert(staticFactsAndLiteralsHaveBeenGivenIDs);
      return nonStaticPNECount;
    }
    static void createAllLiterals(VAL::problem *p, VAL::TypeChecker *tc);
    void collectLiterals(VAL::TypeChecker *tc);
    static void writeAllLiterals(ostream &o);
    static void writeAllPNEs(ostream &o);
    static OpStore::iterator opsBegin() { return instOps.begin(); };
    static OpStore::iterator opsEnd() { return instOps.end(); };

    static PNEStore::iterator pnesBegin() { return pnes.begin(); };
    static PNEStore::iterator pnesEnd() { return pnes.end(); };

    static LiteralStore::iterator literalsBegin() { return literals.begin(); };
    static LiteralStore::iterator literalsEnd() { return literals.end(); };

    static map< VAL::pddl_type *, vector< VAL::const_symbol * > > &getValues() {
      return values;
    };

    typedef VAL::FastEnvironment::const_iterator const_iterator;
    const_iterator begin() const { return env->begin(); };
    const_iterator end() const { return env->end(); };
    static instantiatedOp *getInstOp(int i) { return instOps[i]; };
    template < typename TI >
    static instantiatedOp *getInstOp(const VAL::operator_ *op, TI sp, TI ep) {
      return getInstOp(op->name, sp, ep);
    };
    template < typename TI >
    static instantiatedOp *getInstOp(VAL::operator_symbol *osym, TI sp, TI ep) {
      return instOps.get(osym, sp, ep);
    };
    template < typename TI >
    static instantiatedOp *getInstOp(const string &opname, TI sp, TI ep) {
      VAL::operator_symbol *osym(
          VAL::current_analysis->op_tab.symbol_get(opname));
      return getInstOp(osym, sp, ep);
    };
    template < typename TI >
    static instantiatedOp *findInstOp(VAL::operator_symbol *osym, TI sp,
                                      TI ep) {
      return instOps.find(osym, sp, ep);
    };
    static OpStore::iterator from(int k) { return opsBegin() + k; };
    const VAL::operator_ *forOp() const { return op; };
    VAL::FastEnvironment *getEnv() { return env; };
    static Literal *getLiteral(Literal *l) { return literals.insert(l); };
    static Literal *findLiteral(Literal *l) { return literals.find(l); };
    static void preventFurtherModificationOfLiterals() {
      literals.preventFurtherModification();
    }

    static PNE *getPNE(PNE *p) { return pnes.insert(p); };
    static PNE *findPNE(PNE *p) { return pnes.find(p); };
    template < typename TI >
    static PNE *findPNE(VAL::func_symbol *f, TI a, TI b) {
      return pnes.get< TI >(f, a, b);
    }

    static set< Literal * > allLiterals(const VAL::pred_symbol *p) {
      return literals.allContents(p);
    };
    const VAL::operator_symbol *getHead() const { return op->name; };
    int getID() const { return id; };
    void setID(int x) { id = x; };

    // added by AMC to find PNEs which match a func_symbol
    static set< PNE * > allPNEs(const VAL::func_symbol *f) {
      return pnes.allContents(f);
    };

    friend class Collector;

    class PropEffectsIterator {
     private:
      instantiatedOp *inst;
      bool isPos;
      VAL::pc_list< VAL::simple_effect * >::iterator effs;
      Literal *lit;

     public:
      PropEffectsIterator(instantiatedOp *io, bool pos)
          : inst(io),
            isPos(pos),
            effs(pos ? io->op->effects->add_effects.begin()
                     : io->op->effects->del_effects.begin()),
            lit(0){};
      void toEnd() {
        effs = isPos ? inst->op->effects->add_effects.end()
                     : inst->op->effects->del_effects.end();
      };
      bool operator==(const PropEffectsIterator &i) const {
        return effs == i.effs;
      };
      bool operator!=(const PropEffectsIterator &i) const {
        return effs != i.effs;
      };
      PropEffectsIterator &operator++() {
        ++effs;
        lit = 0;
        return *this;
      };
      Literal *operator*() {
        if (!lit) {
          Literal l((*effs)->prop, inst->getEnv());
          lit = instantiatedOp::getLiteral(&l);
        };
        return lit;
      };
    };

    class PNEEffectsIterator {
     private:
      instantiatedOp *inst;
      VAL::pc_list< VAL::assignment * >::iterator effs;
      VAL::pc_list< VAL::assignment * >::iterator effsend;
      VAL::pc_list< VAL::timed_effect * >::iterator effts;
      VAL::pc_list< VAL::timed_effect * >::iterator efftsend;
      PNE *pne;

     public:
      PNEEffectsIterator(instantiatedOp *io)
          : inst(io),
            effs(io->op->effects->assign_effects.begin()),
            effsend(io->op->effects->assign_effects.end()),
            effts(io->op->effects->timed_effects.begin()),
            efftsend(io->op->effects->timed_effects.end()),
            pne(0) {
        while (effts != efftsend && (*effts)->ts != VAL::E_CONTINUOUS) ++effts;
        if (effs == effsend && effts != efftsend) {
          effs = (*effts)->effs->assign_effects.begin();
          // cout << "GOT " << **effs << "\n";
          effsend = (*effts)->effs->assign_effects.end();
          ++effts;
          while (effts != efftsend && (*effts)->ts != VAL::E_CONTINUOUS)
            ++effts;
        };
      };
      void toEnd() {
        effs = effsend;
        effts = efftsend;
      };
      bool operator==(const PNEEffectsIterator &i) const {
        return effts == i.effts &&
               (effs == i.effs ||
                (effts == efftsend && effs == effsend && i.effs == i.effsend));
      };
      bool operator!=(const PNEEffectsIterator &i) const {
        return !(*this == i);
      };
      PNE *operator*() {
        if (!pne) {
          PNE p((*effs)->getFTerm(), inst->getEnv());
          pne = instantiatedOp::getPNE(&p);
        };
        return pne;
      };
      PNEEffectsIterator &operator++() {
        ++effs;
        if (effs == effsend && effts != efftsend) {
          effs = (*effts)->effs->assign_effects.begin();
          // cout << "GOT " << **effs << "\n";
          effsend = (*effts)->effs->assign_effects.end();
          ++effts;
          while (effts != efftsend && (*effts)->ts != VAL::E_CONTINUOUS)
            ++effts;
        };
        pne = 0;
        return *this;
      };
      const VAL::expression *getUpdate() { return (*effs)->getExpr(); };
      const VAL::assign_op getOp() const { return (*effs)->getOp(); };
    };

    PropEffectsIterator addEffectsBegin();
    PropEffectsIterator addEffectsEnd();
    PropEffectsIterator delEffectsBegin();
    PropEffectsIterator delEffectsEnd();
    PNEEffectsIterator PNEEffectsBegin();
    PNEEffectsIterator PNEEffectsEnd();

    // added by AMC to test whether a goal can be satisfied by an
    // InstantiatedOp

    bool isGoalMetByOp(const Literal *lit);
    bool isGoalMetByEffect(const VAL::effect_lists *effs, const Literal *lit);
    bool isGoalMetByEffect(VAL::simple_effect *seff, const Literal *lit);
    bool isGoalMetByEffect(VAL::forall_effect *fleff, const Literal *lit);
    bool isGoalMetByEffect(VAL::cond_effect *ceff, const Literal *lit);
    bool isGoalMetByEffect(VAL::timed_effect *teff, const Literal *lit);
  };

  class instantiatedDrv {
   private:
    int id;
    const VAL::derivation_rule *op;
    VAL::FastEnvironment *env;

    instantiatedDrvUtils::index localHead;

    static DrvStore instDrvs;

    static map< VAL::pddl_type *, vector< VAL::const_symbol * > > &values;

    struct DerivationRuleParametersOutput {
      const VAL::FastEnvironment &bindings;

      DerivationRuleParametersOutput(const VAL::FastEnvironment &bs)
          : bindings(bs){};
      string operator()(const VAL::var_symbol *v) const {
        return bindings[v]->getName();
      };
      string operator()(const VAL::parameter_symbol *v) const {
        return bindings[v]->getName();
      };
    };

    static LiteralStore &literals;
    static PNEStore &pnes;

   public:
    instantiatedDrv(const VAL::derivation_rule *o, VAL::FastEnvironment *e)
        : id(0), op(o), env(e), localHead(o->get_head()->head, o){};
    static void instantiate(const VAL::derivation_rule *op,
                            const VAL::problem *p, VAL::TypeChecker &tc);
    ~instantiatedDrv() { delete env; };

    static void filterDrvs(VAL::TypeChecker *const);
    static void drvErase(const instantiatedDrv *o) { instDrvs.erase(o); }

    void write(ostream &o) const {
      o << "(derive-" << op->get_head()->head->getName() << "-" << op;
      transform(op->get_head()->args->begin(), op->get_head()->args->end(),
                FlexiblePrint< string >(o, " ", ""),
                DerivationRuleParametersOutput(*env));
      o << ")";
    };
    int arity() const { return op->get_head()->args->size(); };

    const VAL::const_symbol *getArg(int i) const {
      VAL::parameter_symbol_list::const_iterator a =
          op->get_head()->args->begin();
      for (; i > 0; --i, ++a)
        ;
      return (*env)[*a];
    };
    static void writeAll(ostream &o);
    static int howMany() { return instDrvs.size(); };

    static void createAllLiterals(VAL::problem *p, VAL::TypeChecker *tc);
    void collectLiterals(VAL::TypeChecker *tc);

    static DrvStore::iterator drvsBegin() { return instDrvs.begin(); };
    static DrvStore::iterator drvsEnd() { return instDrvs.end(); };

    typedef VAL::FastEnvironment::const_iterator const_iterator;
    const_iterator begin() const { return env->begin(); };
    const_iterator end() const { return env->end(); };
    static instantiatedDrv *getInstDrv(int i) { return instDrvs[i]; };
    template < typename TI >
    static instantiatedDrv *getInstDrv(const VAL::derivation_rule *op, TI sp,
                                       TI ep) {
      return getInstDrv(op->get_head()->head->getName(), sp, ep);
    };
    template < typename TI >
    static instantiatedOp *getInstDrv(VAL::pred_symbol *osym, TI sp, TI ep) {
      return instDrvs.get(osym, sp, ep);
    };
    template < typename TI >
    static instantiatedOp *getInstDrv(const string &opname, TI sp, TI ep) {
      VAL::pred_symbol *osym(
          VAL::current_analysis->pred_tab.symbol_get(opname));
      return getInstDrv(osym, sp, ep);
    };
    static DrvStore::iterator from(int k) { return drvsBegin() + k; };
    const VAL::derivation_rule *forDrv() const { return op; };
    VAL::FastEnvironment *getEnv() { return env; };

    // const VAL::pred_symbol * getHead() const {return op->get_head()->head;};

    const instantiatedDrvUtils::index *getHead() const { return &localHead; };

    int getID() const { return id; };
    void setID(int x) { id = x; };

    friend class Collector;
  };

  ostream &operator<<(ostream &o, const instantiatedOp &io);
  ostream &operator<<(ostream &o, const instantiatedDrv &io);

  VAL::const_symbol *const getConst(string name);

  VAL::const_symbol *const getConst(const char *name);

  template < typename V, typename I >
  struct FType {
    typedef typename std::iterator_traits< I >::value_type VT;
    typedef V (*FnType)(VT);
  };

  template < typename V, typename I, typename FType< V, I >::FnType f >
  class Iterator {
   private:
    I myIt;

   public:
    Iterator(I i) : myIt(i){};
    Iterator &operator++() {
      ++myIt;
      return *this;
    };
    V operator*() { return f(*myIt); };
    bool operator==(const Iterator &i) { return myIt == i.myIt; };
  };

};  // namespace Inst

#endif

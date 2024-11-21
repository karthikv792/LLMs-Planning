// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#include "valLib.h"
#include "AccumulatedDelta.h"
#include "Action.h"
#include "FastEnvironment.h"
#include "FlexLexer.h"
#include "Plan.h"
#include "PrettyPrinter.h"
#include "State.h"
#include "Validator.h"
#include "ptree.h"
#include "typecheck.h"
#include <cstring>
#include <fstream>
#include <map>
#include <sstream>
#include <vector>

using std::ifstream;
using std::map;
using std::ofstream;
using std::stringstream;

char *current_filename;

namespace VAL {

  parse_category *top_thing;

  analysis an_analysis;
  analysis *current_analysis;

  yyFlexLexer *yfl;
  int Silent;
  int errorCount;
  bool Verbose;
  bool ContinueAnyway = false;
  bool ErrorReport = false;
  bool InvariantWarnings = false;
  bool LaTeX = false;
  TypeChecker *theTC;
  ostream *report = &std::cout;

};  // namespace VAL

extern int yyparse();
extern int yydebug;

using namespace VAL;

class Tracker : public StateObserver {
 private:
  map< const FuncExp *, FEScalar > &oldValues;
  double time;

 public:
  Tracker(map< const FuncExp *, FEScalar > &ovs) : oldValues(ovs), time(-0.1){};
  virtual void notifyChanged(const State *s, const Happening *h) {
    if (s->getTime() > time) {
      time = s->getTime();
      oldValues.clear();
    }
    set< const FuncExp * > chPNEs = s->getChangedPNEs();
    for (map< const FuncExp *, FEScalar >::iterator i = oldValues.begin();
         i != oldValues.end(); ++i) {
      oldValues[i->first] = s->getPriorValue(i->first);
    }

    for (set< const FuncExp * >::const_iterator i = chPNEs.begin();
         i != chPNEs.end(); ++i) {
      oldValues[*i] = s->getPriorValue(*i);
    }
  }
};

class ValuesTracker : public StateObserver {
 private:
  Validator * vld;
  map< const FuncExp *, vector< pair< double, double > > > allValues;
  vector<pair<double,vector<double> > > metrics; // Use this field to track the metrics - we'll do this by default
  int numMets;
  double time;

 public:
  ValuesTracker(Validator * v) : vld(v), numMets(0), time(-0.1){};
  virtual void notifyChanged(const State *s, const Happening *h) {
    // cout << "Told of change at " << s->getTime() << "\n";
    if (s->getTime() > time) {
      time = s->getTime();
    }
    set< const FuncExp * > chPNEs = s->getChangedPNEs();
    if(!chPNEs.empty()) // Deal with metrics
    {
      vector<double> mets(numMets);
      if(numMets > 0)
      {
        vld->computeMetric(s,mets);

      }
      metrics.push_back(make_pair(time,mets));

    }
    for (map< const FuncExp *, vector< pair< double, double > > >::iterator i =
             allValues.begin();
         i != allValues.end(); ++i) {
      // cout << "Checking for " << i->first->getName() << "\n";
      if (time >= 0 && i->second.empty()) {
        i->second.push_back(make_pair(0, s->getPriorValue(i->first)));
      }
      if (chPNEs.find(i->first) != chPNEs.end()) {
        // cout << "Found\n";
        if (h->isEventHappening() ||
            h->isRegularHappening()) {  // cout << "This is a step effect on "
                                        // << i->first->getName() << " at " <<
                                        // time << "\n";
          if (i->second[i->second.size() - 1].first < time)
            i->second.push_back(
                make_pair(time, i->second[i->second.size() - 1].second));
        }

        try {
          i->second.push_back(make_pair(time, s->evaluate(i->first)));
        }
        catch(...)
        {
          i->second.push_back(make_pair(time, nan("")));
        }
      }
       else if(time > i->second.back().first && h->isRegularHappening())
                i->second.push_back(make_pair(time,s->evaluate(i->first)));
      // else cout << "Not found\n";
    }
  }
  void trackMets(int n) {numMets = n;}
  void add(const FuncExp *fe) {
    allValues[fe] = vector< pair< double, double > >();
  }
  int getNumValues(const FuncExp *fe) { return allValues[fe].size(); }
  double getNextValue(const FuncExp *fe, double &t, int &n) {
    // cout << "Got " << allValues[fe].size() << " values stored\n";
    if (n < allValues[fe].size()) {
      t = allValues[fe][n].first;
      double ret = allValues[fe][n].second;
      if (++n >= allValues[fe].size()) n = -1;
      return ret;
    }
    n = -1;
    t = -1;
    return 0;
  }
  double getNextMetric(const unsigned int m,double & t, int & n)
  {
    if(n < metrics.size())
    {
      t = metrics[n].first;
      double v = metrics[n].second[m];
      if(++n >= metrics.size()) n = -1;
      return v;
    }
    n = -1;
    t = -1;
    return 0;
  }
};


class SimulatorValidator {
 private:
  Validator *vld;
  ActiveCtsEffects ace;
  double theTime;
  Plan::timedActionSeq tas;
  map< double, Happening * > happenings;
  map< int, Action * > recorded;
  vector< plan_step * > store;
  map< const FuncExp *, FEScalar > oldValues;

  TypeChecker *tc;
  AccumulatedDelta ad;
  ValuesTracker *vtrack;

  vector< LPCSTR > allocated;
  int numMetrics;

 public:
  SimulatorValidator(Validator *v)
      : vld(v), ace(v), theTime(-0.1), tc(0), vtrack(0), numMetrics(0) {
    v->prepareToExecute();
    Verbose = false;
    ContinueAnyway = true;
    State::addObserver(new Tracker(oldValues));
    State::addObserver(&ad);
  };
  ~SimulatorValidator() {
    for (int i = 0; i < allocated.size(); ++i) {
      delete[] allocated[i];
    }
  }
  int trackMetrics() {
      if(!vtrack) 
      {
          vtrack = new ValuesTracker(vld);
          State::addObserver(vtrack);
      }
      numMetrics = vld->numMetrics();
      vtrack->trackMets(numMetrics);
      return numMetrics;
  };
  void ownTC(TypeChecker *t) { tc = t; };
  bool executeNext();
  void reset() { ad.reset(); };
  bool getLit(const proposition *prp, const FastEnvironment *fenv) const;
  double getVal(const func_term *ft, const FastEnvironment *fenv) const;
  double getPriorVal(const func_term *ft, const FastEnvironment *fenv) const;
  bool add(LPCSTR action[], int aID, bool starting, double t);
  unsigned long getLitCode(const char *lit[]);
  unsigned long getFunCode(const char *fun[]);
  unsigned long *getChangedLits(int &n);
  unsigned long *getChangedFuns(int &n);
  bool queryLiteralCode(unsigned long code);
  double queryValueCode(unsigned long code);
  double queryPriorValueCode(unsigned long code);
  LPCSTR *requestLit(unsigned long code, int &n);
  LPCSTR *requestFun(unsigned long code, int &n);
  void assertLitValue(unsigned long code, bool val);
  void assertFunValue(unsigned long code, double val);
  void write(ostream &o) const {
    if (vld) o << vld->getState();
  }
  void trackFE(unsigned long cd);
  double getNextValue(unsigned long cd, double &t, int &n);
  LPCSTR strAlloc(const string &s) {
    char *cs = new char[s.length() + 1];
    strcpy(cs, s.c_str());
    allocated.push_back(cs);
    return cs;
  }
};

ostream &operator<<(const SimulatorValidator &sv, ostream &o) {
  sv.write(o);
  return o;
}

void SimulatorValidator::trackFE(unsigned long cd) {
  if (!vtrack) {
    vtrack = new ValuesTracker(vld);
    State::addObserver(vtrack);
    // cout << "Added a tracker\n";
  }
  vtrack->add((FuncExp *)cd);
}

double SimulatorValidator::getNextValue(unsigned long cd, double &t, int &n) {
  if(cd < numMetrics && vtrack)
  {
    return vtrack->getNextMetric(cd,t,n);
  }
  // cout << "Checking for " << ((FuncExp *)cd)->getName() << "\n";
  if (vtrack) return vtrack->getNextValue((FuncExp *)cd, t, n);
  t = -1;
  return 0;
}

void SimulatorValidator::assertLitValue(unsigned long code, bool val) {
  // Unpleasant cast necessary to overcome usual const ness of the State
  // cout << "Setting lit " << code << " to " << val << "\n";
  if (val) {
    const_cast< State * >(&(vld->getState()))->add((SimpleProposition *)code);
  } else {
    const_cast< State * >(&(vld->getState()))->del((SimpleProposition *)code);
  }
}

void SimulatorValidator::assertFunValue(unsigned long code, double val) {
  // cout << "Setting " << code << " to " << val << "\n";
  const_cast< State * >(&(vld->getState()))
      ->update((FuncExp *)code, E_ASSIGN, val);
}

bool SimulatorValidator::queryLiteralCode(unsigned long code) {
  return vld->getState().evaluate((SimpleProposition *)code);
}

LPCSTR *SimulatorValidator::requestLit(unsigned long code, int &n) {
  SimpleProposition *sp = (SimpleProposition *)code;
  n = 1 + sp->getProp()->args->size();
  LPCSTR *arr = new LPCSTR[n];
  arr[0] = strAlloc(sp->getPred()->getName());
  for (int i = 1; i < n; ++i) {
    arr[i] = strAlloc(sp->getParameter(i));
  }
  return arr;
}

LPCSTR *SimulatorValidator::requestFun(unsigned long code, int &n) {
  FuncExp *fe = (FuncExp *)code;
  n = 1 + fe->getArity();
  LPCSTR *arr = new LPCSTR[n];
  arr[0] = strAlloc(fe->getName());
  for (int i = 1; i < n; ++i) {
    arr[i] = strAlloc(fe->getParameter(i));
  }
  return arr;
}

double SimulatorValidator::queryValueCode(unsigned long code) {
  try {
    return vld->getState().evaluate((FuncExp *)code);
  }
  catch(...)
  {
    return nan("");
  }
}

double SimulatorValidator::queryPriorValueCode(unsigned long code) {
  return oldValues[(FuncExp *)code];
}

unsigned long *SimulatorValidator::getChangedLits(int &n) {
  const set< const SimpleProposition * > &ps = ad.getChangedLiterals();
  n = ps.size();
  unsigned long *psa = new unsigned long[n];
  int x = 0;
  for (set< const SimpleProposition * >::const_iterator i = ps.begin();
       i != ps.end(); ++i, ++x) {
    psa[x] = (unsigned long)(*i);
  }
  return psa;
}

unsigned long *SimulatorValidator::getChangedFuns(int &n) {
  const set< const FuncExp * > &ps = ad.getChangedPNEs();
  n = ps.size();
  unsigned long *psa = new unsigned long[n];
  int x = 0;
  for (set< const FuncExp * >::const_iterator i = ps.begin(); i != ps.end();
       ++i, ++x) {
    psa[x] = (unsigned long)(*i);
  }
  return psa;
}

unsigned long SimulatorValidator::getLitCode(const char *lit[]) {
  pred_symbol *p = current_analysis->pred_tab.symbol_get(string(lit[0]));
  for (pred_decl_list::const_iterator i =
           current_analysis->the_domain->predicates->begin();
       i != current_analysis->the_domain->predicates->end(); ++i) {
    if (p == (*i)->getPred()) {
      // cout << "Pred " << p->getName() << " found\n";
      var_symbol_list::const_iterator k = (*i)->getArgs()->begin();
      Environment e;
      int arity = (*i)->getArgs()->size();
      for (int j = 0; j < arity; ++j, ++k) {
        e[*k] = current_analysis->const_tab.symbol_probe(string(lit[j + 1]));
        // if(!e[*k]) cout << "We have a problem with " << lit[j+1] << "\n";
      }
      proposition *pr =
          new proposition(p, const_cast< var_symbol_list * >((*i)->getArgs()));
      // MEMORY MANAGEMENT!!!
      unsigned long c = (unsigned long)vld->pf.buildLiteral(pr, e);
      // cout << "Code " << c << "\n";
      // pr.args = 0;
      return c;
    }
  }
  return 0;
}

unsigned long SimulatorValidator::getFunCode(const char *fun[]) {
  func_symbol *p = current_analysis->func_tab.symbol_get(string(fun[0]));
  for (func_decl_list::const_iterator i =
           current_analysis->the_domain->functions->begin();
       i != current_analysis->the_domain->functions->end(); ++i) {
    if (p == (*i)->getFunction()) {
      // cout << "Fun " << p->getName() << " found\n";
      var_symbol_list::const_iterator k = (*i)->getArgs()->begin();
      Environment e;
      int arity = (*i)->getArgs()->size();
      for (int j = 0; j < arity; ++j, ++k) {
        e[*k] = current_analysis->const_tab.symbol_get(string(fun[j + 1]));
      }
      func_term *pr = new func_term(
          p, const_cast< parameter_symbol_list * >(
                 reinterpret_cast< const parameter_symbol_list * >(
                     (*i)->getArgs())));  // Check that this cast will work...
      // Memory management!!
      unsigned long c = (unsigned long)vld->fef.buildFuncExp(pr, e);
      // cout << "Code " << c << "\n";
      // pr.setArgs(0);
      return c;
    }
  }
  return 0;
}

bool SimulatorValidator::executeNext() {
  if (happenings.empty()) return true;
  double tnext = -1;
  Happening *h = 0;
  for (map< double, Happening * >::const_iterator i = happenings.begin();
       i != happenings.end(); ++i) {
    if (i->first > theTime && (tnext < 0 || tnext > i->first)) {
      tnext = i->first;
      h = i->second;
    }
  }

  if (h == 0) return true;
  theTime = tnext;

  bool isOk = vld->extendPlan(h);
  while (true) {
    isOk = vld->executeOneStep() && isOk;
    if (!vld->isLastHappening()) {
      isOk = vld->followThroughOneStep(isOk);
    } else
      break;
  }
  return isOk;
}

bool SimulatorValidator::getLit(const proposition *prp,
                                const FastEnvironment *fenv) const {
  Environment env;

  for (list< parameter_symbol * >::const_iterator i = prp->args->begin();
       i != prp->args->end(); ++i) {
    var_symbol *v = dynamic_cast< var_symbol * >(*i);
    if (v) {
      env[v] = (*fenv)[*i];
    }
  }
  const SimpleProposition *sp = vld->pf.buildLiteral(prp, env);
  return vld->getState().evaluate(sp);
}

double SimulatorValidator::getVal(const func_term *ft,
                                  const FastEnvironment *fenv) const {
  Environment env;

  for (list< parameter_symbol * >::const_iterator i = ft->getArgs()->begin();
       i != ft->getArgs()->end(); ++i) {
    var_symbol *v = dynamic_cast< var_symbol * >(*i);
    if (v) {
      env[v] = (*fenv)[*i];
    }
  }
  const FuncExp *fe = vld->fef.buildFuncExp(ft, env);
  FEScalar fv = vld->getState().evaluate(fe);
  return fv;
}

double SimulatorValidator::getPriorVal(const func_term *ft,
                                       const FastEnvironment *fenv) const {
  Environment env;

  for (list< parameter_symbol * >::const_iterator i = ft->getArgs()->begin();
       i != ft->getArgs()->end(); ++i) {
    var_symbol *v = dynamic_cast< var_symbol * >(*i);
    if (v) {
      env[v] = (*fenv)[*i];
    }
  }
  const FuncExp *fe = vld->fef.buildFuncExp(ft, env);
  if (oldValues.find(fe) == oldValues.end()) {
    // cout << *fe << " HAS VALUE " << vld->getState().evaluate(fe) << "\n";
    return vld->getState().evaluate(fe);
  }
  FEScalar fv = oldValues.find(fe)->second;
  // cout << *fe << " HAD VALUE " << fv << "\n";
  return fv;
}

int findArity(const operator_symbol *op) {
  for (list< operator_ * >::const_iterator i =
           current_analysis->the_domain->ops->begin();
       i != current_analysis->the_domain->ops->end(); ++i) {
    if (op == (*i)->name) {
      return (*i)->parameters->size();
    }
  }
  return 0;
}

plan_step *makePlanStep(LPCSTR actionStr[], double start) {
  const_symbol_list *cs = new const_symbol_list();
  operator_symbol *act = current_analysis->op_tab.symbol_get(actionStr[0]);
  int arity = findArity(act);
  // Set up parameters array
  //
  for (int i = 1; i <= arity; ++i) {
    cs->push_back(current_analysis->const_tab.symbol_get(actionStr[i]));
  }

  plan_step *ps = new plan_step(act, cs);
  ps->start_time = start;
  ps->start_time_given = true;
  ps->duration_given = false;
  ps->duration = 1;  // Just to put it into the future...
  return ps;
}

/*
plan_step * makePlanStep(instantiatedOp * a,double start)
{
    const_symbol_list * cs = new const_symbol_list();
    for(vector<const_symbol*>::const_iterator i = a->getEnv()->begin(); i !=
a->getEnv()->end(); ++i)
    {
        cs->push_back(*i);

    }

    plan_step * ps = new
plan_step(const_cast<operator_symbol*>(a->getHead()),cs); ps->start_time =
start; ps->start_time_given = true; ps->duration_given = false; ps->duration =
1; // Just to put it into the future... return ps;

}
*/

bool SimulatorValidator::add(LPCSTR action[], int aID, bool starting,
                             double when) {
  // We could use a 0 action pointer as a signal to advance time...
  Action *act;

  if (starting) {
    tas.clear();
    Plan::planBuilder pb(vld, tas, current_analysis->the_domain->ops, 0);
    plan_step *ps = makePlanStep(action, when);
    pb(ps);

    act = tas[0].second;
    recorded[aID] = tas[1].second;
    store.push_back(ps);
  } else {
    // Identify the action that we are ending using earlier record
    act = recorded[aID];
  }

  if (when > theTime) {
    if (happenings.find(when) != happenings.end()) {
      happenings.find(when)->second->inject(act);
    } else {
      vector< pair< double, Action * > > as;
      as.push_back(make_pair(when, act));
      happenings.insert(make_pair(when, new Happening(vld, as, when + 1)));
      // cout << *(happenings[when]) << "\n";
    }

    return true;
  }

  return false;
}

void *makeValidator(void *an, double tol, void *typechecker) {
  current_analysis = (analysis *)an;
  const DerivationRules *derivRules = new DerivationRules(
      current_analysis->the_domain->drvs, current_analysis->the_domain->ops);
  return new SimulatorValidator(
      new Validator(derivRules, tol, *((TypeChecker *)typechecker),
                    current_analysis->the_domain->ops,
                    current_analysis->the_problem->initial_state,
                    current_analysis->the_problem->metric, true, true,
                    current_analysis->the_domain->constraints,
                    current_analysis->the_problem->constraints));
}

void *makeValidatorFromIStreams(std::istream &domain, std::istream &problem,
                                double tol) {
  current_analysis = new analysis();  //&an_analysis;
  // an_analysis.const_tab.symbol_put(""); //for events - undefined symbol
  // Silent = 0;
  // errorCount = 0;
  Verbose = false;
  ContinueAnyway = false;
  // ErrorReport = false;
  // Robust = false;
  // JudderPNEs = false;
  // EventPNEJuddering = false;
  // TestingPNERobustness = false;
  // RobustPNEJudder = 0;

  InvariantWarnings = false;
  LaTeX = false;
  ofstream possibleLatexReport;
  makespanDefault = false;
  stepLengthDefault = false;
  // bool CheckDPs = true;
  // bool giveAdvice = true;

  // double tolerance = 0.01;
  // bool lengthDefault = true;
  // double robustMeasure = 0;
  // int noTestPlans = 1000;
  // bool calculateActionRobustness = false;
  // bool calculatePNERobustness = false;
  // RobustMetric robustMetric = MAX;
  // RobustDist robustDist = UNIFORM;

  string s;
  bool ganttObjectsGot = false;
  // cout << "Creating a lexer for the domain\n";
  yfl = new yyFlexLexer(&domain, &cout);

  yydebug = 0;
  // cout << "Parse domain...\n";
  yyparse();
  // cout << "Done that\n";
  delete yfl;
  // cout << "All done\n";
  if (!current_analysis->the_domain) {
    cerr << "Problem in domain definition!\n";
    if (LaTeX)
      *report << "\\section{Error!} Problem in domain definition! \n "
                 "\\end{document}\n";
    exit(-1);
  };
  // cout << "Creating type checker\n";
  // cout << "We have read the domain:\n";
  // cout << current_analysis->the_domain->name << "\n";

  TypeChecker *tc = new TypeChecker(current_analysis);
  // cout << "Ready to typecheck domain\n";
  Verbose = false;
  bool typesOK = tc->typecheckDomain();
  // cout << "Now ready to look at problem\n";
  yfl = new yyFlexLexer(&problem, &cout);
  yyparse();
  // cout << "Parsed that\n";
  delete yfl;
  // cout << "Cleaned up\n";
  typesOK = tc->typecheckProblem();

  SimulatorValidator *sv =
      (SimulatorValidator *)makeValidator(current_analysis, tol, tc);
  sv->ownTC(tc);
  return sv;
}

void *makeValidatorFromFiles(LPCSTR domain, LPCSTR problem, double tol) {
  ifstream dom(domain);
  ifstream prob(problem);

  return makeValidatorFromIStreams(dom, prob, tol);
}

bool post(void *validator, int aID, LPCSTR action[], bool starting,
          double when) {
  return ((SimulatorValidator *)validator)->add(action, aID, starting, when);
}

bool executeNext(void *validator) {
  return ((SimulatorValidator *)validator)->executeNext();
}

bool queryLiteral(const void *validator, const void *literm, const void *fenv) {
  return ((const SimulatorValidator *)validator)
      ->getLit((const proposition *)literm, (const FastEnvironment *)fenv);
}

double queryValue(const void *validator, const void *ft, const void *fenv) {
  return ((const SimulatorValidator *)validator)
      ->getVal((const func_term *)ft, (const FastEnvironment *)fenv);
}

double queryPriorValue(const void *validator, const void *ft,
                       const void *fenv) {
  return ((const SimulatorValidator *)validator)
      ->getPriorVal((const func_term *)ft, (const FastEnvironment *)fenv);
}

unsigned long getLitCode(const void *validator, const char *lit[]) {
  return ((SimulatorValidator *)validator)->getLitCode(lit);
}

unsigned long getFunCode(const void *validator, const char *pne[]) {
  return ((SimulatorValidator *)validator)->getFunCode(pne);
}

unsigned long *getChangedLits(const void *validator, int &n) {
  return ((SimulatorValidator *)validator)->getChangedLits(n);
}

unsigned long *getChangedFuns(const void *validator, int &n) {
  return ((SimulatorValidator *)validator)->getChangedFuns(n);
}

void resetDelta(const void *validator) {
  ((SimulatorValidator *)validator)->reset();
}

bool queryLiteralCode(const void *validator, unsigned long code) {
  return ((SimulatorValidator *)validator)->queryLiteralCode(code);
}

double queryValueCode(const void *validator, unsigned long code) {
  return ((SimulatorValidator *)validator)->queryValueCode(code);
}

bool queryLiteralNamed(const void *validator, LPCSTR lit[]) {
  unsigned long cd = ((SimulatorValidator *)validator)->getLitCode(lit);
  return queryLiteralCode(validator, cd);
}

double queryValueNamed(const void *validator, LPCSTR fun[]) {
  unsigned long cd = ((SimulatorValidator *)validator)->getFunCode(fun);
  return queryValueCode(validator, cd);
}

double queryPriorValueNamed(const void *validator, LPCSTR fun[]) {
  return queryPriorValueCode(validator, getFunCode(validator, fun));
}

double queryPriorValueCode(const void *validator, unsigned long code) {
  return ((SimulatorValidator *)validator)->queryPriorValueCode(code);
}

LPCSTR *whatLit(const void *validator, unsigned long code, int &n) {
  return ((SimulatorValidator *)validator)->requestLit(code, n);
}

LPCSTR *whatFun(const void *validator, unsigned long code, int &n) {
  return ((SimulatorValidator *)validator)->requestFun(code, n);
}

void assertLitValue(void *validator, unsigned long code, bool val) {
  ((SimulatorValidator *)validator)->assertLitValue(code, val);
}

void assertFunValue(void *validator, unsigned long code, double val) {
  ((SimulatorValidator *)validator)->assertFunValue(code, val);
}

void cleanStore(unsigned long *v) { delete[] v; }

void cleanName(LPCSTR *nm) { delete[] nm; }

void cleanState(LPCSTR nm) { delete[] nm; }

void cleanUp(void *vld) { delete (SimulatorValidator *)(vld); }

LPCSTR *finalState(LPCSTR domainContent, LPCSTR problemContent, LPCSTR *actions,
                   int *aID, int *startOrEnd, double *times, int nEntries,
                   int &nState) {
  std::istringstream domain(domainContent);
  std::istringstream problem(problemContent);
  SimulatorValidator *vld =
      (SimulatorValidator *)makeValidatorFromIStreams(domain, problem, 0.001);

  for (int i = 0; i < nEntries; ++i) {
    std::istringstream act(actions[i]);
    vector< string > vals;
    string s;
    while (act >> s) {
      vals.push_back(s);
    }
    LPCSTR *actBits = new LPCSTR[vals.size()];
    for (int j = 0; j < vals.size(); ++j) {
      actBits[j] = vld->strAlloc(vals[j]);
    }

    vld->add(actBits, aID[i], startOrEnd[i] == 0, times[i]);
    delete[] actBits;
  }
  for (int i = 0; i < nEntries; ++i) {
    vld->executeNext();  // Ignore whether this executed OK?
  }

  static char *s = "=";
  static char *pred = "predicate:";
  int k;
  unsigned long *chlits = vld->getChangedLits(k);
  vector< LPCSTR > bits;
  for (int i = 0; i < k; ++i) {
    int n;
    LPCSTR *pieces =
        vld->requestLit(chlits[i], n);  // n is the number of args + 1 for the
                                        // predicate, +2 for the =true;false

    const char *eval = queryLiteralCode(vld, chlits[i]) ? "true" : "false";
    char *cpy = new char[11];
    strcpy(cpy, pred);
    bits.push_back(cpy);
    for (int j = 0; j < n; ++j) {
      cpy = new char[strlen(pieces[j]) + 1];
      strcpy(cpy, pieces[j]);
      bits.push_back(cpy);
    }
    cpy = new char[2];
    strcpy(cpy, s);
    bits.push_back(cpy);
    cpy = new char[strlen(eval) + 1];
    strcpy(cpy, eval);
    bits.push_back(cpy);

    cleanName(pieces);
  }
  cleanStore(chlits);

  static char *func = "function:";

  unsigned long *chfuns = vld->getChangedFuns(k);  // assume I can reuse this
                                                   // int

  for (int i = 0; i < k; ++i) {
    int n;
    char *cpy = new char[10];
    strcpy(cpy, func);
    bits.push_back(cpy);

    LPCSTR *pieces = vld->requestFun(
        chfuns[i], n);  // n is the number of args + 1 for the function

    for (int j = 0; j < n; ++j) {
      cpy = new char[strlen(pieces[j]) + 1];
      strcpy(cpy, pieces[j]);
      bits.push_back(cpy);
    }
    cpy = new char[2];
    strcpy(cpy, s);
    bits.push_back(cpy);

    double value = queryValueCode(vld, chfuns[i]);
    stringstream theval;
    theval << value;
    char *val = new char[theval.str().size() + 1];
    strcpy(val, theval.str().c_str());
    bits.push_back(val);
    cleanName(pieces);
  }
  cleanStore(chfuns);

  k = bits.size();
  LPCSTR *finalbits = new LPCSTR[k];
  nState = k;

  for (int i = 0; i < k; ++i) {
    finalbits[i] = bits[i];
  }

  cleanUp(vld);
  return finalbits;
}

void cleanStateDelta(LPCSTR *fs, int n) {
  for (int i = 0; i < n; ++i) delete[] fs[i];
  delete[] fs;
}

LPCSTR getState(void *v) {
  SimulatorValidator *vld = (SimulatorValidator *)(v);
  ostringstream out;
  out << "(define (problem " << current_analysis->the_problem->name
      << "_progressed)\n(:domain " << current_analysis->the_problem->domain_name
      << ")\n";

  if (current_analysis->the_problem->objects) {
    out << "(:objects\n";

    for (const_symbol_list::const_iterator i =
             current_analysis->the_problem->objects->begin();
         i != current_analysis->the_problem->objects->end(); ++i) {
      out << (*i)->getName();
      if ((*i)->type) out << " - " << (*i)->type->getName();
      out << "\n";
    }

    out << ")\n";
  }

  out << "\n(:init\n";
  vld->write(out);
  out << ")\n\n(:goal\n";
  PrettyPrinter *pp = new PrettyPrinter();
  pp->setShowType(false);
  parse_category::setWriteController(std::unique_ptr< WriteController >(pp));
  current_analysis->the_problem->the_goal->write(out);
  out << "))\n";

  char *str = new char[out.str().size() + 1];
  strcpy(str, out.str().c_str());
  return str;
}

void trackFE(void *v, unsigned long cd) {
  SimulatorValidator *vld = (SimulatorValidator *)(v);
  vld->trackFE(cd);
}

int trackMetrics(void * v)
{
  SimulatorValidator *vld = (SimulatorValidator *)(v);
  return vld->trackMetrics();
}

double getNextValue(void *v, unsigned long cd, double &t, int &n) {
  SimulatorValidator *vld = (SimulatorValidator *)(v);
  return vld->getNextValue(cd, t, n);
}

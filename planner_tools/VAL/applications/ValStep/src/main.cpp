// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#include "valLib.h"
#include <iostream>
#include <stdio.h>
#include <vector>

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

// Here is a very simple example for using the library.
// NB: Currently, the library is called libvalstep.dll and is stored in
// bin\Debug for compilation purposes (linker needs to see it). The header
// valLib.h is in the main project director (VALSTEP). For execution, the
// library needs to be in the execution directory, so a copy is also currently
// stored in VALSTEP, along with the domain and problem files for this example.

// Run and use as follows:
// ? prompt can be responded to with...
//    start <action name and parameters> @ time
// where time is a number relative to the start of the plan (and must be >= now)
// the action name and parameters must all be lower case strings matching the
// domain action being used together with appropriate constants. The library is
// not (currently) robust to errors in this!
//    end <n> @ time
// This ends the action whose identity is number n (reported to the user as
// feedback when the action started) at time as given.
//    x
// Execute the next happening (which might be multiple action starts or ends at
// the same time point).
//    q
// Quit.

// Eg:
// ? start getoutofslips @ 0
// ? x
// ...
// ? end 1 @ 1
// ? x
// ...
// ? q

// We are currently not robust to various unpleasant events such as executing
// with an empty set of actions.

#include "FlexLexer.h"
#include "VisitController.h"

using std::ifstream;
using std::map;
using std::ofstream;
using std::stringstream;
using std::vector;

namespace VAL {

  extern parse_category *top_thing;
  extern analysis an_analysis;
  extern yyFlexLexer *yfl;
}  // namespace VAL

extern int yyparse();
extern int yydebug;

using namespace VAL;
using namespace std;

class PlanBit {
 private:
  const plan_step *p;
  bool start;
  int act;

 public:
  PlanBit(const plan_step *p1, bool c, int a) : p(p1), start(c), act(a) {}

  void write(ostream &o) const {
    if (start) {
      o << "start " << p->op_sym->getName() << " ";
      for (const_symbol_list::const_iterator i = p->params->begin();
           i != p->params->end(); ++i) {
        o << (*i)->getName() << " ";
      }
      o << "@ " << p->start_time;
    } else {
      o << "end " << act << " @ " << p->start_time + p->duration;
    }
    // if(p->duration_given) o << "[" << p->duration << "]";
  }

  void handle(void *vld) {
    // cout << "Handle " << p->op_sym->getName() << "\n";
    if (start) {
      LPCSTR *nm = new LPCSTR[p->params->size() + 1];
      nm[0] = new char[p->op_sym->getName().size() + 1];
      strcpy((char *)nm[0], p->op_sym->getName().c_str());
      int j = 1;
      for (const_symbol_list::const_iterator i = p->params->begin();
           i != p->params->end(); ++i, ++j) {
        nm[j] = new char[(*i)->getName().size() + 1];
        strcpy((char *)nm[j], (*i)->getName().c_str());
      }

      post(vld, act, nm, true, p->start_time);
      for (int i = 0; i <= p->params->size(); ++i) {
        delete[] nm[i];
      }
      delete[] nm;
    } else
      post(vld, act, 0, false, p->start_time + p->duration);
  }
};

ostream &operator<<(ostream &o, const PlanBit &p) {
  p.write(o);
  return o;
}

class PlanProcessor : public VisitController {
 private:
  map< double, list< PlanBit > > planbits;
  int x;

 public:
  PlanProcessor() : x(0){};

  virtual void visit_plan_step(plan_step *p) {
    ++x;
    planbits[p->start_time].push_back(PlanBit(p, true, x));

    if (p->duration_given) {
      planbits[p->start_time + p->duration].push_back(PlanBit(p, false, x));
    }
  };

  void show() {
    for (map< double, list< PlanBit > >::const_iterator i = planbits.begin();
         i != planbits.end(); ++i) {
      for (list< PlanBit >::const_iterator j = i->second.begin();
           j != i->second.end(); ++j) {
        cout << *j << "\n";
      }
      cout << "x\n";
    }
  }

  void buildFrom(char *argv) {
    current_analysis = &an_analysis;
    ifstream plan(argv);

    yfl = new yyFlexLexer(&plan, &cout);

    yydebug = 0;
    yyparse();
    delete yfl;
    if (top_thing) {
      // cout << "Got plan\n";
      top_thing->visit(this);
      /*
                 int nEntries = ts.size();

                 int aID[nEntries];
                 int startOrEnd[nEntries];
                 double timesA[nEntries];
                 LPCSTR actions[nEntries];

                 for(int i = 0;i < nEntries;++i)
                 {
                     aID[i] = ids[i];
                     startOrEnd[i] = stEnds[i];
                     timesA[i] = ts[i];
                     actions[i] = acts[i];
                 }

                 int nState;
                 LPCSTR * finals =
         finalState(thedomainfile.c_str(),theproblemfile.c_str(),actions,aID,startOrEnd,timesA,nEntries,nState);
                 ofstream out(argv[4]);
                 for(int i = 0;i < nState;++i)
                 {
                     out << finals[i];
                     if(finals[i][0]=='=')
                     {
                         out << " " << finals[i+1] << "\n";
                         ++i;
                     }
                     else out << " ";
                 }
                 cleanStateDelta(finals,nState);
     */
    }
  }

  void runPlan(char *dom, char *prob, char *out, bool wantState) {
    void *vld = makeValidatorFromFiles(dom, prob, 0.001);

    for (map< double, list< PlanBit > >::iterator i = planbits.begin();
         i != planbits.end(); ++i) {
      for (list< PlanBit >::iterator j = i->second.begin();
           j != i->second.end(); ++j) {
        j->handle(vld);
      }
      executeNext(vld);
    }

    ofstream outp(out);
    if (wantState) {
      LPCSTR st = getState(vld);
      outp << st;
      cleanState(st);
    } else {
      int n;
      unsigned long *vals = getChangedLits(vld, n);
      for (int i = 0; i < n; ++i) {
        int x;
        LPCSTR *nm = whatLit(vld, vals[i], x);

        if (queryLiteralCode(vld, vals[i])) {
          outp << "(";
          for (int j = 0; j < x; ++j) {
            outp << nm[j];
            if (j < x - 1) outp << " ";
          }
          outp << ")\n";
        } else {
          outp << "(not (";
          for (int j = 0; j < x; ++j) {
            outp << nm[j];
            if (j < x - 1) outp << " ";
          }
          outp << "))\n";
        }
        cleanName(nm);
      }
      cleanStore(vals);

      vals = getChangedFuns(vld, n);
      for (int i = 0; i < n; ++i) {
        int x;
        LPCSTR *nm = whatFun(vld, vals[i], x);
        outp << "(= (";
        for (int j = 0; j < x; ++j) {
          outp << nm[j];
          if (j < x - 1) outp << " ";
        }
        outp << ") " << queryValueCode(vld, vals[i]) << ")\n";
        cleanName(nm);
      }
      cleanStore(vals);
    }
    cleanUp(vld);
  }
};

const char **splitString(char *input, const char *separator, int *n) {
  int len = strlen(input);

  char *input2 = (char *)malloc(len + 1);
  strcpy(input2, input);

  char *p = strtok(input, " ");
  int nParts = 0;
  while (p) {
    p = strtok(NULL, " ");
    nParts++;
  }

  (*n) = nParts;

  const char **res = NULL;
  res = (const char **)realloc(res, sizeof(char *) * nParts);

  p = strtok(input2, " ");
  int j;
  for (j = 0; j < nParts; j++) {
    res[j] = p;
    p = strtok(NULL, " ");
  }
  return res;
}
void calculatePlan(char *domain, char *problem, char *executedActions,
                   char *output) {
  void *vld = makeValidatorFromFiles(domain, problem, 0.001);
  // const char** action = NULL;

  FILE *file = fopen(executedActions, "r");
  char line[256];
  FILE *file2 = fopen(output, "w");

  while (fgets(line, sizeof(line), file)) {
    int id = atoi(strtok(line, ";"));

    int start = atoi(strtok(NULL, ";"));

    char *mnem = strtok(NULL, ";");
    double t = atof(strtok(NULL, ";"));
    int nSize;
    const char **mnempar = splitString(mnem, " ", &nSize);

    if (start == 0)
      post(vld, id, mnempar, true, t);
    else
      post(vld, id, 0, false, t);

    executeNext(vld);

    int n;
    unsigned long *vals = getChangedLits(vld, n);
    for (int i = 0; i < n; ++i) {
      int x;
      LPCSTR *nm = whatLit(vld, vals[i], x);
      string s;
      fprintf(file2, "predicate: ");
      for (int j = 0; j < x; ++j) {
        fprintf(file2, "%s ", nm[j]);
      }
      fprintf(file2, "= %s\n",
              queryLiteralCode(vld, vals[i]) ? "true" : "false");
      cleanName(nm);
    }
    cleanStore(vals);

    vals = getChangedFuns(vld, n);
    for (int i = 0; i < n; ++i) {
      int x;
      LPCSTR *nm = whatFun(vld, vals[i], x);
      fprintf(file2, "function: ");
      for (int j = 0; j < x; ++j) {
        fprintf(file2, "%s ", nm[j]);
      }
      fprintf(file2, "= %f\n", queryValueCode(vld, vals[i]));
      cleanName(nm);
    }
    cleanStore(vals);
    resetDelta(vld);
  }
  fclose(file);
  fclose(file2);
  cleanUp(vld);
  return;
}

void calcPlan(int argc, char *argv[]) {
  ifstream domainfile(argv[1]);
  string thedomainfile;
  thedomainfile.assign(istreambuf_iterator< char >(domainfile),
                       istreambuf_iterator< char >());

  ifstream problemfile(argv[2]);
  string theproblemfile;
  theproblemfile.assign(istreambuf_iterator< char >(problemfile),
                        istreambuf_iterator< char >());

  ifstream actionsfile(argv[3]);

  vector< LPCSTR > acts;
  vector< int > ids;
  vector< int > stEnds;
  vector< double > ts;

  string action;

  while (getline(actionsfile, action)) {
    size_t sp = action.find(";");
    string theid = action.substr(0, sp);
    int id = atoi(theid.c_str());
    ids.push_back(id);

    string startorend = action.substr(sp + 1, sp + 3);
    sp += 3;  // +1 for ; and +1 for the startorend;

    int soe = atoi(startorend.c_str());
    stEnds.push_back(soe);

    string theactionnameandparams =
        action.substr(sp, action.find(";", sp) - sp);
    sp += theactionnameandparams.size() + 1;
    char *act = new char[theactionnameandparams.size() + 1];
    strcpy(act, theactionnameandparams.c_str());
    acts.push_back(act);

    string t = action.substr(sp, action.find(";", sp));

    double time = atof(t.c_str());
    ts.push_back(time);
  }
  actionsfile.close();

  int nEntries = ts.size();

  int *aID = new int[nEntries];
  int *startOrEnd = new int[nEntries];
  double *timesA = new double[nEntries];
  LPCSTR *actions = new LPCSTR[nEntries];

  for (int i = 0; i < nEntries; ++i) {
    aID[i] = ids[i];
    startOrEnd[i] = stEnds[i];
    timesA[i] = ts[i];
    actions[i] = acts[i];
  }

  int nState;
  LPCSTR *finals =
      finalState(thedomainfile.c_str(), theproblemfile.c_str(), actions, aID,
                 startOrEnd, timesA, nEntries, nState);
  ofstream out(argv[4]);
  for (int i = 0; i < nState; ++i) {
    out << finals[i];
    if (finals[i][0] == '=') {
      out << " " << finals[i + 1] << "\n";
      ++i;
    } else
      out << " ";
  }
  cleanStateDelta(finals, nState);
  delete[] aID;
  delete[] startOrEnd;
  delete[] timesA;
  delete[] actions;
}

void calcPlanPDDL(int argc, char *argv[], bool wS) {
  PlanProcessor p;
  p.buildFrom(argv[3]);
  p.runPlan(argv[1], argv[2], argv[4], wS);
}

void runValStep(istream& in, void* vld, int aID) {
  while (true) {
    vector< string > wds;
    string s;
    cout << "? ";
    in >> s;
    if (s == "q") {
      break;
    } else if (s == "e") {
      string toEcho;
      getline(in, toEcho);
      cout << toEcho << endl;
    } else if (s == "x") {
      executeNext(vld);
      int n;
      unsigned long *vals = getChangedLits(vld, n);
      cout << "Seeing " << n << " changed lits\n";
      for (int i = 0; i < n; ++i) {
        int x;
        LPCSTR *nm = whatLit(vld, vals[i], x);
        for (int j = 0; j < x; ++j) {
          cout << nm[j] << " ";
        }
        cout << "- now " << (queryLiteralCode(vld, vals[i]) ? "true" : "false")
             << "\n";
        cleanName(nm);
      }
      cleanStore(vals);
      vals = getChangedFuns(vld, n);
      for (int i = 0; i < n; ++i) {
        int x;
        LPCSTR *nm = whatFun(vld, vals[i], x);
        for (int j = 0; j < x; ++j) {
          cout << nm[j] << " ";
        }
        cout << "- now " << (queryValueCode(vld, vals[i])) << "\n";
        cleanName(nm);
      }
      cleanStore(vals);
    } else if (s == "w") {
      in >> s;
      ofstream output(s.c_str(), ofstream::out);
      LPCSTR state = getState(vld);
      output << state;
      output.close();
      cleanState(state);
    } else if (s == "end") {
      int a;
      in >> a;
      in >> s;
      double t;
      in >> t;
      post(vld, a, 0, false, t);
    } else if (s == "assert") {
      cout << "Asserting ";
      while (true) {
        in >> s;
        if (s == "-") {
          break;
        }
        wds.push_back(s);
        cout << s << " ";
      }
      LPCSTR *fact = new LPCSTR[wds.size()];
      for (unsigned int i = 0; i < wds.size(); ++i) {
        fact[i] = wds[i].c_str();
      }
      in >> s;
      cout << " is " << s << "\n";
      unsigned long cd = getLitCode(vld, fact);
      assertLitValue(vld, cd, (s == "true"));
      delete[] fact;
    } else {
      while (true) {
        in >> s;
        if (s == "@") {
          break;
        }
        wds.push_back(s);
      }
      LPCSTR *act = new LPCSTR[wds.size()];
      for (unsigned int i = 0; i < wds.size(); ++i) {
        act[i] = wds[i].c_str();
      }
      double t;
      in >> t;
      post(vld, aID, act, true, t);
      cout << "Posted action " << aID << "\n";
      delete[] act;
      ++aID;
    }
  }
}

int main(int argc, char *argv[]) {
  /*    if(argc==5)
      {
          //calculatePlan(argv[1], argv[2], argv[3], argv[4]);

          calcPlan(argc,argv);
          exit(0);
      }
  */

  bool withState = true;
  bool fromFile  = false;
  string inputF  = "";

  while (argc > 1 && argv[1][0] == '-') {
    if (argv[1][1] == 'd') {
      withState = false;  // Just report delta
    }
    if (argv[1][1] == 'i') {
      fromFile = true;
      inputF = string(argv[2]);
      --argc;
      ++argv;
    }
    --argc;
    ++argv;
  }

  if (fromFile)
    cout << "Input File " << inputF << endl;

  if (argc == 5) {
    calcPlanPDDL(argc, argv, withState);
    exit(0);
  }

  if (argc < 2) {
    cout << "please specify your domain file and problem file\n";
    exit(0);
  }
  void *vld = makeValidatorFromFiles(argv[1], argv[2], 0.001);
  int aID = 1;

  if (fromFile){
    ifstream fin(inputF.c_str());
    runValStep(fin,vld,aID);
  } else
    runValStep(cin,vld,aID);

  LPCSTR state = getState(vld);
  cout << state;
  cleanUp(vld);
  cleanState(state);
  return 0;
}

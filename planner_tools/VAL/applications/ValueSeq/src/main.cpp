// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#include <iostream>

#include "valLib.h"

#include "ptree.h"

#include "FlexLexer.h"
#include <fstream>

#include <cstring>
#include <sstream>
#include <vector>

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

bool printTimes;
bool timeSeries;

class PlanBit {
 private:
  const plan_step *p;
  bool start;
  int act;

  vector< double > before;
  vector< double > after;

  bool wantMetrics;
  vector<double> metricsBefore;
  vector<double> metricsAfter;

 public:
  PlanBit(const plan_step *p1, bool c, int a, bool wm) : p(p1), start(c), act(a), wantMetrics(wm) {}

  PlanBit() : p(NULL), start(false), act(0) {}

  bool appearsIn(const vector< string > &tags) {
    string s(p->op_sym->getName());
    for (vector< string >::const_iterator i = tags.begin(); i != tags.end();
         ++i) {
      if (s.find(*i) != string::npos) return true;
    }
    return false;
  }

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

  void capturePre(void *vld, vector< int > &cds) {
    for (vector< int >::const_iterator i = cds.begin(); i != cds.end(); ++i) {
      before.push_back(queryValueCode(vld, *i));
    }
  }

  void handle(void *vld, vector< int > &cds) {
    // cout << "Handle " << p->op_sym->getName() << "\n";
    if (start) {
      // cout << "Query\n";
      if (!timeSeries) {
        for (vector< int >::const_iterator i = cds.begin(); i != cds.end();
             ++i) {
          before.push_back(queryValueCode(vld, *i));
        }
      }
      // cout << "Post\n";
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

  void capturePost(void *vld, vector< int > &cds) {
    if (!start || timeSeries) {
      for (vector< int >::const_iterator i = cds.begin(); i != cds.end(); ++i) {
        after.push_back(queryValueCode(vld, *i));
      }
    }
  }

  void showValues(ostream &o) const {
    if (start) {
      if (printTimes) {
        o << p->start_time << ", ";
      }
      o << p->op_sym->getName();
      for (const_symbol_list::const_iterator i = p->params->begin();
           i != p->params->end(); ++i) {
        o << " " << (*i)->getName();
      }

      for (int i = 0; i < before.size(); ++i) {
        o << ", " << before[i];
      }
    } else {
      for (int i = 0; i < after.size(); ++i) {
        o << ", " << after[i];
      }
    }
  }

  void showAll(ostream &o) const {
    if (start)
      o << p->start_time;
    else
      o << p->start_time + p->duration;

    for (vector< double >::const_iterator i = before.begin(), j = after.begin();
         i != before.end(); ++i, ++j) {
      o << ", " << *j;
    }

    o << "\n";
  }

  void showInit(ostream &o, int i) const { o << before[i]; }

  double getInit(int i) const { return before[i]; }

  void showVal(ostream &o, int i) const { o << after[i]; }

  double getVal(int i) const { return after[i]; }
};

ostream &operator<<(ostream &o, const PlanBit &p) {
  p.write(o);
  return o;
}

class PlanProcessor : public VisitController {
 private:
  void *vld;
  vector< string > store;
  map< double, list< PlanBit > > planbits;
  vector< PlanBit * > planStepStarts;
  vector< PlanBit * > planStepEnds;
  vector< int > codes;
  int x;

  vector< string > tags;

  double endTime;

  bool wantMetrics;
  int numMetrics;

 public:
  PlanProcessor(void *v) : vld(v), x(0), endTime(0), wantMetrics(false), numMetrics(0) {};

  virtual void visit_plan_step(plan_step *p) {
    // cout << "See a step " << p->op_sym->getName() << "\n";
    ++x;
    planbits[p->start_time].push_back(PlanBit(p, true, x, wantMetrics));
    planStepStarts.push_back(&planbits[p->start_time].back());
    endTime = p->start_time;

    if (p->duration_given) {
      planbits[p->start_time + p->duration].push_back(PlanBit(p, false, x, wantMetrics));
      planStepEnds.push_back(&planbits[p->start_time + p->duration].back());
      endTime = endTime > p->start_time + p->duration
                    ? endTime
                    : p->start_time + p->duration;
    } else {
      planStepEnds.push_back(0);
    }
  };

  void runPlan() {
    for (map< double, list< PlanBit > >::iterator i = planbits.begin();
         i != planbits.end(); ++i) {
      for (list< PlanBit >::iterator j = i->second.begin();
           j != i->second.end(); ++j) {
        j->handle(vld, codes);
      }
      if (timeSeries) {
        i->second.begin()->capturePre(vld, codes);
      }
      executeNext(vld);
      if (!timeSeries) {
        for (list< PlanBit >::iterator j = i->second.begin();
             j != i->second.end(); ++j) {
          j->capturePost(vld, codes);
        }
      } else {
        i->second.begin()->capturePost(vld, codes);
      }
    }
  }

  void add(char arg[]) {
    // First check for metric request:
    char metrics[] = "metrics";
    if(arg[0] == '$' && sizeof(arg) == 8)
    {
      for(int i = 0;i < 7;++i)
      {
        if(metrics[i] != arg[i+1]) 
        {
          
          return;
        }
      }
      wantMetrics = true;
      numMetrics = trackMetrics(vld);
      return;
    }
    // We need to find the function code...
    store.push_back(arg);
    char *tok = strtok(arg, " ");
    vector< char * > toks;
    while (tok != NULL) {
      toks.push_back(tok);
      tok = strtok(NULL, " ");
    }
    codes.push_back(getFunCode(vld, (const char **)toks.data()));
    // cout << "Acquired code " << codes[codes.size()-1] << "\n";
    if (timeSeries) {
      trackFE(vld, codes[codes.size() - 1]);
    }
  }

  void addTag(char arg[]) { tags.push_back(arg); }

  void write(ostream &o) const {
    int j = 0;
    for (vector< PlanBit * >::const_iterator i = planStepStarts.begin();
         i != planStepStarts.end(); ++i, ++j) {
      if (!(*i)->appearsIn(tags)) {
        (*i)->showValues(o);
        if (planStepEnds[j]) planStepEnds[j]->showValues(o);
        o << "\n";
      }
    }
  }

  void writeTS(ostream &o) const {
    int c = 0;
    double val;
    double val1;
    double t;
    double printt;
    for (vector< string >::const_iterator s = store.begin(); s != store.end();
         ++s, ++c) {
      o << *s << "\n";
      double t = 0;
      int n = 0;
      while (n >= 0) {
        val = getNextValue(vld, codes[c], t, n);
        if (t >= 0) {
          printt = t;
          o << t << ", " << val << "\n";
          val1 = val;
        }
      }
      if (printt < endTime) o << endTime << ", " << val1 << "\n";
    }
    if(wantMetrics)
    {
      int n = 0;
      for(unsigned int i = 0;i < numMetrics;++i)
      {
        n= 0;
        t = 0;
        o << "metric " << i << "\n";
        while(n >= 0)
        {
          val = getNextValue(vld,i,t,n);
          if (t >= 0) {
            printt = t;
            o << t << ", " << val << "\n";
            val1 = val;
          }
        }
      }
    }
  }
};

ostream &operator<<(ostream &o, const PlanProcessor &p) {
  if (!timeSeries)
    p.write(o);
  else
    p.writeTS(o);
  return o;
}

namespace VAL {
  extern bool Verbose;
}
int main(int argc, char *argv[]) {
  printTimes = false;
  timeSeries = false;
  if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 't') {
    --argc;
    ++argv;
    printTimes = true;
  }
  if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'T') {
    --argc;
    ++argv;
    timeSeries = true;
  }
  if (argc < 3) {
    cout
        << "Use: ValueSeq [-t|-T] <domain> <problem> <plan - in PDDL format> "
           "[function term]* [REMOVE [tag]*]\n\n\tFunction terms are "
           "(quote enclosed if need spaces) grounded expressions;\n\t\t$metrics is a special request for the metrics to be reported\n\t\tand is only useful with the -T option\n\t"
        << "Tags are strings that cause actions to not be reported if they appear in the action names\n\nOutput: Sequence of actions with values of all function terms before and, for durative actions, after execution\n\
            \t-t: Print start times of the actions in first column\n\
            \t\n-T: Generate time series data only for the function terms\n\n";

    return 0;
  }

  current_analysis = &an_analysis;
  ifstream plan(argv[3]);

  yfl = new yyFlexLexer(&plan, &cout);

  yydebug = 0;
  yyparse();
  delete yfl;
  parse_category *tt = top_thing;
  top_thing = 0;
  void *vld = makeValidatorFromFiles(argv[1], argv[2], 0.001);

  if (tt) {
    PlanProcessor p(vld);
    for (int i = 4; i < argc; ++i) {
      string s(argv[i]);
      if (s == "REMOVE") {
        for (++i; i < argc; ++i) {
          p.addTag(argv[i]);
        }

      }
      // cout << "Adding " << argv[i] << "\n";
      else
        p.add(argv[i]);
    }
    tt->visit(&p);
    p.runPlan();

    cout << p;
  }

  cleanUp(vld);

  return 0;
}

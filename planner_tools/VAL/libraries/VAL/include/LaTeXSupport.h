// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#ifndef __LATEXSUPPORT
#define __LATEXSUPPORT

#include "Utils.h"
#include "Validator.h"
#include "main.h"
#include <iostream>
#include <string>
#include <vector>

using std::ostream;
using std::string;
using std::vector;

namespace VAL {

  struct showList {
    void operator()(
        const pair< double, vector< pair< string, vector< double > > > > &ps)
        const {
      if (LaTeX) {
        string s;
        *report << ps.first << " \\>";
        for (vector< pair< string, vector< double > > >::const_iterator i =
                 ps.second.begin();
             i != ps.second.end(); ++i) {
          s = i->first;
          replaceSubStrings(s, "/", "/\\-");
          latexString(s);
          *report << "\\begin{minipage}[t]{12cm} " << s << " ";
          for (vector< double >::const_iterator j = i->second.begin();
               j != i->second.end(); ++j) {
            *report << *j << " ";
          };

          *report << " \\end{minipage}\\\\\n \\>";
        };
        *report << "\\\\\n";
      } else {
        cout << "\nValue: " << ps.first << "\n ";
        for (vector< pair< string, vector< double > > >::const_iterator i =
                 ps.second.begin();
             i != ps.second.end(); ++i) {
          cout << i->first << " ";
          copy(i->second.begin(), i->second.end(),
               ostream_iterator< double >(cout, " "));
          cout << "\n";
        };
      };
    };
  };

  void displayFailedLaTeXList(vector< string > &vs);

  class LaTeXSupport {
   private:
    int NoGraphPoints;
    int noPoints;
    int noGCPages;
    int noGCPageRows;
    vector< string > ganttObjectsAndTypes;
    vector< string > ganttObjects;

   public:
    LaTeXSupport() : NoGraphPoints(500), noGCPages(0), noGCPageRows(0){};
    void LaTeXHeader();
    void LaTeXPlanReportPrepare(char *);
    void LaTeXPlanReport(Validator *v, plan *);
    void LaTeXEnd();
    void LaTeXGantt(Validator *v);
    void LaTeXGraphs(Validator *v);
    void LaTeXDomainAndProblem();
    void LaTeXBuildGraph(ActiveCtsEffects *ace, const State *s);
    void setnoGCPages(int g) { noGCPages = g; };
    void setnoGCPageRows(int g) { noGCPageRows = g; };
    void setnoPoints(int n) {
      noPoints = n;
      if (noPoints < 10)
        noPoints = 10;
      else if (noPoints > 878)
        noPoints = 878;
      NoGraphPoints = noPoints;
    };
    void addGanttObject(char *c) { ganttObjectsAndTypes.push_back(c); };
  };

  extern LaTeXSupport latex;

};  // namespace VAL

#endif

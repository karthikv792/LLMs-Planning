// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#include "FlexLexer.h"
#include "ptree.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>

extern int yyparse();
extern int yydebug;

using std::ifstream;
using std::ofstream;
using std::stringstream;

namespace VAL {

  extern parse_category *top_thing;

  extern analysis an_analysis;
  extern analysis *current_analysis;

  extern yyFlexLexer *yfl;

};  // namespace VAL

extern char *current_filename;

#include "Utils.h"
#include "VisitController.h"

namespace VAL {

  extern bool Verbose;

  class DomainWriter : public VisitController {
   private:
    stringstream record;
    time_spec filter;

   public:
    virtual ~DomainWriter(){};

    virtual void visit_symbol(symbol *){};
    virtual void visit_control_symbol(control_symbol *){};
    virtual void visit_pred_symbol(pred_symbol *){};
    virtual void visit_func_symbol(func_symbol *){};
    virtual void visit_const_symbol(const_symbol *){};

    virtual void visit_var_symbol(var_symbol *){};
    virtual void visit_pddl_typed_symbol(pddl_typed_symbol *){};
    virtual void visit_plus_expression(plus_expression *){};
    virtual void visit_minus_expression(minus_expression *){};
    virtual void visit_mul_expression(mul_expression *){};
    virtual void visit_div_expression(div_expression *){};
    virtual void visit_uminus_expression(uminus_expression *){};
    virtual void visit_int_expression(int_expression *){};
    virtual void visit_float_expression(float_expression *){};
    virtual void visit_special_val_expr(special_val_expr *){};
    virtual void visit_func_term(func_term *){};
    virtual void visit_assignment(assignment *){};

    virtual void visit_simple_goal(simple_goal *s) {
      if (record.str() == "") {
        record << "\\vspace{1mm}\\begin{compactitem}\n";
      }
      record << "\\item ";
      if (s->getPolarity() != E_POS) {
        record << "{\\em not} ";
      }
      string t(s->getProp()->head->getName());
      latexString(t);
      record << t;
    };
    virtual void visit_qfied_goal(qfied_goal *){};
    virtual void visit_conj_goal(conj_goal *c) { c->getGoals()->visit(this); };
    virtual void visit_disj_goal(disj_goal *){};
    virtual void visit_timed_goal(timed_goal *t) {
      if (t->getTime() == filter) {
        t->getGoal()->visit(this);
      }
    };
    virtual void visit_imply_goal(imply_goal *){};
    virtual void visit_neg_goal(neg_goal *){};
    virtual void visit_comparison(comparison *){};
    virtual void visit_proposition(proposition *){};

    virtual void visit_simple_effect(simple_effect *){};
    virtual void visit_forall_effect(forall_effect *){};
    virtual void visit_cond_effect(cond_effect *){};
    virtual void visit_effect_lists(effect_lists *){};

    virtual void visit_action(action *){};

    void setFilter(time_spec v) {
      record.str(string());
      filter = v;
    }

    virtual void visit_durative_action(durative_action *d) {
      cout << "\\begin{tabular}{|>{\\columncolor{blue!25}}lc>{\\columncolor{"
              "pink!"
              "25}}l|}\n\\hline\n{\\textsc{At Start Conditions}} & "
              "\\multicolumn{1}{c|}{\\cellcolor{blue!25} $\\longleftarrow$ "
              "\\textsc{Invariants} $\\longrightarrow$} & {\\textsc{Effects on "
              "Completion}} \\\\ \n";

      cout << " & \\multicolumn{1}{c|}{\\cellcolor{blue!25} "
              "\\begin{minipage}{0.3\\textwidth}{";
      // cout << "Invariants in here";
      setFilter(E_OVER_ALL);

      d->precondition->visit(this);
      if (record.str() != "") record << "\\end{compactitem}\n\\vspace{1mm}";
      cout << record.str();
      cout << "}\\end{minipage}} & "
              "\\\\\\hhline{>{\\arrayrulecolor{blue!25}}->{\\arrayrulecolor{"
              "black}"
              "}->{\\arrayrulecolor{pink!25}}->{\\arrayrulecolor{black}}}\n";
      string s(d->name->getName());
      latexString(s);
      stringstream ss;
      ss << "{\\bf " << s << "}";
      if (!d->parameters->empty()) {
        ss << "\\\\\n\\begin{centering}{";
        for (var_symbol_list::const_iterator i = d->parameters->begin();
             i != d->parameters->end();) {
          string t((*i)->getName());
          latexString(t);
          ss << "{\\em " << t << "} -- ";
          string t1((*i)->type->getName());
          latexString(t1);
          ss << t1;
          ++i;
          ss << "\\\\\n";
        }
        ss << "}\\end{centering}";
      }
      // cout << " &
      // \\multicolumn{1}{|c|}{\\begin{tcolorbox}[width=0.3\\textwidth]" << s <<
      // "\\end{tcolorbox}} & \\\\\\cline{2-2}\n";
      cout << " & "
              "\\multicolumn{1}{|c|}{\\cellcolor{orange!25}\\begin{minipage}{0."
              "3\\textwidth}{\\vspace{1mm}"
           << ss.str()
           << "\\vspace{1mm}}\\end{minipage}} & "
              "\\\\\\hhline{>{\\arrayrulecolor{blue!25}}->{\\arrayrulecolor{"
              "black}"
              "}->{\\arrayrulecolor{pink!25}}->{\\arrayrulecolor{black}}}\n";
      cout << "\\multirow{-3}{*}{\\begin{minipage}{0.3\\textwidth} {";
      setFilter(E_AT_START);
      d->precondition->visit(this);
      if (record.str() != "") record << "\\end{compactitem}\n\\vspace{1mm}";
      cout << record.str();
      cout
          << "}\\end{minipage}} & \\multicolumn{1}{|l}{\\cellcolor{pink!25} "
             "\\textsc{Initial Effects}} & "
             "\\multirow{-3}{*}{\\begin{minipage}{0.3\\textwidth} {End effects "
             "in here}\\end{minipage}}\\\\\n";
      cout << "\\hline \n\\end{tabular}\n\n";
    };

    virtual void visit_domain(domain *p) {
      cout << "\\documentclass{article}\n\\usepackage{paralist}\n\\usepackage{"
              "multirow}\n\\usepackage{a4wide}\n\\usepackage{hhline}"
              "\n\\usepackage[table]{xcolor}\n\\begin{document}\n";
      visit_operator_list(p->ops);
      cout << "\\end{document}\n";
    };
  };

};  // namespace VAL

using namespace VAL;

/**
 * This expects any number of filenames as arguments, although
 * it probably doesn't ever make sense to supply more than two.
 */
int main(int argc, char *argv[]) {
  current_analysis = &an_analysis;
  ifstream *current_in_stream;
  yydebug = 0;  // Set to 1 to output yacc trace
  Verbose = true;
  yfl = new yyFlexLexer;

  // Loop over given args
  for (int a = 1; a < argc; ++a) {
    current_filename = argv[a];
    // cout << "File: " << current_filename << '\n';
    current_in_stream = new ifstream(current_filename);
    if (current_in_stream->bad()) {
      // Output a message now
      cout << "Failed to open\n";

      // Log an error to be reported in summary later
      line_no = 0;
      log_error(E_FATAL, "Failed to open file");
    } else {
      line_no = 1;

      // Switch the tokeniser to the current input stream
      yfl->switch_streams(current_in_stream, &cout);
      yyparse();

      // Output syntax tree
      DomainWriter dw;

      if (top_thing) top_thing->visit(&dw);
    }
    delete current_in_stream;
  }
  // Output the errors from all input files
  // current_analysis->error_list.report();
  delete yfl;
}

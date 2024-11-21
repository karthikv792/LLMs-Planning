// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#include "TIM.h"
#include "FastEnvironment.h"
#include "FlexLexer.h"
#include "TimSupport.h"
#include "TypedAnalyser.h"
#include "ptree.h"
#include <cstdio>
#include <fstream>
#include <iostream>

extern int yyparse();
extern int yydebug;

using std::cerr;
using std::ifstream;
using std::istream;
using std::ofstream;
using std::ostream;

namespace VAL {

  extern bool Verbose;
  extern ostream *report;
  extern parse_category *top_thing;

  extern analysis *current_analysis;

  extern yyFlexLexer *yfl;
  extern TypeChecker *theTC;
  extern bool FAverbose;

  int PropInfo::x = 0;

};  // namespace VAL

extern char *current_filename;
using namespace VAL;

namespace TIM {

  TIMAnalyser *TA;

  void TIMstage1(char *argv[]) {
    current_analysis = new analysis;
    std::shared_ptr<IDopTabFactory> fac = std::make_shared<IDopTabFactory>();
    current_analysis->setFactory(fac);
    current_analysis->pred_tab.replaceFactory< holding_pred_symbol >();
    current_analysis->func_tab.replaceFactory< extended_func_symbol >();
    current_analysis->const_tab.replaceFactory< TIMobjectSymbol >();
    current_analysis->op_tab.replaceFactory< TIMactionSymbol >();
    current_analysis->setFactory(std::make_shared<TIMfactory>());
    Associater::buildEPS =
        std::make_unique< specEPSBuilder< TIMpredSymbol > >();

    istream *current_in_stream;
    yydebug = 0;  // Set to 1 to output yacc trace

    yfl = new yyFlexLexer;

    // Loop over given args

    for (int i = 0; i < 2; ++i) {
      bool realfile = false;
      if (argv[i][0] == '-') {
        current_filename = (char *)"stdin";
        // cout << "Here we are....\n";
        current_in_stream = &std::cin;
        realfile = false;
      } else {
        current_filename = argv[i];
        // cout << "File: " << current_filename << '\n';
        current_in_stream = new ifstream(current_filename);
        realfile = true;
      }
      if (current_in_stream->bad()) {
        // Output a message now
        cerr << "Failed to open ";
        if (i) {
          cerr << "problem";
        } else {
          cerr << "domain";
        }
        cerr << " file " << current_filename << "\n";
        exit(0);
        // Log an error to be reported in summary later
        line_no = 0;
        log_error(E_FATAL, "Failed to open file");
      } else {
        line_no = 1;

        // Switch the tokeniser to the current input stream
        yfl->switch_streams(current_in_stream, &cout);
        yyparse();

        // Output syntax tree
        // if (top_thing) top_thing->display(0);
      }
      if (realfile) {
        delete current_in_stream;
      }
    }
    // Output the errors from all input files
    if (current_analysis->error_list.errors) {
      cerr << "Critical Errors Encountered in Domain/Problem File\n";
      cerr << "--------------------------------------------------\n\n";
      cerr << "Due to critical errors in the supplied domain/problem file, the "
              "planner\n";
      cerr << "has to terminate.  The errors encountered are as follows:\n";
      current_analysis->error_list.report();
      exit(0);
    } else if (current_analysis->error_list.warnings) {
      cout << "Warnings encountered when parsing Domain/Problem File\n";
      cerr << "-----------------------------------------------------\n\n";
      cerr << "The supplied domain/problem file appear to violate part of the "
              "PDDL\n";
      cerr << "language specification.  Specifically:\n";
      current_analysis->error_list.report();
      cerr << "\nThe planner will continue, but you may wish to fix your files "
              "accordingly\n";
    }

    delete yfl;
  }

  void TIMstage2() {
    DurativeActionPredicateBuilder dapb;
    current_analysis->the_domain->visit(&dapb);

    theTC = new TypeChecker(current_analysis);
    if (!theTC->typecheckDomain()) {
      cerr << "Type Errors Encountered in Domain File\n";
      cerr << "--------------------------------------\n\n";
      cerr << "Due to type errors in the supplied domain file, the planner\n";
      cerr << "has to terminate.  The log of type checking is as follows:\n\n";
      Verbose = true;
      theTC->typecheckDomain();
      exit(0);
    }
    if (!theTC->typecheckProblem()) {
      cerr << "Type Errors Encountered in Problem File\n";
      cerr << "---------------------------------------\n\n";
      cerr << "Due to type errors in the supplied problem file, the planner\n";
      cerr << "has to terminate.  The log of type checking is as follows:\n\n";
      Verbose = true;
      theTC->typecheckProblem();
      exit(0);
    }
    TypePredSubstituter a;
    current_analysis->the_problem->visit(&a);
    current_analysis->the_domain->visit(&a);

    Analyser aa(dapb.getIgnores());
    current_analysis->the_problem->visit(&aa);
    current_analysis->the_domain->visit(&aa);

    //    current_analysis->the_domain->predicates->visit(&aa);

    if (FAverbose && current_analysis->the_domain->functions)
      current_analysis->the_domain->functions->visit(&aa);
    TA = new TIMAnalyser(*theTC, current_analysis);
    current_analysis->the_domain->visit(TA);
    current_analysis->the_problem->visit(TA);
    for_each(current_analysis->the_domain->ops->begin(),
             current_analysis->the_domain->ops->end(), completeMutexes);
    TA->checkSV();
    dapb.reverse();
    current_analysis->the_domain->visit(&dapb);
    for (vector< durative_action * >::iterator i = aa.getFixedDAs().begin();
         i != aa.getFixedDAs().end(); ++i) {
      (static_cast< TIMactionSymbol * >((*i)->name))->assertFixedDuration();
    };
  }

  void performTIMAnalysis(char *argv[]) {
    TIMstage1(argv);
    TIMstage2();
  }
};  // namespace TIM

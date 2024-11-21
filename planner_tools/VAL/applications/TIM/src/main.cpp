// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#include "TIM.h"
#include "ptree.h"
#include <iostream>

#include "SimpleEval.h"
#include "instantiation.h"

using namespace TIM;
using namespace VAL;
using namespace Inst;

int main(int argc, char *argv[]) {
  performTIMAnalysis(&argv[1]);

  for_each(TA->pbegin(), TA->pend(), ptrwriter< PropertySpace >(cout, "\n"));
  for_each(TA->abegin(), TA->aend(), ptrwriter< PropertySpace >(cout, "\n"));
  /*
          SimpleEvaluator::setInitialState();
      for(operator_list::const_iterator os =
     current_analysis->the_domain->ops->begin(); os !=
     current_analysis->the_domain->ops->end();++os)
      {
          cout << (*os)->name->getName() << "\n";
          instantiatedOp::instantiate(*os,current_analysis->the_problem,*theTC);
          cout << instantiatedOp::howMany() << " so far\n";
      };
      cout << instantiatedOp::howMany() << "\n";
      instantiatedOp::writeAll(cout);

          cout << "\nList of all literals:\n";
      instantiatedOp::createAllLiterals(current_analysis->the_problem);
          instantiatedOp::writeAllLiterals(cout);

           const pred_symbol * p =
     *(dynamic_cast<holding_pred_symbol*>(current_analysis->pred_tab.symbol_probe("at"))->pBegin());
       set<Literal *> ls1 = instantiatedOp::allLiterals(p);
          const pred_symbol * p1 =
     *(dynamic_cast<holding_pred_symbol*>(current_analysis->pred_tab.symbol_probe("near"))->pBegin());
       set<Literal *> ls2 = instantiatedOp::allLiterals(p1);


          for(set<Literal*>::iterator i = ls1.begin();i != ls1.end();++i)
          {
                  for(set<Literal*>::iterator j = ls2.begin();j !=
     ls2.end();++j)
                  {
                          cout << "Checking " << **i << " and " << **j << "..."
                           << isMutex((*i)->getHead(),(*i)->begin(),(*i)->end(),
                                                          (*j)->getHead(),(*j)->begin(),(*j)->end())
                                  << "\n";
                  };
          };

      for_each(current_analysis->the_domain->ops->begin(),
                          current_analysis->the_domain->ops->end(),showMutex);

      VAL::operator_* A = *(current_analysis->the_domain->ops->begin());
      VAL::operator_* B = *(++current_analysis->the_domain->ops->begin());
      vector<VAL::const_symbol*> argsA, argsB;
      for(var_symbol_list::iterator i = A->parameters->begin();!(i ==
     A->parameters->end());++i)
      {
          argsA.push_back(*(theTC->range(*i).begin()));
      };
      for(var_symbol_list::iterator i = B->parameters->begin();!(i ==
     B->parameters->end());++i)
      {
          argsB.push_back(*(theTC->range(*i).begin()));
      };
          for_each(argsA.begin(),argsA.end(),ptrwriter<VAL::const_symbol>(cout,"
     ")); cout << "\n";
          for_each(argsB.begin(),argsB.end(),ptrwriter<VAL::const_symbol>(cout,"
     ")); cout << "\n";

          cout << "Mutexes for " << A->name->getName() << " and " <<
     B->name->getName() << "\n" << getMutexes(A,argsA.begin(),argsA.end(),
                                          B,argsB.begin(),argsB.end()) << "\n";



          string args[] = {"truck0","depot0","distributor0"};

          typedef string * strP;

          cout <<
     *(instantiatedOp::getInstOp(string("drive"),Iterator<const_symbol *
     const,strP,&getConst>(args), Iterator<const_symbol *
     const,strP,&getConst>(args+3))) << "\n";
  */
  /*
          domain* the_domain = current_analysis->the_domain;
       the_domain->predicates->write(std::cout);

       std::cout << "Print the predicate vars" << std::endl;

       pred_decl_list* predicates = the_domain->predicates;
       for (pred_decl_list::const_iterator ci = predicates->begin(); ci !=
                predicates->end(); ci++)
       {
           holding_pred_symbol * hps = HPS((*ci)->getPred());
           for(holding_pred_symbol::PIt i = hps->pBegin();i != hps->pEnd();++i)
           {
                  TIMpredSymbol * tps = cTPS(*i);
           tps -> write(std::cout);
           std::cout << "\nIs definitely static " <<
              tps->isDefinitelyStatic() << tps->isStatic() << std::endl;
           std::cout << ", ";
           }
       }
       */
}

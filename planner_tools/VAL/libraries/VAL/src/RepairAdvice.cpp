// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#include "RepairAdvice.h"
#include "Utils.h"
#include "Validator.h"

namespace VAL {

  std::unique_ptr< UnsatConditionFactory > ErrorLog::fac(std::make_unique<UnsatConditionFactory>());

  string UnsatCondition::getAdviceString() const {
    string ans;
    ostringstream aStringStream;
    ostream *oldReport = report;
    report = &aStringStream;
    advice();

    ans = aStringStream.str();

    report = oldReport;
    return ans;
  };

  void UnsatPrecondition::display() const {
    *report << *action << " has an unsatisfied precondition at time " << time;
    if (LaTeX) *report << "\\\\";
    *report << "\n";
  };

  string UnsatPrecondition::getDisplayString() const {
    string ans = "The precondition is unsatisfied";
    return ans;
  };

  void UnsatPrecondition::advice() const {
    if (LaTeX)
      *report << "\\item ";
    else
      *report << "\n";

    display();

    action->displayDurationAdvice(&state);

    if (ap->isAdvice()) {
      if (LaTeX)
        ap->displayLaTeX();
      else
        ap->display();
    }
  };

  void UnsatDurationCondition::display() const {
    *report << *action << " has an unsatisfied duration constraint at time "
            << time;
    if (LaTeX) *report << "\\\\";
    *report << "\n";
  };

  string UnsatDurationCondition::getDisplayString() const {
    string ans = "The duration constraint is unsatisfied";
    return ans;
  };

  void UnsatDurationCondition::advice() const {
    if (LaTeX)
      *report << "\\item ";
    else
      *report << "\n";
    display();

    *report << "Change the duration by at least " << error << "!";
    if (LaTeX) *report << "\\\\";
    *report << "\n";
  };

  void MutexViolation::display() const {
    //*report << *action1 << " and "<<action2<<" are mutex at time "<< time;
    *report << *action1 << " has a mutex violation at time " << time;
    if (action2 != 0) *report << " with " + action2->getName();
    if (LaTeX) *report << "\\\\";
    *report << "\n";
  };

  string MutexViolation::getDisplayString() const {
    string ans = action1->getName() + " has a mutex violation";
    if (action2 != 0) ans += " with " + action2->getName();
    return ans;
  };

  void MutexViolation::advice() const {
    if (LaTeX)
      *report << "\\item ";
    else
      *report << "\n";
    display();

    *report << "Separate these actions!";
    if (LaTeX) *report << "\\\\";
    *report << "\n";
  };

  void UnsatGoal::display() const {
    *report << "The goal is not satisfied";
    if (LaTeX) *report << "\\\\";
    *report << "\n";
  };

  string UnsatGoal::getDisplayString() const {
    string ans = "The goal is not satisfied";
    return ans;
  };

  void UnsatGoal::advice() const {
    if (LaTeX)
      *report << "\\item ";
    else
      *report << "\n";
    display();

    if (LaTeX)
      ap->displayLaTeX();
    else
      ap->display();
  };

  void UnsatInvariant::display() const {
    if (LaTeX) *report << "\\item ";
    *report << *action << " has its condition unsatisfied between time "
            << startTime << " to " << endTime << ", ";

    if (!rootError) {
      *report << "the condition is satisfied on ";
      satisfiedOn.writeOffset(startTime);
      *report << ". ";
    } else {
      *report << "Sorry there were problems computing the intervals!";
    };

    // if(LaTeX) *report << " For each $t$ in $( 0, "<<endTime-startTime<<")$
    // follow:\\\\"; else *report << " For each t in ( 0,
    // "<<endTime-startTime<<")
    // follow:\n";
    if (!LaTeX) *report << "\n";
  };

  string UnsatInvariant::getDisplayString() const {
    string ans = "The invariant condition is unsatisfied";

    return ans;
  };

  void UnsatInvariant::advice() const {
    if (!LaTeX) *report << "\n";
    display();

    if (ap->isAdvice()) {
      if (LaTeX)
        ap->displayLaTeX();
      else
        ap->display();
    }
  };

  void ErrorLog::displayReport() const {
    if (conditions.size() == 0) return;
    if (LaTeX)
      *report << "\\subsection{Error Report}\n";
    else
      *report << "\nError Report:\n";
    for (vector< const UnsatCondition * >::const_iterator i =
             conditions.begin();
         i != conditions.end(); ++i) {
      (*i)->display();
    };
  };

  ErrorLog::~ErrorLog() {
    for (vector< const UnsatCondition * >::const_iterator i =
             conditions.begin();
         i != conditions.end(); ++i) {
      delete (*i);
    };
  };

  void ErrorLog::addPrecondition(double t, const Action *a, const State *s) {
    const UnsatCondition *pre = fac->buildUnsatPrecondition(t, a, s);
    conditions.push_back(pre);
  };

  void ErrorLog::addUnsatDurationCondition(double t, const Action *a,
                                           const State *s, double e) {
    const UnsatCondition *pre = fac->buildUnsatDurationCondition(t, a, s, e);
    conditions.push_back(pre);
  };

  void ErrorLog::addMutexViolation(double t, const Action *a1, const Action *a2,
                                   const State *s) {
    const UnsatCondition *pre = fac->buildMutexViolation(t, a1, a2, s);
    conditions.push_back(pre);
  };

  void ErrorLog::addGoal(const Proposition *p, const State *s) {
    const UnsatCondition *pre = fac->buildUnsatGoal(p, s);
    conditions.push_back(pre);
  };

  void ErrorLog::addUnsatInvariant(double st, double e, Intervals ints,
                                   const Action *a, const State *s,
                                   bool rootError) {
    const UnsatInvariant *inv =
        fac->buildUnsatInvariant(st, e, ints, a, s, rootError);
    conditions.push_back(inv);
  };

};  // namespace VAL

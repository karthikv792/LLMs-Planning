#ifndef __VALLIB
#define __VALLIB

#include "valLibExport.h"

#ifdef __cplusplus

extern "C" {
#endif  // __cplusplus

// Time line is maintained relative to plan start time, so translation to
// absolute time is required in both directions. Could supply utilities to
// manage this...

// Execute next happening on time line
VAL_EXPORT bool executeNext(void *validator);

// Execute all happenings on time line until time "when"
VAL_EXPORT bool executeTo(void *validator, double when);

// Build the initial validation object - relies on having the analysis object
// already...
VAL_EXPORT void *makeValidator(void *an, double tol, void *typeChecker);
// Another way to build the validator
VAL_EXPORT void *makeValidatorFromFiles(LPCSTR domain, LPCSTR problem,
                                        double tol);

// post allows an action to be added to the plan time line (at when), start or
// end (starting flag) and with action name and parameters and action ID (the
// action ID is chosen by the user, but must be unique for each action inserted
// into the plan. The ID is used to identify the same action when it is ending,
// so that when ending an action it is not necessary to give the string array
// (it will not be examined).
VAL_EXPORT bool post(void *validator, int aID, LPCSTR action[], bool starting,
                     double when);

// Request values of variables (boolean/numeric) - not suitable for library
// version
// bool queryLiteral(const void * validator,const void * literm,const void *
// fenv); double queryValue(const void * validator,const void * fterm,const void
// * fenv); double queryPriorValue(const void * validator,const void *
// fterm,const void * fenv);

// Manage variables via codes...
VAL_EXPORT unsigned long getLitCode(const void *validator, LPCSTR lit[]);
VAL_EXPORT unsigned long getFunCode(const void *validator, LPCSTR pne[]);

// Discover the strings for a code
VAL_EXPORT LPCSTR *whatLit(const void *validator, unsigned long code, int &n);
VAL_EXPORT LPCSTR *whatFun(const void *validator, unsigned long code, int &n);

// Arrays allocated in heap - caller takes responsibility for clean up
// (currently) Since the allocation is a C++ new, there is a suitable C function
// to call clean up
VAL_EXPORT unsigned long *getChangedLits(const void *validator, int &n);
VAL_EXPORT unsigned long *getChangedFuns(const void *validator, int &n);
VAL_EXPORT void resetDelta(const void *validator);

VAL_EXPORT void cleanStore(unsigned long *v);
VAL_EXPORT void cleanName(LPCSTR *nm);
VAL_EXPORT void cleanState(LPCSTR nm);
VAL_EXPORT void cleanStateDelta(LPCSTR *fs, int n);

// Query values via strings or codes
VAL_EXPORT bool queryLiteralNamed(const void *validator, LPCSTR lit[]);
VAL_EXPORT double queryValueNamed(const void *validator, LPCSTR fun[]);
VAL_EXPORT double queryPriorValueNamed(const void *validator, LPCSTR fun[]);
VAL_EXPORT bool queryLiteralCode(const void *validator, unsigned long code);
VAL_EXPORT double queryValueCode(const void *validator, unsigned long code);
VAL_EXPORT double queryPriorValueCode(const void *validator,
                                      unsigned long code);

VAL_EXPORT void cleanUp(void *validator);

// Do we really need string versions of these? I hope not!
VAL_EXPORT void assertLitValue(void *validator, unsigned long code, bool val);
VAL_EXPORT void assertFunValue(void *validator, unsigned long code, double val);

VAL_EXPORT LPCSTR *finalState(LPCSTR domainContent, LPCSTR problemContent,
                              LPCSTR *actions, int *aID, int *startOrEnd,
                              double *times, int nEntries, int &nState);

VAL_EXPORT LPCSTR getState(void *v);
VAL_EXPORT void trackFE(void *v, unsigned long cd);
VAL_EXPORT int trackMetrics(void * v);
VAL_EXPORT double getNextValue(void *v, unsigned long cd, double &t, int &n);

/*

Also need a function to write state... to file? to string? as a data structure?
JSON document
*/

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif

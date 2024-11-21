// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#include "instantiation.h"
#include "DebugWriteController.h"
#include "Exceptions.h"
#include "SimpleEval.h"
#include "TIM.h"
#include "main.h"
#include "ptree.h"
#include "typecheck.h"
#include <FlexLexer.h>
#include <assert.h>
#include <cstdio>
#include <fstream>
#include <iostream>

using std::cerr;
using std::endl;
using std::ifstream;

using namespace VAL;

namespace Inst {

  set< instantiatedDrvUtils::index, instantiatedDrvUtils::indexLT >
      instantiatedDrvUtils::definitive;
  bool instantiatedDrvUtils::initDefinitive = false;

  instantiatedDrvUtils::index *instantiatedDrvUtils::purify(const index *i) {
    if (!initDefinitive) {
      for (derivations_list::const_iterator os =
               current_analysis->the_domain->drvs->begin();
           os != current_analysis->the_domain->drvs->end(); ++os) {
        index newIndex((*os)->get_head()->head, (*os));
        definitive.insert(newIndex);
      };
      initDefinitive = true;
    };
    set< index, indexLT >::iterator defItr = definitive.find(*i);
    return const_cast< index * >(&(*(defItr)));
  };

  bool varFree(const VAL::parameter_symbol_list *pl) {
    for (VAL::parameter_symbol_list::const_iterator i = pl->begin();
         i != pl->end(); ++i) {
      if (!dynamic_cast< const VAL::const_symbol * >(*i)) return false;
    };
    return true;
  }

  ostream &operator<<(ostream &o, const instantiatedOp &io) {
    io.write(o);
    return o;
  };

  ostream &operator<<(ostream &o, const instantiatedDrv &io) {
    io.write(o);
    return o;
  };

  ostream &operator<<(ostream &o, const PNE &io) {
    io.write(o);
    return o;
  };

  ostream &operator<<(ostream &o, const Literal &io) {
    io.write(o);
    return o;
  };

  void instantiatedOp::writeAll(ostream &o) { instOps.write(o); };

  void instantiatedDrv::writeAll(ostream &o) { instDrvs.write(o); };

  void LitStoreEvaluator::evaluateSimpleGoal(FastEnvironment *f,
                                             simple_goal *s) {
    const bool esgDebug = false;

    extended_pred_symbol *eps = EPS(s->getProp()->head);

    holding_pred_symbol *const hps = eps->getParent();

    bool hpsIsStatic = true;

    {
      holding_pred_symbol::PIt epsItr = hps->pBegin();
      const holding_pred_symbol::PIt epsEnd = hps->pEnd();

      for (; epsItr != epsEnd; ++epsItr) {
        if (!(*epsItr)->appearsStatic()) {
          hpsIsStatic = false;
          break;
        }
      }
    }

    // if (eps->appearsStatic() && eps->isCompletelyStatic(f, s->getProp())) {
    if (hpsIsStatic) {
      if (esgDebug) {
        Literal l(s->getProp(), f);
        cout << "\t\tQuerying static fact " << l << "\n";
      }
      unknownTrue = false;
      unknownFalse = false;

      bool epsMatch = false;

      holding_pred_symbol::PIt epsItr = hps->pBegin();
      const holding_pred_symbol::PIt epsEnd = hps->pEnd();

      for (; epsItr != epsEnd; ++epsItr) {
        if ((*epsItr)->contains(f, s->getProp())) {
          if (SimpleEvaluator::verbose)
            cout << s->getProp()->head->getName()
                 << " is static, and in the EPS " << *epsItr << std::endl;
          valueTrue = true;
          valueFalse = false;
          epsMatch = true;
          break;
        }
      }

      if (!epsMatch) {
        valueTrue =
            (InitialStateEvaluator::init0State.find(s->getProp()->head) !=
             InitialStateEvaluator::init0State.end());
        valueFalse = !valueTrue;
        if (SimpleEvaluator::verbose)
          cout << s->getProp()->head->getName()
               << " is static but not in the arity 0 initial state\n";
      }

      /*
                      if (eps->contains(f,s->getProp())) {
                              valueTrue = true;
                              valueFalse = false;
                      }
                      else
                      {
                              valueTrue =
         (InitialStateEvaluator::init0State.find(s->getProp()->head) !=
         InitialStateEvaluator::init0State.end()); valueFalse = !valueTrue;
                      }
      */
      if (s->getPolarity() == E_NEG) {
        const bool vt = valueTrue;
        valueTrue = valueFalse;
        valueFalse = vt;
      };

      return;
    }

    if (s->getPolarity() == E_NEG) {
      if (esgDebug) {
        Literal l(s->getProp(), f);
        cout << "\t\tQuerying negative fact " << l << "\n";
      }

      Literal l(s->getProp(), f);
      Literal *l1 = instantiatedOp::findLiteral(&l);
      if (l1) {
        valueTrue = false;
        unknownTrue = true;
        valueFalse = false;
        unknownFalse = true;
      } else {
        valueFalse = false;
        unknownFalse = false;
        valueTrue = true;
        unknownTrue = false;
      }

    } else {
      if (esgDebug) {
        Literal l(s->getProp(), f);
        cout << "\t\tQuerying fact " << l << "\n";
      }

      Literal l(s->getProp(), f);
      Literal *l1 = instantiatedOp::findLiteral(&l);
      if (l1) {
        valueTrue = false;
        unknownTrue = true;
        valueFalse = false;
        unknownFalse = true;
      } else {
        valueTrue = false;
        unknownTrue = false;
        valueFalse = true;
        unknownFalse = false;
      }
    }
  };

  OpStore instantiatedOp::instOps;
  DrvStore instantiatedDrv::instDrvs;

  map< VAL::pddl_type *, vector< VAL::const_symbol * > > instantiatedValues;

  map< VAL::pddl_type *, vector< VAL::const_symbol * > >
      &instantiatedOp::values = instantiatedValues;
  map< VAL::pddl_type *, vector< VAL::const_symbol * > >
      &instantiatedDrv::values = instantiatedValues;

#ifndef NDEBUG
  const VAL::operator_ *instantiatedOp::insistOnOp = 0;
  vector< VAL::const_symbol * > instantiatedOp::insistOnOpParameters;
  const instantiatedOp *instantiatedOp::opBeforeFiltering = 0;
#endif

  void instantiatedOp::filterOps(VAL::TypeChecker *const tc) {
    //        cout << "*** Filtering Operators ***\n";
    int offset = 0;
    for (OpStore::iterator i = opsBegin(); !(i == opsEnd()); ++i) {
      SimpleEvaluator se(tc, (*i)->getEnv(), LSE(literals));
#ifndef NDEBUG
      if (*i == opBeforeFiltering) {
        SimpleEvaluator::verbose = true;
      } else {
        SimpleEvaluator::verbose = false;
      }
#endif
      (*i)->forOp()->visit(&se);
      if (se.reallyFalse()) {
        //			cout << "Kill " << *(*i) << "\n";
        opErase(*i);
        ++offset;
      } else {
        (*i)->setID((*i)->getID() - offset);
      };
    }
    instOps.clearUp();
  };

  void instantiatedDrv::filterDrvs(VAL::TypeChecker *const tc) {
    int offset = 0;
    for (DrvStore::iterator i = drvsBegin(); !(i == drvsEnd()); ++i) {
      SimpleEvaluator se(tc, (*i)->getEnv(), LSE(literals));
      (*i)->forDrv()->visit(&se);
      if (se.reallyFalse()) {
        //			cout << "Kill " << *(*i) << "\n";
        drvErase(*i);
        ++offset;
      } else {
        (*i)->setID((*i)->getID() - offset);
      };
    }
    instDrvs.clearUp();
  };

  /**
   *  A class to represent the joint parameter domain constraints (JPDCs) from a
   * single parameter of an action.  The JPDCs are constructed and manipulated
   * by other classes - the main purpose of this class is to make sure that the
   * JPDCs are well-formed, i.e. that no restriction is made from a parameter to
   * its own value.
   *  @see ParameterDomainsAndConstraints
   */
  class JPDCData {
    /**
     *  A record of the JPDCs from one parameter of an operator.  Each entry in
     * the map is a pair consisting of:
     *  - an int, representing an entity/constant that could be used to the
     *    current parameter of the operator
     *  - a map, detailing permissible values for (a subset of) the other
     * operator parameters if this parameter is assigned this value.  Each entry
     * of this map is a pair consisting of:
     *     - the index of another operator parameter (where parameter 0 is the
     * first parameter)
     *     - a set of values that can be given to that parameter
     *
     *  Defined thus, jpdcs[a][b], if defined, is the set of values that can be
     * given to parameter <code>b</code> if this parameter is assigned the value
     * <code>a</code>.
     */
    map< int, map< int, set< int > > > jpdcs;

   public:
    JPDCData() {}

    /**
     *  An iterator through the map of JPDCs stored for the parameter
     * corresponding to this object.  Each entry in the map is a pair consisting
     * of:
     *  - an int, representing an entity/constant that could be used to the
     *    current parameter of the operator
     *  - a map, detailing permissible values for (a subset of) the other
     * operator parameters if this parameter is assigned this value.  Each entry
     * of this map is a pair consisting of:
     *     - the index of another operator parameter (where parameter 0 is the
     * first parameter)
     *     - a set of values that can be given to that parameter
     */
    typedef map< int, map< int, set< int > > >::iterator iterator;

    /** Const version of the <code>iterator</code> for this class. */
    typedef map< int, map< int, set< int > > >::const_iterator const_iterator;

    /** @Return <code>true</code> if no JPDCs are defined from this object (i.e.
     * from its corresponding parameter). */
    inline bool empty() const { return jpdcs.empty(); }

    /**
     * @Return An iterator pointing to the beginning of the collection of JPDCs
     * for the parameter corresponding to this object.
     * @see jpdcs
     */
    inline const_iterator begin() const { return jpdcs.begin(); }

    /**
     * @Return An iterator pointing to the end of the collection of JPDCs for
     * the parameter corresponding to this object.
     * @see jpdcs
     */
    inline const_iterator end() const { return jpdcs.end(); }

    inline iterator beginw() { return jpdcs.begin(); }

    inline iterator endw() { return jpdcs.end(); }

    /**
     *  Record a JPDC from a value of the parameter corresponding to this object
     * (assuming no prior JPDCs from that value are defined).
     *
     *  @param paramID  The ID of the parameter corresponding to this object.
     * This is only given to force the JPDCs to be well-formed: a parameter may
     * not enforce restrictions upon its own value.
     *  @param oItr     An iterator pointing to somewhere in the jpdcs for this
     * object (or the end). As this class uses an STL map, insertion performance
     * is improved the closer this iterator is to where the JPDC will need to be
     * recorded
     *  @param d        The new JPDC to record, a pair consisting of:
     *                  - a value that can be given to the parameter
     * corresponding to this object
     *                  - a map, detailing permissible values for (a subset of)
     * the other operator parameters if this parameter is assigned this value.
     * Each entry of this map is a pair consisting of:
     *                      - the index of another operator parameter, i.e. one
     * other than <code>paramID</code>, and where parameter 0 is the first
     * parameter.
     *                      - a set of values that can be given to that
     * parameter
     *
     *  @return  An <code>iterator<code> pointing to where <code>d</code> was
     * inserted; or, if JPDCs were previously defined from the given value of
     * the parameter (<code>d.first</code>), an iterator pointing to where these
     * are.
     *
     */
    inline iterator insert(const int &paramID, const iterator &oItr,
                           const pair< int, map< int, set< int > > > &d) {
      assert(d.second.find(paramID) == d.second.end());

      return jpdcs.insert(oItr, d);
    }

    /**
     *  Erase the specified JPDC record.
     *
     *  @param itr  An iterator pointing to the JPDC record to erase
     */
    inline void erase(const iterator &itr) { jpdcs.erase(itr); }

    /**
     *  Record a JPDC from the parameter corresponding to this object.
     *
     *  @param thisParam  The ID of the parameter corresponding to this object.
     * This serves to ensure the JPDCs are well-formed: a parameter may not
     * enforce restrictions upon its own value.
     *  @param thisVal    The value setting for this parameter from which to
     * enforce restrictions upon the values of other parameters
     *  @param otherParam The ID of another parameter of the operator (i.e.
     * <code>thisParam != otherParam</code>)
     *  @param otherVal   A value of <code>otherParam</code> that is permissible
     * if the parameter corresponding to this object is given the value
     * <code>thisVal</code>.
     *
     */
    inline void addRestriction(const int &thisParam, const int &thisVal,
                               const int &otherParam, const int &otherVal) {
      assert(thisParam != otherParam);
      assert(otherParam >= 0);
      assert(thisParam >= 0);
      jpdcs[thisVal][otherParam].insert(otherVal);
    }

    /**
     *  Find the JPDCs from the given value of of the parameter corresponding to
     * this object.
     *  @param thisVal  A value for the parameter corresponding to this object
     *  @return  An iterator pointing to the JPDCs from that value - returns
     * <code>end()</code> if undefined.
     */
    inline const_iterator find(const int &thisVal) const {
      return jpdcs.find(thisVal);
    }
  };

  // typedef map<int, map<int, set<int> > > JPDCData;

  /**
   *  A class representing the constraints on the values an actions'
   *  parameters can take.  For each parameter, the class records
   *  a set of possible values for that parameter.  Then, through
   *  use of <code>JPDCData</code> objects, it records constraints
   *  between pairs of parameters.
   *
   *  @see JPDCData, ParameterDomainConstraints
   */
  class ParameterDomainsAndConstraints {
   protected:
    /**
     * A method to trim redundant elements from the targets of JPDCs.  If a JPDC
     * records that setting parameter <code>a</code> to the value <code>b</code>
     * means that parameter <code>c</code> could take the value <code>d</code>,
     * then if <code>d</code> is removed from the domain of <code>c</code>, it
     * is redundant, and hence should also be removed from the JPDC.
     *
     * @param oItr  An iterator pointing to the JPDCs arising when some value
     * has been given to some parameter. From this, all redundant restrictions
     * on other parameters' values will be eliminated.
     */
    void trim(JPDCData::iterator &oItr) {
      map< int, set< int > >::iterator trimItr = oItr->second.begin();
      const map< int, set< int > >::iterator trimEnd = oItr->second.end();

      for (; trimItr != trimEnd; ++trimItr) {
        if (!domains[trimItr->first].first) {
          set< int > isect;
          std::set_intersection(
              domains[trimItr->first].second.begin(),
              domains[trimItr->first].second.end(), trimItr->second.begin(),
              trimItr->second.end(),
              insert_iterator< set< int > >(isect, isect.begin()));
          isect.swap(trimItr->second);
        }
      }
    }

   public:
    /** How many parameters the operator has. */
    int domCount;

    /**
     *  The domains of the parameters of this operator.  Each entry in the
     * vector is a pair, where:
     *   - if the first value is <code>true</code>, the parameter can in theory
     * take any type-correct value
     *   - otherwise, the second value contains a set of permissible values for
     * that parameter
     *
     *  (The use of a paired representation is to improve the efficiency of
     * union/intersection operations.)
     */
    vector< pair< bool, set< int > > > domains;

    /**
     * Joint parameter domain constraints.  For each parameter <code>i</code>,
     * <code>jpdcs[i][j][k]</code> (if defined) is a set of permissible values
     * for parameter <code>k</code> if <code>i</code> is set to <code>j</code>.
     * Note that these are set in strictly descending order of parameter index -
     * i.e. <code>jpdcs[i][j][k]</code> is only ever defined if <code>k <
     * i</code>. This is to save space/time, and also because in the
     * instantiation code, JPDCs are only ever used in a downwards direction.
     */
    vector< JPDCData > jpdcs;

    /**
     *  Initialise a record of the parameter domains and constraints between
     * them for an operator.
     *
     *  @param i  The number of operator parameters
     */
    ParameterDomainsAndConstraints(const int &i)
        : domCount(i), domains(i, make_pair(true, set< int >())), jpdcs(i) {}

    /**
     *  Copy constructor: duplicate the record of the parameter domains and
     * constraints between them.
     *
     *  @param d  The object to copy
     */
    ParameterDomainsAndConstraints(const ParameterDomainsAndConstraints &d)
        : domCount(d.domCount), domains(d.domains), jpdcs(d.jpdcs){};

    /**
     *  A function to either take the union of, or the intersection, of two set
     * pairs, overwriting one of the inputs.
     *
     *  @param dest    One of the input set pairs, to be overwritten by the
     * output of the intersection/union operation
     *  @param other   The other input set pair for the operation
     *  @param doUnion If <code>true</code>, the union of the two sets is taken.
     * Otherwise, the intersection is taken.
     *
     *  @return  <code>true</code> if elements were removed from
     * <code>dest</code>.
     *
     *  @see domains
     */
    static bool mergePair(pair< bool, set< int > > &dest,
                          const pair< bool, set< int > > &other,
                          const bool &doUnion) {
      bool toReturn = false;
      if (dest.first) {
        if (!doUnion) {
          if (!(dest.first = other.first)) {
            dest.second = other.second;
            toReturn = true;
          }
        }
      } else if (doUnion && other.first) {
        dest.second = other.second;
        dest.first = true;
        toReturn = true;
      } else if (!other.first) {
        set< int >::iterator usItr = dest.second.begin();
        const set< int >::iterator usEnd = dest.second.end();

        set< int >::const_iterator dItr = other.second.begin();
        const set< int >::const_iterator dEnd = other.second.end();

        while (usItr != usEnd && dItr != dEnd) {
          if (*usItr < *dItr) {
            if (doUnion) {
              ++usItr;
            } else {
              toReturn = true;
              const set< int >::iterator usDel = usItr++;
              dest.second.erase(usDel);
            }
          } else if (*usItr > *dItr) {
            if (doUnion) {
              dest.second.insert(*dItr);
              toReturn = true;
            }
            ++dItr;
          } else {
            ++usItr;
            ++dItr;
          }
        }
        if (!doUnion && usItr != usEnd) {
          dest.second.erase(usItr, usEnd);
          toReturn = true;
        }
      }
      return toReturn;
    }

    /** For debugging purposes: print out the details of the JPDCS from the
     * given parameter. */
    void printJPDCs(const int &paramID) {
      JPDCData::const_iterator oItr = jpdcs[paramID].begin();
      const JPDCData::const_iterator oEnd = jpdcs[paramID].end();

      for (; oItr != oEnd; ++oItr) {
        cout << "If set to " << oItr->first << " implies:\n";
        map< int, set< int > >::const_iterator oaItr = oItr->second.begin();
        const map< int, set< int > >::const_iterator oaEnd = oItr->second.end();

        for (; oaItr != oaEnd; ++oaItr) {
          cout << "\t" << (oaItr->first + 1) << " in {";
          set< int >::const_iterator sItr = oaItr->second.begin();
          const set< int >::const_iterator sEnd = oaItr->second.end();

          for (int idx = 0; sItr != sEnd; ++sItr, ++idx) {
            if (idx) cout << ", ";
            cout << *sItr;
          }
          cout << "}\n";
        }
      }
    }

    /**
     *  Merge in new JPDCs from the given parameter ID.  The basic premise is
     * that both the existing JPDCs recorded (if any) and the new ones specified
     * must be respected. Thus, restrictions on other parameters are never
     * deleted, only collated; and in the case where there are overlapping
     * restrictions, the intersection is taken.
     *
     *  More precisely:
     *  - If, previously, the value of a parameter <code>p</code> was
     * restricted, and the new JPDCs make no reference to <code>p</code>, the
     * JPDCs upon <code>p</code> are unaffected (i.e. left as-is).
     *  - If the new JPDCs restrict the value of a parameter <code>q</code>,
     * upon which there were previously no JPDCs from the current parameter,
     * then the JPDCs from this parameter to <code>q</code> are copied into this
     * object.
     *  - Otherwise, both the existing records and the new records impose
     * restrictions on a parameter, based on the value given to that specified.
     * In this case:
     *    - if the existing data has restrictions from a value of the current
     * parameter which the new data does not, these restrictions are unaffected
     *    - if the new data has restrictions from a value of the current
     * parameter which the existing data does not, these restrictions are copied
     * as-is
     *    - otherwise, both the existing data and the new data restrict the
     * value of another parameter when this parameter is given some value, and
     * the intersection of the two permissible value sets is taken: both the
     * existing restrictions and the new restrictions must be respected.
     *
     *  @param paramID  The ID of the parameter for which to specify new JPDCs.
     *  @param newData  The new JPDC data for that parameter.
     */
    void mergeInJPDCs(const int &paramID, const JPDCData &newData) {
      static const bool mergeDebug = false;

      if (jpdcs[paramID].empty()) {
        // if we've never had JPDCs from this parameter, keep all the new ones
        if (mergeDebug) {
          cout << "First time JPDCs have been specified from parameter "
               << (paramID + 1) << endl;
        }
        jpdcs[paramID] = newData;
        if (mergeDebug) printJPDCs(paramID);
        return;
      }

      if (mergeDebug) {
        cout << "Merging in JPDCs for parameter " << (paramID + 1) << endl;
      }
      const bool domainIsFull = domains[paramID].first;

      set< int >::const_iterator domItr, domEnd;

      if (!domainIsFull) {
        domItr = domains[paramID].second.begin();
        domEnd = domains[paramID].second.end();
      }

      if (mergeDebug) {
        if (domainIsFull) {
          cout << "Domain of " << paramID << " is unrestricted\n";
        } else {
          cout << "Domain of " << paramID << " can only be:";
          set< int >::const_iterator dompItr = domains[paramID].second.begin();
          for (; dompItr != domEnd; ++dompItr) {
            cout << " " << *dompItr;
          }
          cout << endl;
        }
      }

      JPDCData::const_iterator nItr = newData.begin();
      const JPDCData::const_iterator nEnd = newData.end();

      JPDCData::iterator oItr = jpdcs[paramID].beginw();
      const JPDCData::iterator oEnd = jpdcs[paramID].endw();

      while (nItr != nEnd && oItr != oEnd) {
        if (nItr->first < oItr->first) {  // if this parameter now has
                                          // restrictions from the value
                                          // nItr->first, but it didn't used to
          if (mergeDebug) {
            cout << "Now have restrictions from the value " << nItr->first
                 << ", but didn't used to\n";
          }
          if (domainIsFull || (domItr != domEnd && nItr->first == *domItr)) {
            if (mergeDebug) {
              cout << " - Copied into the stored JPDCs\n";
            }
            oItr = jpdcs[paramID].insert(paramID, oItr, *nItr);
            assert(oItr->first == nItr->first);
            trim(oItr);

            ++oItr;
            ++nItr;
            if (!domainIsFull) ++domItr;
          } else {
            if (mergeDebug) {
              if (domItr != domEnd) {
                cout << " - However, next possible legal value of this "
                        "parameter "
                        "would be "
                     << *domItr << ", so discarding restrictions\n";
              } else {
                cout << " - However, there are no more legal values of this "
                        "parameter - discarding restrictions\n";
              }
            }
            ++nItr;  // discard restrictions from nItr->first if that value
                     // doesn't exist any more
          }
        } else if (oItr->first < nItr->first) {  // not changing anything about
                                                 // how the value oItr->first
                                                 // affects other parameters
          if (mergeDebug) {
            cout << "Used to have restrictions from the value " << nItr->first
                 << ", but nothing changed there\n";
          }

          if (domainIsFull) {
            if (mergeDebug) {
              cout << " - Moving on\n";
            }
            trim(oItr);
            ++oItr;
          } else {
            if (domItr != domEnd && oItr->first == *domItr) {
              if (mergeDebug) {
                cout << " - Value " << *domItr << " still okay, moving on\n";
              }
              trim(oItr);
              ++oItr;
              ++domItr;
            } else {
              if (mergeDebug) {
                if (domItr != domEnd) {
                  cout << " - However, next possible legal value of this "
                          "parameter would be "
                       << *domItr << ", so discarding restrictions\n";
                } else {
                  cout << " - However, there are no more legal values of this "
                          "parameter - discarding restrictions\n";
                }
              }
              const JPDCData::iterator oDel = oItr++;
              jpdcs[paramID].erase(oDel);
            }
          }
        } else {  // now intersect - have multiple restrictions

          if (mergeDebug) {
            cout << "Merging restrictions stemming from setting the parameter "
                    "to "
                 << oItr->first << endl;
          }
          if (!domainIsFull) {
            if (domItr != domEnd && oItr->first == *domItr) {
              ++domItr;
            } else {
              const JPDCData::iterator oDel = oItr++;
              jpdcs[paramID].erase(oDel);
              ++nItr;
              if (mergeDebug) {
                if (domItr != domEnd) {
                  cout << " - However, next possible legal value of this "
                          "parameter would be "
                       << *domItr << ", so discarding restrictions\n";
                } else {
                  cout << " - However, there are no more legal values of this "
                          "parameter - discarding restrictions\n";
                }
              }
              continue;
            }
          }

          map< int, set< int > >::const_iterator naItr = nItr->second.begin();
          const map< int, set< int > >::const_iterator naEnd =
              nItr->second.end();

          map< int, set< int > >::iterator oaItr = oItr->second.begin();
          const map< int, set< int > >::iterator oaEnd = oItr->second.end();

          while (naItr != naEnd && oaItr != oaEnd) {
            if (naItr->first < oaItr->first) {  // this parameter now has
                                                // restrictions from nItr->first
                                                // onto naItr->first
              if (mergeDebug) {
                cout << "  * Can now (newly) restrict the value of parameter "
                     << (naItr->first + 1) << endl;
              }
              oaItr = oItr->second.insert(oaItr, *naItr);
              assert(oaItr->first == naItr->first);
              ++oaItr;
              ++naItr;
            } else if (oaItr->first <
                       naItr->first) {  // no change on restrictions from
                                        // oItr->first onto oaItr->first
              ++oaItr;
            } else {
              if (mergeDebug) {
                cout << "  * Intersecting restrictions on the value of "
                        "parameter "
                     << (naItr->first + 1) << endl;
                cout << "    +  Before had " << oaItr->second.size()
                     << " options, suggesting " << naItr->second.size() << endl;
              }
              set< int > isect;
              std::set_intersection(
                  oaItr->second.begin(), oaItr->second.end(),
                  naItr->second.begin(), naItr->second.end(),
                  std::insert_iterator< set< int > >(isect, isect.begin()));
              oaItr->second.swap(isect);

              if (mergeDebug) {
                cout << "      - Intersection is of size "
                     << oaItr->second.size() << endl;
              }
              ++oaItr;
              ++naItr;
            }
          }

          for (; naItr != naEnd; ++naItr) {
            oaItr = oItr->second.insert(oaItr, *naItr);
          }

          trim(oItr);

          ++oItr;
          ++nItr;
        }
      }

      for (; oItr != oEnd; ++oItr) {
        trim(oItr);
      }

      for (; nItr != nEnd; ++nItr) {
        oItr = jpdcs[paramID].insert(paramID, oItr, *nItr);
        trim(oItr);
      }

      if (mergeDebug) {
        cout << "After merge:\n";
        printJPDCs(paramID);
      }
    }
  };

  /*


  class ParameterDomains {

  public:
          int domCount;
          vector<pair<bool, set<int> > > domains;

          ParameterDomains(const int & i) : domCount(i),
  domains(i,make_pair(true,set<int>())) {}; ParameterDomains(const
  ParameterDomains & d) : domCount(d.domCount), domains(d.domains) {};

          static bool mergePair(pair<bool, set<int> > & dest, const pair<bool,
  set<int> > & other, const bool & doUnion) { bool toReturn = false; if
  (dest.first) { if (!doUnion) { if (!(dest.first = other.first)) { dest.second
  = other.second; toReturn = true;
                                  }
                          }
                  } else if (doUnion && other.first) {
                          dest.second = other.second;
                          dest.first = true;
                          toReturn = true;
                  } else if (!other.first) {
                          set<int>::iterator usItr = dest.second.begin();
                          const set<int>::iterator usEnd = dest.second.end();

                          set<int>::const_iterator dItr = other.second.begin();
                          const set<int>::const_iterator dEnd =
  other.second.end();

                          while (usItr != usEnd && dItr != dEnd) {
                                  if (*usItr < *dItr) {
                                          if (doUnion) {
                                                  ++usItr;
                                          } else {
                                                  toReturn = true;
                                                  const set<int>::iterator usDel
  = usItr++; dest.second.erase(usDel);
                                          }
                                  } else if (*usItr > *dItr) {
                                          if (doUnion) {
                                                  dest.second.insert(*dItr);
                                                  toReturn = true;
                                          }
                                          ++dItr;
                                  } else {
                                          ++usItr; ++dItr;
                                  }
                          }
                          if (!doUnion && usItr != usEnd) {
                                  dest.second.erase(usItr,usEnd);
                                  toReturn = true;
                          }
                  }
                  return toReturn;
          }
  };

  class InstQueueEntry {

  public:
          vector<VAL::const_symbol *> vars;
          int assignNext;

          InstQueueEntry(const int & i) : vars(i,0), assignNext(i-1) {};

  };
  */
  struct ConstSymbolLT {
    bool operator()(const VAL::const_symbol *const a,
                    const VAL::const_symbol *const b) const {
      return (a->getName() < b->getName());
    }
  };

  class PDCIterator;

  class ParameterDomainConstraints : public VAL::VisitController {
   protected:
    typedef map< const VAL::const_symbol *const, int, ConstSymbolLT > pviLookup;
    //	const VAL::operator_ * const op;
    /** How many parameters the operator has */
    const int varCount;

    /**
     * A stack of domain constraints, built whilst visiting the tree defining
     * the actions' preconditions. Internal nodes (conjunction and disjunction)
     * push a new object onto the stack, and leaf nodes (facts) update the entry
     * on the top of the stack (the front of the list) to reflect their
     * constraints: taking the union in the case of disjunction, or the
     * intersection in the case of conjunction.  Whether the parent node is a
     * conjunction or disjunction node is recorded in the member variable
     * <code>doUnion</code>: if the front element is <code>true</code>, the
     * parent is a disjunction node, and the union can be taken; otherwise, the
     * parent is a conjunction and the intersection should be taken.
     */
    list< ParameterDomainsAndConstraints > domainStack;

    /**
     *  For each parameter of the operator, a vectors of the type-correct
     * parameter bindings. These form the basis of the <code>int</code>
     * identifiers for parameters used elsewhere in the code: the symbol
     * corresponding to setting parameter <code>a</code> to the value
     *  <code>b</code> is the entry <code>[a][b]</code>.
     */
    vector< vector< VAL::const_symbol * > > possibleParameterValues;

    /**
     * For each action parameter, a map from which values are type-correct to
     * a unique identifier for each.  This allows ints rather than
     * const_symbol*s to be used elsewhere.
     */
    vector< pviLookup > parameterValuesToIndices;

    /** The symbols used in the action definition for each parameter (e.g.
     * <code>?a</code>, <code>?b</code>) */
    vector< VAL::parameter_symbol * > vars;

    /** The symbol denoting the equality predicate */
    VAL::pred_symbol *const equality;

    /**
     * If the front element is <code>true</code>, then when visiting leaf nodes
     * (facts), the parent is a disjunction node, and the permissible domains
     * for each parameter are those according to the fact being visited, and
     * those already on the front of <code>domainStack</code>.  Otherwise, if
     * the front element is <code>false</code>, the intersection is taken, as
     * the parent is a conjunction node and both the new constraints and the
     * existing constraints should be respected.
     */
    list< bool > doUnion;

    /**
     * If a leaf node does not contain a static fact, then the domains can't be
     * restricted beyond the values that are type-correct.  In this case, the
     * index of the parameters referred to by the fact are added to the set at
     * the front of this stack.
     */
    list< set< int > > panic;

    /** True if the parent of the current node is the root node.*/
    bool rootSpecial;

    /** A set of parameters to visit, to propagate parameter constraint changes.
     */
    set< int > updateFrom;

    friend class PDCIterator;

    template < typename T >
    ParameterDomainConstraints(T *parameters, const string &name,
                               VAL::TypeChecker &tc)
        : varCount(parameters->size()),
          possibleParameterValues(varCount),
          parameterValuesToIndices(varCount),
          vars(varCount),
          equality(VAL::current_analysis->pred_tab.symbol_probe("=")) {
      {
        int i = 0;
        for (typename T::const_iterator p = parameters->begin();
             p != parameters->end(); ++p, ++i) {
          //   symbols[i] = *p;

          if (instantiatedValues.find((*p)->type) == instantiatedValues.end()) {
            try {
              instantiatedValues[(*p)->type] = tc.range(*p);
            } catch (const TypeException &e) {
              cerr << "A problem has been encountered with your domain/problem "
                      "file.\n";
              cerr << "--------------------------------------------------------"
                      "--"
                      "---\n";
              cerr
                  << "Unfortunately, a type error has been encountered in your "
                     "domain and problem file,\n";
              cerr << "and the planner has to terminate.  Specifically, for "
                      "parameter "
                   << (i + 1) << "\n";
              cerr << "of the action/derivation rule '" << name << "':\n\n\t";

              Verbose = true;
              try {
                instantiatedValues[(*p)->type] = tc.range(*p);
              } catch (const TypeException &f) {
              }
              exit(1);
            }
          };
          possibleParameterValues[i].insert(
              possibleParameterValues[i].end(),
              instantiatedValues[(*p)->type].begin(),
              instantiatedValues[(*p)->type].end());
          vars[i] = *p;
        };
      }
      for (int v = 0; v < varCount; ++v) {
        updateFrom.insert(v);
        const int jLim = possibleParameterValues[v].size();
        for (int j = 0; j < jLim; ++j) {
          parameterValuesToIndices[v][possibleParameterValues[v][j]] = j;
        }
      }
      domainStack.push_front(ParameterDomainsAndConstraints(varCount));

      // inheriting classes should always call their own propagate after this
      // constructor
      // propagate();

      /*cout << "Final JPDCs:\n";

      for (int v = 1; v < varCount; ++v) {
          cout << v << ":\n";
          domainQueue.front().printJPDCs(v);
      }*/
      /*for (int v = 0; v < varCount; ++v) {
       pair<bool,set<int> > & currPair = domainQueue.front().domains[v];
       if (!currPair.first) {
        vector<VAL::const_symbol*> usedToBe(currPair.second.size(),0);
        usedToBe.swap(possibleParameterValues[v]);

        set<int>::iterator ndItr = currPair.second.begin();
        const set<int>::iterator ndEnd = currPair.second.end();

        for (int ins = 0; ndItr != ndEnd; ++ndItr, ++ins) {
         possibleParameterValues[v][ins] = usedToBe[*ndItr];
        }
       }
      }*/
    }

    virtual void propagate() = 0;

#ifndef NDEBUG
    /** @brief  Whether to be verbose - by default, no, but inheriting classes
     * my change this. */
    virtual bool beVerbose() const { return false; }

    virtual void checkParameterOptionRemains(const int &paramID) {}
#endif

   public:
#ifndef NDEBUG
    const vector< vector< VAL::const_symbol * > > &getPossibleParameterValues()
        const {
      return possibleParameterValues;
    }
#endif

    ~ParameterDomainConstraints(){};

    /*	virtual void propagate() {
                    while (!updateFrom.empty()) {
    //			set<int>::iterator nItr = updateFrom.begin();
    //			const int currParam = *nItr;
    //			updateFrom.erase(nItr);
                            updateFrom.clear();
                            doUnion.push_front(false);
                            panic.push_front(set<int>());
                            rootSpecial = true;
                            op->visit(this);
                            doUnion.pop_front();
                            panic.pop_front();
                    }
            }
    */

    /**
     *  Visit a <code>simple_goal</code>, i.e. a propositional precondition.  If
     * the precondition is static (i.e. fixed in the initial state), then it is
     * used to restrict the permissible values of the domains of the parameters
     * referred to by the precondition.  Otherwise, not a lot can be done, and
     * any type-correct binding for the mentioned parameters is assumed to be
     * reasonable.  @see domainStack, panic
     */
    virtual void visit_simple_goal(VAL::simple_goal *s) {
#ifndef NDEBUG
      const bool jpdcDebug = beVerbose();
#else
      static const bool jpdcDebug = false;
#endif

      holding_pred_symbol *const hps = EPS(s->getProp()->head)->getParent();

      bool inPanic = false;
      if (hps == this->equality) {
        if (jpdcDebug) cout << " - Found the = fact\n";
        if (!doUnion.front()) return;
        inPanic = true;
      }

      bool hpsIsStatic = true;

      {
        holding_pred_symbol::PIt epsItr = hps->pBegin();
        const holding_pred_symbol::PIt epsEnd = hps->pEnd();

        for (; epsItr != epsEnd; ++epsItr) {
          if (!(*epsItr)->appearsStatic()) {
            hpsIsStatic = false;
            break;
          }
        }
      }

      if (!hpsIsStatic) {
        if (jpdcDebug)
          cout << " - Found a non-static fact " << s->getProp()->head->getName()
               << " at " << hps << std::endl;
        if (!doUnion.front()) return;
        inPanic = true;
      }

      const bool debug = jpdcDebug;  /// false;
      if (debug || jpdcDebug) {
        cout << "Static precondition " << s->getProp()->head->getName()
             << " at " << hps << std::endl;
        hps->write(cout);
      }

      const int affects = s->getProp()->args->size();

      // If parameterIndex[x]=y then predicate arg x is parameter y of the
      // action
      vector< int > parameterIndex(affects, -1);

      // Constant symbols embedded in predicates, e.g. (on ?x table)
      vector< const VAL::const_symbol * > hasToMatch(affects,
                                                     (VAL::const_symbol *)0);

      // New set of permissible domain values for the action parameter
      // corresponding to predicate argument i
      vector< pair< bool, set< int > > > newSets(
          affects, make_pair(false, set< int >()));
      vector< JPDCData > newJPDCs(affects);

      // Old set of permissible domain values for the action parameter
      // corresponding to predicate argument i.  For each pair, the first
      // argument is true if the set hasn't been touched yet, so if we take the
      // intersection of several sets, we copy the first, set the bool to false,
      // and then intersection with the rest.
      vector< pair< bool, set< int > > * > oldSets(
          affects, (pair< bool, set< int > > *)0);
      vector< JPDCData * > oldJPDCs(affects, (JPDCData *)0);

      int affWithBiggestParam = -1;
      int biggestParam = -1;

      VAL::parameter_symbol_list::iterator argItr = s->getProp()->args->begin();
      for (int aff = 0; aff < affects; ++aff, ++argItr) {
        if (const VAL::const_symbol *const c =
                dynamic_cast< const VAL::const_symbol * >(*argItr)) {
          if (debug) cout << "\tArgument " << aff << " is constant\n";
          hasToMatch[aff] = c;
        } else {
          const int paramID =
              static_cast< const VAL::IDsymbol< VAL::var_symbol > * >(*argItr)
                  ->getId();
          if (inPanic) {
            panic.front().insert(paramID);
          } else {
            oldSets[aff] = &(domainStack.front().domains[paramID]);
            oldJPDCs[aff] = &(domainStack.front().jpdcs[paramID]);
          }
          parameterIndex[aff] = paramID;
          if (debug)
            cout << "\tArgument " << aff << " is action parameter " << paramID
                 << "\n";
          if (paramID > biggestParam) {
            biggestParam = paramID;
            affWithBiggestParam = aff;
          }
        }
      }

      if (inPanic) {
        if (debug) cout << "In a panic, returning\n";
        return;
      }

      const bool isect = !doUnion.front();

      list< IState::iterator > checkInit;

      {
        holding_pred_symbol::PIt epsItr = hps->pBegin();
        const holding_pred_symbol::PIt epsEnd = hps->pEnd();

        const IState::iterator isEnd = InitialStateEvaluator::initState.end();

        for (; epsItr != epsEnd; ++epsItr) {
          const IState::iterator isItr =
              InitialStateEvaluator::initState.find(*epsItr);
          if (isItr != isEnd) {
            checkInit.push_back(isItr);
          }
        }
      }

      // If there are no propositions of this predicate type defined, empty the
      // domains
      if (checkInit.empty()) {
        if (debug) {
          cout << "No predicates of type " << s->getProp()->head->getName()
               << " are defined in the initial state, bailing out\n";
        }
        if (isect) {
          for (int aff = 0; aff < affects; ++aff) {
            if (oldSets[aff]) {
              oldSets[aff]->first = false;
              oldSets[aff]->second.clear();
            }
          }
          for (int aff = 0; aff < affects; ++aff) {
            if (oldJPDCs[aff]) {
              const int paramID = parameterIndex[aff];
              if (paramID >= 1) {
                domainStack.front().mergeInJPDCs(paramID, newJPDCs[aff]);
              }
            }
          }
        }
        return;
      }

      vector< int > scratch(affects, -1);

      int gi = 0;

      list< IState::iterator >::const_iterator predItr = checkInit.begin();
      const list< IState::iterator >::const_iterator predEnd = checkInit.end();

      for (; predItr != predEnd; ++predItr) {
        vector< VAL::parameter_symbol_list * >::iterator groundItr =
            (*predItr)->second.begin();
        const vector< VAL::parameter_symbol_list * >::iterator groundEnd =
            (*predItr)->second.end();

        // Loop over each instance of the predicate in the initial state
        for (; groundItr != groundEnd; ++groundItr, ++gi) {
          bool keep = true;
          {
            VAL::parameter_symbol_list::iterator wwItr = (*groundItr)->begin();
            const VAL::parameter_symbol_list::iterator wwEnd =
                (*groundItr)->end();

            for (int aff = 0; wwItr != wwEnd; ++wwItr, ++aff) {
              const VAL::parameter_symbol *const weWant = *wwItr;
              const VAL::const_symbol *const asConst =
                  dynamic_cast< const VAL::const_symbol * >(weWant);
              assert(asConst);

              if (hasToMatch[aff]) {
                // Simple case: precondition of the action said (connected ?x
                // table), and this fact says (connected ?a ?b) where ?b !=
                // table
                // - no way this static fact can support this action
                if (hasToMatch[aff] == asConst) {
                  scratch[aff] = -1;
                } else {
                  if (debug)
                    cout << "\t\tDiscarding ground instance " << gi
                         << " - const parameter mismatch\n";
                  keep = false;
                  break;
                }
              } else {
                const pviLookup::iterator findEntry =
                    parameterValuesToIndices[parameterIndex[aff]].find(asConst);
                if (findEntry ==
                    parameterValuesToIndices[parameterIndex[aff]].end()) {
                  // no way this argument of the fact can be used as a binding
                  // for the relevant action parameter
                  keep = false;
                  break;
                }

                const int asInt = findEntry->second;
                scratch[aff] = asInt;

                if (isect) {  // if we're keeping only the intersection  (i.e.
                              // doing conjunction over facts)
                  if (oldSets[aff] &&
                      !oldSets[aff]->first) {  // if the old choices are
                                               // defined, and are non-empty
                    if (oldSets[aff]->second.find(asInt) ==
                        oldSets[aff]->second.end()) {  // if this binding has
                                                       // already been pruned
                      keep = false;
                      break;
                    }
                  }
                }
              }
            }
          }

          if (keep) {  // If the fact is consistent with permissible parameter
                       // values
            if (jpdcDebug || debug) {
              cout << "\t\tConsistent static fact " << gi << ": ("
                   << hps->getName();
              VAL::parameter_symbol_list::iterator wwItr =
                  (*groundItr)->begin();
              const VAL::parameter_symbol_list::iterator wwEnd =
                  (*groundItr)->end();
              for (; wwItr != wwEnd; ++wwItr) {
                cout << " " << (*wwItr)->getName();
              }
              cout << ")\n";
            }
            if (isect) {
              for (int aff = 0; aff < affects; ++aff) {
                if (scratch[aff] != -1) {  // if it wasn't a const parameter
                  newSets[aff].second.insert(
                      scratch[aff]);  // add it to the new domain of permissible
                                      // values for the parameter

                  if (aff == affWithBiggestParam) {
                    const int pIdx1 = biggestParam;

                    for (int aff2 = 0; aff2 < affects; ++aff2) {
                      const int pIdx2 = parameterIndex[aff2];
                      if (pIdx2 < 0) continue;  // if it's a constant
                      if (pIdx2 >= pIdx1)
                        continue;  // only store backwards jpdcs (see comments
                                   // on the jpdcs variable definition
                      if (jpdcDebug) {
                        cout << "  * JPDC: action parameter " << (pIdx1 + 1)
                             << "=" << scratch[aff] << "  =>  " << (pIdx2 + 1)
                             << "=" << scratch[aff2] << endl;
                      }
                      newJPDCs[aff].addRestriction(pIdx1, scratch[aff], pIdx2,
                                                   scratch[aff2]);
                    }
                  }
                }
              }
            } else {
              for (int aff = 0; aff < affects; ++aff) {
                if (oldSets[aff] && !oldSets[aff]->first &&
                    scratch[aff] != -1) {
                  if (oldSets[aff]->second.insert(scratch[aff]).second) {
                    updateFrom.insert(parameterIndex[aff]);
                  };
                }
              }
            }
          }
        }
      }
      if (isect) {
        for (int aff = 0; aff < affects; ++aff) {
          if (oldSets[aff]) {  // if we previously had some options for this
                               // parameter
            const int paramID = parameterIndex[aff];
            if (newSets[aff].second.size() ==
                possibleParameterValues[paramID].size()) {
              // if the new options are just all entities of the right type,
              // there's not a lot of point taking the intersection with this
              // lower down, so we'll clear the set and set the bool to true
              // (pretending it's never been touched)
              newSets[aff].second.clear();
              newSets[aff].first = true;
            }

            if (ParameterDomainsAndConstraints::mergePair(
                    domainStack.front().domains[paramID], newSets[aff],
                    doUnion.front())) {
              // If we've shrunk the domain, revisit dependents
              updateFrom.insert(paramID);
            };

#ifndef NDEBUG
            checkParameterOptionRemains(paramID);
#endif
          }
        }
        for (int aff = 0; aff < affects; ++aff) {
          const int paramID = parameterIndex[aff];
          if (paramID != -1)
            domainStack.front().mergeInJPDCs(paramID, newJPDCs[aff]);
        }
      } else {
        for (int aff = 0; aff < affects; ++aff) {
          if (oldSets[aff] && !oldSets[aff]->first) {
            const int paramID = parameterIndex[aff];
            if (oldSets[aff]->second.size() ==
                possibleParameterValues[paramID].size()) {
              oldSets[aff]->second.clear();
              oldSets[aff]->first = true;
            }
          }
        }
      }
    }

    /**
     *  Method called when visiting nodes for which no static analysis has yet
     *  been coded.  It updates <code>panic</code> to assume that any
     * type-correct value for any parameter is reasonable.
     */
    void blindPanic() {
      for (int v = 0; v < varCount; ++v) {
        panic.front().insert(v);
      }
    }

    /** Stub code to visit quantified goals.  For now, do nothing.  @see
     * blindPanic */
    virtual void visit_qfied_goal(VAL::qfied_goal *) {
      if (doUnion.front()) {
        blindPanic();
      }
    };

    /**
     * Visit a conjunction node.  A new entry is pushed to the front of
     * <code>domainStack</code>, and the value <code>false</code> is pushed to
     * the front of <code>doUnion</code>.  Hence, when visiting the children,
     * the intersection of their constraints is taken, i.e. all must be
     * respected.
     */
    virtual void visit_conj_goal(VAL::conj_goal *c) {
      static const bool conjDebug = false;
      const bool haveSpecial = rootSpecial;
      rootSpecial = false;
      if (!haveSpecial) {
        domainStack.push_front(domainStack.front());
        doUnion.push_front(false);
        panic.push_front(set< int >());
      }

      goal_list::const_iterator i = c->getGoals()->begin();
      goal_list::const_iterator iEnd = c->getGoals()->end();
      for (int j = 1; i != iEnd; ++i, ++j) {
        if (conjDebug)
          cout << "& Visiting conjunction goal element " << j << endl;
        (*i)->visit(this);
      }
      if (!haveSpecial) {
        doUnion.pop_front();
        list< ParameterDomainsAndConstraints >::iterator second =
            domainStack.begin();
        ++second;
        for (int v = 0; v < varCount; ++v) {
          if (panic.front().find(v) != panic.front().end()) {
            const pair< bool, set< int > > panicPair(true, set< int >());
            if (ParameterDomainsAndConstraints::mergePair(
                    second->domains[v], panicPair, doUnion.front())) {
              updateFrom.insert(v);
            };
          } else {
            if (ParameterDomainsAndConstraints::mergePair(
                    second->domains[v], domainStack.front().domains[v],
                    doUnion.front())) {
              updateFrom.insert(v);
            };
          }
        }
        domainStack.pop_front();
        panic.pop_front();
      }
    }

    /**
     * Visit a disjunction node.  A new entry is pushed to the front of
     * <code>domainStack</code>, and the value <code>true</code> is pushed to
     * the front of <code>doUnion</code>.  Hence, when visiting the children,
     * the union of their constraints is taken, i.e. at least one must be
     * respected.
     */
    virtual void visit_disj_goal(VAL::disj_goal *c) {
      domainStack.push_front(domainStack.front());

      doUnion.push_front(true);
      panic.push_front(set< int >());

      goal_list::const_iterator i = c->getGoals()->begin();
      goal_list::const_iterator iEnd = c->getGoals()->end();
      for (; i != iEnd; ++i) {
        (*i)->visit(this);
      }
      doUnion.pop_front();

      list< ParameterDomainsAndConstraints >::iterator second =
          domainStack.begin();
      ++second;
      for (int v = 0; v < varCount; ++v) {
        if (panic.front().find(v) != panic.front().end()) {
          const pair< bool, set< int > > panicPair(true, set< int >());
          if (ParameterDomainsAndConstraints::mergePair(
                  second->domains[v], panicPair, doUnion.front())) {
            updateFrom.insert(v);
          };
        } else {
          if (ParameterDomainsAndConstraints::mergePair(
                  second->domains[v], domainStack.front().domains[v],
                  doUnion.front())) {
            updateFrom.insert(v);
          };
        }
      }
      panic.pop_front();
    }

    /**
     * Visit timed PDDL goals (<code>at start</code> etc.).  We assume the time
     * specifier can be stripped.
     */
    virtual void visit_timed_goal(VAL::timed_goal *t) {
      t->getGoal()->visit(this);
    }

    /** Stub code to visit implication goals.  For now, do nothing.  @see
     * blindPanic */
    virtual void visit_imply_goal(VAL::imply_goal *) { blindPanic(); }

    /** Stub code to visit negative goals.  For now, do nothing.  @see
     * blindPanic
     */
    virtual void visit_neg_goal(VAL::neg_goal *) { blindPanic(); }

    /** Stub code to visit numeric goals.  For now, these are ignored - any
     * domain
     * constraints are determined solely from the logical preconditions. */
    virtual void visit_comparison(VAL::comparison *){};
    virtual void visit_preference(VAL::preference *){};
    virtual void visit_event(VAL::event *e) {
      if (e->precondition) e->precondition->visit(this);
    };
    virtual void visit_process(VAL::process *p) {
      if (p->precondition) p->precondition->visit(this);
    };
    virtual void visit_action(VAL::action *o) {
      if (o->precondition) o->precondition->visit(this);
    };
    virtual void visit_derivation_rule(VAL::derivation_rule *drv) {
      if (drv->get_body()) drv->get_body()->visit(this);
    };
    virtual void visit_durative_action(VAL::durative_action *o) {
      if (o->precondition) o->precondition->visit(this);
    };

    virtual void visit_plus_expression(VAL::plus_expression *s){};
    virtual void visit_minus_expression(VAL::minus_expression *s){};
    virtual void visit_mul_expression(VAL::mul_expression *s){};
    virtual void visit_div_expression(VAL::div_expression *s){};
    virtual void visit_uminus_expression(VAL::uminus_expression *s){};
    virtual void visit_int_expression(VAL::int_expression *s){};
    virtual void visit_float_expression(VAL::float_expression *s){};
    virtual void visit_special_val_expr(VAL::special_val_expr *s){};
    virtual void visit_func_term(VAL::func_term *s){};

    virtual void fleshOut(
        vector< vector< VAL::const_symbol * >::const_iterator > &vals,
        vector< vector< VAL::const_symbol * >::const_iterator > &starts,
        vector< vector< VAL::const_symbol * >::const_iterator > &ends, int &c) {
      for (int i = 0; i < varCount; ++i) {
        vals[i] = starts[i] = possibleParameterValues[i].begin();
        ends[i] = possibleParameterValues[i].end();
        c *= possibleParameterValues[i].size();
      }
    }

    /**
     * Obtain an iterator pointing to the first possible combination of
     * parameter bindings.  Call this once the actions' preconditions have been
     * visited by this class.
     *
     * @Return An iterator pointing to the first combination of parameter
     * bindings
     */
    virtual PDCIterator *getIterator();
  };

  class OperatorParameterDomainConstraints : public ParameterDomainConstraints {
   protected:
    /** @brief The operator for which parameter domain constraints are being
     * constructed. */
    const VAL::operator_ *const op;

    virtual void propagate() {
      while (!updateFrom.empty()) {
        //   set<int>::iterator nItr = updateFrom.begin();
        //   const int currParam = *nItr;
        //   updateFrom.erase(nItr);
        updateFrom.clear();
        doUnion.push_front(false);
        panic.push_front(set< int >());
        rootSpecial = true;
        op->visit(this);
        doUnion.pop_front();
        panic.pop_front();
      }
    }

#ifndef NDEBUG

    virtual bool beVerbose() const {
      return (op == instantiatedOp::insistOnOp);
    }

    virtual void checkParameterOptionRemains(const int &paramID) {
      if (instantiatedOp::insistOnOp == op) {
        const int lookedUp =
            parameterValuesToIndices[paramID]
                .find(instantiatedOp::insistOnOpParameters[paramID])
                ->second;

        assert(domainStack.front().domains[paramID].first ||
               (domainStack.front().domains[paramID].second.find(lookedUp) !=
                domainStack.front().domains[paramID].second.end()));
      }
    }

#endif

   public:
    OperatorParameterDomainConstraints(const VAL::operator_ *const opIn,
                                       VAL::TypeChecker &tc)
        : ParameterDomainConstraints(opIn->parameters, opIn->name->getName(),
                                     tc),
          op(opIn) {
      propagate();
    }
  };

  class DerivationRuleParameterDomainConstraints
      : public ParameterDomainConstraints {
   protected:
    /** @brief The derivation rule for which parameter domain constraints are
     * being constructed. */
    const VAL::derivation_rule *const rule;

    virtual void propagate() {
      while (!updateFrom.empty()) {
        //   set<int>::iterator nItr = updateFrom.begin();
        //   const int currParam = *nItr;
        //   updateFrom.erase(nItr);
        updateFrom.clear();
        doUnion.push_front(false);
        panic.push_front(set< int >());
        rootSpecial = true;
        rule->visit(this);
        doUnion.pop_front();
        panic.pop_front();
      }
    }

   public:
    DerivationRuleParameterDomainConstraints(
        const VAL::derivation_rule *const ruleIn, VAL::TypeChecker &tc)
        : ParameterDomainConstraints(ruleIn->get_head()->args,
                                     ruleIn->get_head()->head->getName(), tc),
          rule(ruleIn) {
      propagate();
    }
  };

  /**
   *  A class representing an iterator over the possible parameter bindings for
   *  an operator.  It respects all domain constraints and JPDCs.
   */
  class PDCIterator {
   protected:
    /**
     * Each domain set used within the iterator is annotated with one of these
     * values.
     *  - <code>whole</code> means the set contains all type-safe values for
     * that parameter
     *  - <code>needsDeleting</code> means the set was created following an
     * intersection operation from multiple constraints on that parameter.  It
     * must be deleted when no longer needed.
     *  - <code>borrowed</code> means the set was borrowed from a JPDC, and
     * hence can be used but must not be deleted, as this will be handled by the
     * <code>JPDCData</code> class.
     */
    enum SetStatus { whole = 0, needsDeleting = 1, borrowed = 2 };

    friend class ParameterDomainConstraints;

    ParameterDomainConstraints *const parent;
    ParameterDomainsAndConstraints &pds;

    int varCount;
    int lim;
    bool valid;

    vector< list< pair< set< int > *, SetStatus > > > allowedValuesForParameter;
    vector< set< int > > wholeSets;
    vector< set< int >::iterator > valItrs;
    vector< set< int >::iterator > valEnds;
    vector< vector< bool > > popSet;

    /**
     *  Update the permissible domains of the parameters of the actions, based
     * on the assignments made thus far.  The parameter given is the index of
     * the parameter from which the JPDCs are to be used to restrict the values
     * of lower-indexed parameters.  Its JPDCs are applied to the possible
     * values of earlier parameters (recording the new sets in
     * <code>allowedValuesForParameter</code>.  The procedure is essentially
     * recursive, but to support iteration rather than having to explicitly
     * enumerate and then return the possible bindings, it is preempted, and
     * enough information is stored in <code>allowedValuesForParameter</code>
     * and <code>popSet</code> to allow it to pick up from where it left off,
     * and update from the next value of the specified parameter each time it is
     * called.
     *
     *  @param x  The parameter index to visit, using its JPDCs to restrict the
     * values of lower-indexed parameters, i.e. <code>i < x</code>.
     *
     *  @return <code>true</code> if there are any remaining possible parameter
     * bindings to the operator.
     */
    bool rollover(int x) {
      static const bool debug = false;

      while (x) {
        const int xVal = *(valItrs[x]);
        if (debug) {
          cout << "Considering parameter " << x + 1 << " = "
               << (*this)[x]->getName() << endl;
        }
        bool keepValue = true;
        const JPDCData::const_iterator xJPDC = pds.jpdcs[x].find(xVal);
        if (xJPDC != pds.jpdcs[x].end()) {
          if (debug) {
            cout << "- JPDCs affecting " << xJPDC->second.size()
                 << " other parameters\n";
          }
          map< int, set< int > >::const_iterator cItr = xJPDC->second.begin();
          const map< int, set< int > >::const_iterator cEnd =
              xJPDC->second.end();
          for (; cItr != cEnd; ++cItr) {
            if (allowedValuesForParameter[cItr->first].front().second ==
                whole) {
              if (debug)
                cout << "  * First time a restriction has been put on "
                     << (1 + cItr->first) << ", using the JPDC set\n";
              allowedValuesForParameter[cItr->first].push_front(make_pair(
                  const_cast< set< int > * >(&(cItr->second)), borrowed));
              popSet[x][cItr->first] = true;
            } else {
              if (debug)
                cout << "  * Taking intersection between existing JPDC set and "
                        "new one for parameter "
                     << (1 + cItr->first) << endl;

              const set< int > *const oldSet =
                  allowedValuesForParameter[cItr->first].front().first;
              allowedValuesForParameter[cItr->first].push_front(
                  make_pair(new set< int >(), needsDeleting));
              set< int > *const destSet =
                  allowedValuesForParameter[cItr->first].front().first;

              std::set_intersection(
                  oldSet->begin(), oldSet->end(), cItr->second.begin(),
                  cItr->second.end(),
                  insert_iterator< set< int > >(*destSet, destSet->begin()));

              popSet[x][cItr->first] = true;
            }
            if (allowedValuesForParameter[cItr->first].front().first->empty()) {
              if (debug)
                cout << "  x Parameter set for " << (1 + cItr->first)
                     << " became empty\n";
              keepValue = false;
              break;
            }
          }
        } else {
          if (debug) {
            cout << "- No JPDCs from setting " << (x + 1) << " to " << xVal
                 << endl;
          }
        }

        if (keepValue) {
          if (debug)
            cout << "Keeping this value of the parameter, moving on to " << x
                 << endl;
          --x;
          valItrs[x] = allowedValuesForParameter[x].front().first->begin();
          valEnds[x] = allowedValuesForParameter[x].front().first->end();
          if (valItrs[x] == valEnds[x]) {
            ++x;
            keepValue = false;
            if (debug)
              cout << "Change of plan - set for " << x << " is empty\n";
          }
        }

        if (!keepValue) {
          if (debug)
            cout << "Not keeping parameter setting for " << (x + 1) << endl;
          for (int ps = 0; ps < x; ++ps) {
            if (popSet[x][ps]) {
              popSet[x][ps] = false;
              if (allowedValuesForParameter[ps].front().second ==
                  needsDeleting) {
                delete allowedValuesForParameter[ps].front().first;
              }
              allowedValuesForParameter[ps].pop_front();
            }
          }
          ++valItrs[x];

          while (valItrs[x] == valEnds[x]) {
            if (debug) {
              cout << "> No more options left for parameter " << (x + 1)
                   << endl;
            }
            ++x;
            if (x == varCount) {
              if (debug) {
                cout << "> No more options left at all, bailing out\n";
              }
              return false;
            }
            for (int ps = 0; ps < x; ++ps) {
              if (popSet[x][ps]) {
                popSet[x][ps] = false;
                if (allowedValuesForParameter[ps].front().second ==
                    needsDeleting) {
                  delete allowedValuesForParameter[ps].front().first;
                }
                allowedValuesForParameter[ps].pop_front();
              }
            }
            if (debug) {
              cout << "> Moved back up to parameter " << (x + 1)
                   << ", and onto next option\n";
            }

            ++valItrs[x];
          }
        }

        if (debug) {
          cout << "At loop exit: about to look at parameter " << (x + 1)
               << endl;
        }
      }

      if (debug) {
        cout << "All done, returning ";
        if (x == 0) {
          cout << "true\n";
        } else {
          cout << "false\n";
        }
      }
      return (x == 0);
    }

    PDCIterator(ParameterDomainConstraints *const p)
        : parent(p),
          pds(parent->domainStack.front()),
          varCount(p->varCount),
          lim(varCount - 1),
          allowedValuesForParameter(varCount),
          wholeSets(varCount),
          valItrs(varCount),
          valEnds(varCount),
          popSet(varCount) {
      for (int i = 1; i < varCount; ++i) {
        popSet[i] = vector< bool >(i, false);
      }

      for (int i = 0; i < varCount; ++i) {
        if (pds.domains[i].first) {
          const int ss = parent->possibleParameterValues[i].size();
          set< int >::iterator insItr = wholeSets[i].end();
          for (int s = 0; s < ss; ++s) {
            insItr = wholeSets[i].insert(insItr, s);
          }
        } else {
          wholeSets[i] = pds.domains[i].second;
        }
      }

      if (!varCount) {
        valid = false;
        return;
      }

      for (int i = 0; i < varCount; ++i) {
        allowedValuesForParameter[i].push_front(
            make_pair(&(wholeSets[i]), whole));
      }

      valItrs[lim] = wholeSets[lim].begin();
      valEnds[lim] = wholeSets[lim].end();

      if (valItrs[lim] == valEnds[lim]) {
        valid = false;
        return;
      }

      valid = rollover(lim);
    }

   public:
    virtual ~PDCIterator() {
      for (int i = 0; i < varCount; ++i) {
        list< pair< set< int > *, SetStatus > >::iterator sItr =
            allowedValuesForParameter[i].begin();
        const list< pair< set< int > *, SetStatus > >::iterator sEnd =
            allowedValuesForParameter[i].end();
        for (; sItr != sEnd; ++sItr) {
          if (sItr->second == needsDeleting) {
            delete sItr->first;
          }
        }
      }
    }

    /** Return the current binding for the specified parameter */
    inline VAL::const_symbol *&operator[](const int &i) const {
      assert(i >= 0 && i < varCount);
      return parent->possibleParameterValues[i][*(valItrs[i])];
    }

    /**
     * Return whether bindings are currently defined - <code>false</code> if no
     * further bindings exist beyond those considered thus far,
     * <code>true</code> otherwise.
     */
    inline const bool &isValid() const { return valid; }

    /**
     *  Move onto the next parameter binding.  In the simple case, there are
     * more values to consider in the domain of the first parameter, in which
     * case the iterator over these values is incremented, and the next binding
     * can be considered.  If this is not the case, then the next parameter
     * <code>x</code> with remaining values is determined and
     * <code>rollover(x)</code> is called.
     *  @see rollover
     */
    void next() {
      static const bool debug = false;

      static int x;
      x = 0;
      ++(valItrs[x]);

      if (debug) {
        if (valItrs[x] != valEnds[x]) {
          cout << "Still more options for parameter " << (x + 1) << endl;
        }
      }

      while (valItrs[x] == valEnds[x]) {
        if (debug) cout << "Finished options for parameter " << (x + 1) << endl;
        ++x;
        if (x == varCount) {
          if (debug) cout << "No more parameter options left, bailing out\n";
          valid = false;
          return;
        }
        for (int ps = 0; ps < x; ++ps) {
          if (popSet[x][ps]) {
            popSet[x][ps] = false;
            if (allowedValuesForParameter[ps].front().second == needsDeleting) {
              delete allowedValuesForParameter[ps].front().first;
            }
            allowedValuesForParameter[ps].pop_front();
          }
        }
        ++valItrs[x];
      }
      if (x) {
        valid = rollover(x);
        if (debug) {
          if (valid) {
            cout << "More options left, returning true\n";
          } else {
            cout << "No more options left, returning false\n";
          }
        }
      } else {
        if (debug) cout << "More options left, returning true\n";
        valid = true;
      }
    }
  };

  PDCIterator *ParameterDomainConstraints::getIterator() {
    return new PDCIterator(this);
  }

  void instantiatedOp::instantiate(const VAL::operator_ *op,
                                   const VAL::problem *prb,
                                   VAL::TypeChecker &tc) {
    FastEnvironment e(
        static_cast< const id_var_symbol_table * >(op->symtab)->numSyms());

    const int opParamCount = op->parameters->size();

    OperatorParameterDomainConstraints pdc(op, tc);

#ifndef NDEBUG
    if (insistOnOp && insistOnOp == op) {
      for (int p = 0; p < opParamCount; ++p) {
        vector< VAL::const_symbol * >::const_iterator pItr =
            pdc.getPossibleParameterValues()[p].begin();
        const vector< VAL::const_symbol * >::const_iterator pEnd =
            pdc.getPossibleParameterValues()[p].end();

        for (; pItr != pEnd; ++pItr) {
          if (*pItr == insistOnOpParameters[p]) {
            std::cout << "\tFound appropriate possibility for parameter "
                      << (p + 1) << std::endl;
            break;
          }
        }

        if (pItr == pEnd) {
          std::cout
              << "Error during instantiating the named operator: parameter "
              << p + 1 << " cannot be the desired value\n";
          exit(1);
        }
      }
    }
#endif

    // cout << c << " candidates to consider\n";
    SimpleEvaluator se(&tc, 0, ISC());
    if (!opParamCount) {
      se.prepareForVisit(&e);
      op->visit(&se);
      if (!se.reallyFalse()) {
        FastEnvironment *ecpy = e.copy();
        instantiatedOp *o = new instantiatedOp(op, ecpy);
        if (instOps.insert(o)) {
          delete o;
        };
      };
      return;
    };

    vector< VAL::var_symbol * > vars(opParamCount);

    {
      int i = 0;
      var_symbol_list::const_iterator p = op->parameters->begin();
      var_symbol_list::const_iterator pEnd = op->parameters->end();

      for (; p != pEnd; ++p, ++i) {
        vars[i] = *p;
      }
    }

    std::unique_ptr< PDCIterator > options(pdc.getIterator());

    while (options->isValid()) {
      for (int x = 0; x < opParamCount; ++x) {
        e[vars[x]] = (*options)[x];
      }
      if (!TIM::selfMutex(op, makeIterator(&e, op->parameters->begin()),
                          makeIterator(&e, op->parameters->end()))) {
        se.prepareForVisit(&e);
        const_cast< VAL::operator_ * >(op)->visit(&se);
        if (!se.reallyFalse()) {
          FastEnvironment *ecpy = e.copy();
          instantiatedOp *o = new instantiatedOp(op, ecpy);
          if (instOps.insert(o)) {
            delete o;
          };
        }
#ifndef NDEBUG
        else if (insistOnOp && insistOnOp == op) {
          bool allMatched = true;
          int p = 0;
          for (var_symbol_list::const_iterator a = op->parameters->begin();
               a != op->parameters->end(); ++a, ++p) {
            if (e[*a] != insistOnOpParameters[p]) {
              allMatched = false;
              break;
            }
          }

          if (allMatched) {
            cout << "Killed\n" << op->name->getName() << "(";
            for (var_symbol_list::const_iterator a = op->parameters->begin();
                 a != op->parameters->end(); ++a) {
              cout << e[*a]->getName() << " ";
            };
            cout << ") - preconditions evaluated to false\n";
            exit(1);
          }
        }
#endif

      }
#ifndef NDEBUG
      else if (insistOnOp && insistOnOp == op) {
        bool allMatched = true;
        int p = 0;
        for (var_symbol_list::const_iterator a = op->parameters->begin();
             a != op->parameters->end(); ++a, ++p) {
          if (e[*a] != insistOnOpParameters[p]) {
            allMatched = false;
            break;
          }
        }

        if (allMatched) {
          cout << "Decided that (" << op->name->getName();
          for (var_symbol_list::const_iterator a = op->parameters->begin();
               a != op->parameters->end(); ++a) {
            cout << " " << e[*a]->getName();
          };
          cout << ") was self mutex\n";

          exit(1);
        }
      }
#endif

      options->next();
    }
  };

  void instantiatedDrv::instantiate(const VAL::derivation_rule *op,
                                    const VAL::problem *prb,
                                    VAL::TypeChecker &tc) {
    FastEnvironment e(
        static_cast< const id_var_symbol_table * >(op->get_vars())->numSyms());

    const int opParamCount = op->get_head()->args->size();

    DerivationRuleParameterDomainConstraints pdc(op, tc);

    // cout << c << " candidates to consider\n";
    SimpleEvaluator se(&tc, &e, ISC());
    if (!opParamCount) {
      se.prepareForVisit(&e);
      op->visit(&se);
      if (!se.reallyFalse()) {
        FastEnvironment *ecpy = e.copy();
        instantiatedDrv *o = new instantiatedDrv(op, ecpy);
        if (instDrvs.insert(o)) {
          delete o;
        };
      };
      return;
    };

    vector< VAL::parameter_symbol * > vars(opParamCount);

    {
      int i = 0;
      parameter_symbol_list::const_iterator p = op->get_head()->args->begin();
      parameter_symbol_list::const_iterator pEnd = op->get_head()->args->end();

      for (; p != pEnd; ++p, ++i) {
        vars[i] = *p;
      }
    }

    std::unique_ptr< PDCIterator > options(pdc.getIterator());

    while (options->isValid()) {
      for (int x = 0; x < opParamCount; ++x) {
        e[vars[x]] = (*options)[x];
      }

      {
        se.prepareForVisit(&e);
        const_cast< VAL::derivation_rule * >(op)->visit(&se);
        if (!se.reallyFalse()) {
          FastEnvironment *ecpy = e.copy();
          instantiatedDrv *o = new instantiatedDrv(op, ecpy);

          if (instDrvs.insert(o)) {
            delete o;
          }
        };
      };

      options->next();
    };
  };

  LiteralStore instantiatedLiterals;
  PNEStore instantiatedPNEs;

  LiteralStore &instantiatedOp::literals = instantiatedLiterals;
  PNEStore &instantiatedOp::pnes = instantiatedPNEs;

  LiteralStore &instantiatedDrv::literals = instantiatedLiterals;
  PNEStore &instantiatedDrv::pnes = instantiatedPNEs;

  class Collector : public VisitController {
   private:
    VAL::TypeChecker *tc;
    bool adding;
    const VAL::operator_ *op;
    const VAL::derivation_rule *drv;
    FastEnvironment *fe;
    LiteralStore &literals;
    PNEStore &pnes;

    bool inpres;
    bool checkpos;
    bool onlyCollectEffects;

   public:
    Collector(const VAL::operator_ *o, FastEnvironment *f, LiteralStore &l,
              PNEStore &p, VAL::TypeChecker *t = 0)
        : tc(t),
          adding(true),
          op(o),
          drv(0),
          fe(f),
          literals(l),
          pnes(p),
          inpres(true),
          checkpos(true),
          onlyCollectEffects(true){};

    Collector(const VAL::derivation_rule *o, FastEnvironment *f,
              LiteralStore &l, PNEStore &p, VAL::TypeChecker *t = 0)
        : tc(t),
          adding(true),
          op(0),
          drv(o),
          fe(f),
          literals(l),
          pnes(p),
          inpres(true),
          checkpos(true),
          onlyCollectEffects(true){};

    virtual void visit_simple_goal(simple_goal *p) {
      if (onlyCollectEffects) return;
      VAL::extended_pred_symbol *s = EPS(p->getProp()->head);

      if (VAL::current_analysis->pred_tab.symbol_probe("=") == s->getParent()) {
        return;
      };
      if (!inpres || (p->getPolarity() && !checkpos) ||
          (!p->getPolarity() && checkpos)) {
        Literal *l = new Literal(p->getProp(), fe);
        if (literals.insert(l)) {
          delete l;
        } else {
          //				cout << "Created " << *l << "\n";
        };
      };
    };
    virtual void visit_qfied_goal(qfied_goal *p) {
      vector< vector< VAL::const_symbol * >::const_iterator > vals(
          p->getVars()->size());
      vector< vector< VAL::const_symbol * >::const_iterator > starts(
          p->getVars()->size());
      vector< vector< VAL::const_symbol * >::const_iterator > ends(
          p->getVars()->size());
      vector< VAL::var_symbol * > vars(p->getVars()->size());
      fe->extend(vars.size());
      int i = 0;
      int c = 1;
      for (var_symbol_list::const_iterator pi = p->getVars()->begin();
           pi != p->getVars()->end(); ++pi, ++i) {
        if (instantiatedOp::values.find((*pi)->type) ==
            instantiatedOp::values.end()) {
          instantiatedOp::values[(*pi)->type] = tc->range(*pi);
        };
        vals[i] = starts[i] = instantiatedOp::values[(*pi)->type].begin();
        ends[i] = instantiatedOp::values[(*pi)->type].end();
        if (ends[i] == starts[i]) return;
        (*fe)[(*pi)] = *(vals[i]);
        vars[i] = *pi;
        c *= instantiatedOp::values[(*pi)->type].size();
      };

      --i;
      while (vals[i] != ends[i]) {
        // This is inefficient because it creates a copy of the environment even
        // if the copy is never used. In practice, this should not be a problem
        // because a quantified effect presumably uses the variables it
        // quantifies.
        FastEnvironment *ecpy = fe;
        fe = fe->copy();
        p->getGoal()->visit(this);
        fe = ecpy;

        int x = 0;
        ++vals[0];
        if (vals[0] != ends[0]) (*fe)[vars[0]] = *(vals[0]);
        while (x < i && vals[x] == ends[x]) {
          vals[x] = starts[x];
          (*fe)[vars[x]] = *(vals[x]);
          ++x;
          ++vals[x];
          if (vals[x] != ends[x]) (*fe)[vars[x]] = *(vals[x]);
        };
      };
    };

    virtual void visit_conj_goal(conj_goal *p) { p->getGoals()->visit(this); };
    virtual void visit_disj_goal(disj_goal *p) { p->getGoals()->visit(this); };
    virtual void visit_timed_goal(timed_goal *p) { p->getGoal()->visit(this); };
    virtual void visit_imply_goal(imply_goal *p) {
      p->getAntecedent()->visit(this);
      p->getConsequent()->visit(this);
    };
    virtual void visit_neg_goal(neg_goal *p) {
      bool oldcheck = checkpos;
      checkpos = !checkpos;
      p->getGoal()->visit(this);
      checkpos = oldcheck;
    };
    virtual void visit_preference(preference *p) { p->getGoal()->visit(this); };
    virtual void visit_simple_effect(simple_effect *p) {
      if (!adding && onlyCollectEffects) return;
      Literal *l = new Literal(p->prop, fe);
      if (literals.insert(l)) {
        delete l;
      };
    };
    virtual void visit_constraint_goal(constraint_goal *cg) {
      if (cg->getRequirement()) {
        cg->getRequirement()->visit(this);
      };
      if (cg->getTrigger()) {
        cg->getTrigger()->visit(this);
      };
    };
    virtual void visit_named_goal(named_goal *ng) { ng->gl->visit(this); }

    virtual void visit_forall_effect(forall_effect *p) {
      //		p->getEffects()->visit(this);
      vector< vector< VAL::const_symbol * >::const_iterator > vals(
          p->getVarsList()->size());
      vector< vector< VAL::const_symbol * >::const_iterator > starts(
          p->getVarsList()->size());
      vector< vector< VAL::const_symbol * >::const_iterator > ends(
          p->getVarsList()->size());
      vector< VAL::var_symbol * > vars(p->getVarsList()->size());
      fe->extend(vars.size());
      int i = 0;
      int c = 1;
      for (var_symbol_list::const_iterator pi = p->getVarsList()->begin();
           pi != p->getVarsList()->end(); ++pi, ++i) {
        if (instantiatedOp::values.find((*pi)->type) ==
            instantiatedOp::values.end()) {
          instantiatedOp::values[(*pi)->type] = tc->range(*pi);
        };
        vals[i] = starts[i] = instantiatedOp::values[(*pi)->type].begin();
        ends[i] = instantiatedOp::values[(*pi)->type].end();
        if (ends[i] == starts[i]) return;
        (*fe)[(*pi)] = *(vals[i]);
        vars[i] = *pi;
        c *= instantiatedOp::values[(*pi)->type].size();
      };

      --i;
      while (vals[i] != ends[i]) {
        // This is inefficient because it creates a copy of the environment even
        // if the copy is never used. In practice, this should not be a problem
        // because a quantified effect presumably uses the variables it
        // quantifies.
        FastEnvironment *ecpy = fe;
        fe = fe->copy();
        p->getEffects()->visit(this);
        fe = ecpy;

        int x = 0;
        ++vals[0];
        if (vals[0] != ends[0]) (*fe)[vars[0]] = *(vals[0]);
        while (x < i && vals[x] == ends[x]) {
          vals[x] = starts[x];
          (*fe)[vars[x]] = *(vals[x]);
          ++x;
          ++vals[x];
          if (vals[x] != ends[x]) (*fe)[vars[x]] = *(vals[x]);
        };
      };

      // Ends here!
    };
    virtual void visit_cond_effect(cond_effect *p) {
      p->getCondition()->visit(this);
      p->getEffects()->visit(this);
    };
    virtual void visit_timed_effect(timed_effect *p) { p->effs->visit(this); };
    virtual void visit_timed_initial_literal(timed_initial_literal *p) {
      p->effs->visit(this);
    };
    virtual void visit_effect_lists(effect_lists *p) {
      p->add_effects.pc_list< simple_effect * >::visit(this);
      p->forall_effects.pc_list< forall_effect * >::visit(this);
      p->cond_effects.pc_list< cond_effect * >::visit(this);
      p->timed_effects.pc_list< timed_effect * >::visit(this);
      bool whatwas = adding;
      adding = !adding;
      p->del_effects.pc_list< simple_effect * >::visit(this);
      adding = whatwas;
      p->assign_effects.pc_list< assignment * >::visit(this);
    };
    virtual void visit_operator_(VAL::operator_ *p) {
      inpres = true;
      checkpos = true;
      if (p->precondition) p->precondition->visit(this);
      inpres = false;

      adding = true;
      p->effects->visit(this);
    };

    virtual void visit_derivation_rule(VAL::derivation_rule *p) {
      inpres = true;
      checkpos = true;
      if (p->get_body()) p->get_body()->visit(this);
      inpres = false;

      adding = true;
      Literal *l = new Literal(p->get_head(), fe);
      if (literals.insert(l)) {
        delete l;
      } else {
        //			cout << "Created " << *l << "\n";
      }
    };

    virtual void visit_action(VAL::action *p) {
      visit_operator_(p);  // static_cast<VAL::operator_*>(p));
    };
    virtual void visit_durative_action(VAL::durative_action *p) {
      visit_operator_(p);  // static_cast<VAL::operator_*>(p));
    };
    virtual void visit_process(VAL::process *p) { visit_operator_(p); };
    virtual void visit_event(VAL::event *p) { visit_operator_(p); };
    virtual void visit_problem(VAL::problem *p) {
      p->initial_state->visit(this);
      inpres = false;
      p->the_goal->visit(this);
      if (p->constraints) p->constraints->visit(this);
    };

    virtual void visit_assignment(assignment *a) {
      const func_term *ft = a->getFTerm();
      PNE *pne = new PNE(ft, fe);
      if (pnes.insert(pne)) {
        delete pne;
      };
    };
  };

  void instantiatedOp::createAllLiterals(VAL::problem *p,
                                         VAL::TypeChecker *tc) {
    literals.clear();
    assert(!howManyLiteralsOfAnySort());
    Collector c((VAL::operator_ *)0, 0, literals, pnes, tc);
    p->visit(&c);
    for (OpStore::iterator i = instOps.begin(); i != instOps.end(); ++i) {
      (*i)->collectLiterals(tc);
    };

    instantiatedDrv::createAllLiterals(p, tc);
  };

  void instantiatedOp::collectLiterals(VAL::TypeChecker *tc) {
    Collector c(op, env, literals, pnes, tc);
    op->visit(&c);
  };

  void instantiatedDrv::createAllLiterals(VAL::problem *,
                                          VAL::TypeChecker *tc) {
    for (DrvStore::iterator i = instDrvs.begin(); i != instDrvs.end(); ++i) {
      (*i)->collectLiterals(tc);
    };
  };

  void instantiatedDrv::collectLiterals(VAL::TypeChecker *tc) {
    //	cout << "Collecting literals for "; write(cout); cout << "\n";
    Collector c(op, env, literals, pnes, tc);
    op->visit(&c);
  };

  void instantiatedOp::writeAllLiterals(ostream &o) { literals.write(o); };

  void instantiatedOp::writeAllPNEs(ostream &o) { pnes.write(o); };

  VAL::const_symbol *const getConst(string name) {
    return current_analysis->const_tab.symbol_get(name);
  };

  VAL::const_symbol *const getConst(const char *name) {
    return current_analysis->const_tab.symbol_get(name);
  };

  // Added by AMC to test whether a goal may be satisfied by the effects
  // of an InstantiatedOp

  bool instantiatedOp::isGoalMetByOp(const Literal *lit) {
    effect_lists *effs = op->effects;

    return isGoalMetByEffect(effs, lit);
  };

  bool instantiatedOp::isGoalMetByEffect(const VAL::effect_lists *effs,
                                         const Literal *lit) {
    using VAL::pc_list;

    for (pc_list< VAL::simple_effect * >::const_iterator i =
             effs->add_effects.begin();
         i != effs->add_effects.end(); ++i) {
      if (isGoalMetByEffect(*i, lit)) return true;
    };
    for (pc_list< VAL::forall_effect * >::const_iterator i =
             effs->forall_effects.begin();
         i != effs->forall_effects.end(); ++i) {
      if (isGoalMetByEffect(*i, lit)) return true;
    };
    for (pc_list< VAL::cond_effect * >::const_iterator i =
             effs->cond_effects.begin();
         i != effs->cond_effects.end(); ++i) {
      if (isGoalMetByEffect(*i, lit)) return true;
    };
    for (pc_list< VAL::cond_effect * >::const_iterator i =
             effs->cond_effects.begin();
         i != effs->cond_effects.end(); ++i) {
      if (isGoalMetByEffect(*i, lit)) return true;
    };
    for (pc_list< VAL::timed_effect * >::const_iterator i =
             effs->timed_effects.begin();
         i != effs->timed_effects.end(); ++i) {
      if (isGoalMetByEffect(*i, lit)) return true;
    };
    return false;
  };

  bool instantiatedOp::isGoalMetByEffect(VAL::simple_effect *seff,
                                         const Literal *lit) {
    Literal l(seff->prop, env);
    Literal *lt = instantiatedOp::getLiteral(&l);
    //  std::cout <<"Simple effect: " << (*lt) << "\n";
    return (lit == lt);
  };

  bool instantiatedOp::isGoalMetByEffect(VAL::forall_effect *fleff,
                                         const Literal *lit) {
    if (isGoalMetByEffect(fleff->getEffects(), lit))
      return true;
    else
      return false;
  };

  bool instantiatedOp::isGoalMetByEffect(VAL::cond_effect *ceff,
                                         const Literal *lit) {
    if (isGoalMetByEffect(ceff->getEffects(), lit))
      return true;
    else
      return false;
  };

  bool instantiatedOp::isGoalMetByEffect(VAL::timed_effect *teff,
                                         const Literal *lit) {
    if (isGoalMetByEffect(teff->effs, lit))
      return true;
    else
      return false;
  };

  instantiatedOp::PropEffectsIterator instantiatedOp::addEffectsBegin() {
    return PropEffectsIterator(this, true);
  };

  instantiatedOp::PropEffectsIterator instantiatedOp::addEffectsEnd() {
    PropEffectsIterator pi(this, true);
    pi.toEnd();
    return pi;
  };

  instantiatedOp::PropEffectsIterator instantiatedOp::delEffectsBegin() {
    return PropEffectsIterator(this, false);
  };

  instantiatedOp::PropEffectsIterator instantiatedOp::delEffectsEnd() {
    PropEffectsIterator pi(this, false);
    pi.toEnd();
    return pi;
  };

  instantiatedOp::PNEEffectsIterator instantiatedOp::PNEEffectsBegin() {
    return PNEEffectsIterator(this);
  };

  instantiatedOp::PNEEffectsIterator instantiatedOp::PNEEffectsEnd() {
    PNEEffectsIterator pi(this);
    pi.toEnd();
    return pi;
  };

  // New code to give unique identifiers to non-static facts and functions

  int instantiatedOp::nonStaticLiteralCount = 0;
  int instantiatedOp::nonStaticPNECount = 0;

  bool instantiatedOp::staticFactsAndLiteralsHaveBeenGivenIDs = false;

  void instantiatedOp::assignStateIDsToNonStaticLiteralsAndPNEs() {
    nonStaticLiteralCount = 0;
    nonStaticPNECount = 0;
    {
      LiteralStore::iterator lsItr = instantiatedOp::literalsBegin();
      const LiteralStore::iterator lsEnd = instantiatedOp::literalsEnd();

      for (; lsItr != lsEnd; ++lsItr) {
        holding_pred_symbol *const hps = EPS((*lsItr)->getHead())->getParent();

        bool hpsIsStatic = true;

        {
          holding_pred_symbol::PIt epsItr = hps->pBegin();
          const holding_pred_symbol::PIt epsEnd = hps->pEnd();

          for (; epsItr != epsEnd; ++epsItr) {
            if (!(*epsItr)->appearsStatic()) {
              hpsIsStatic = false;
              break;
            }
          }
        }

        if (!hpsIsStatic) {
          (*lsItr)->setStateID(nonStaticLiteralCount);
          ++nonStaticLiteralCount;
        }
      }
    }

    {
      PNEStore::iterator psItr = instantiatedOp::pnesBegin();
      const PNEStore::iterator psEnd = instantiatedOp::pnesEnd();

      for (; psItr != psEnd; ++psItr) {
        if (!EFT((*psItr)->getHead())->isStatic()) {
          (*psItr)->setStateID(nonStaticPNECount);
          ++nonStaticPNECount;
        }
      }
    }

    staticFactsAndLiteralsHaveBeenGivenIDs = true;
  }

};  // namespace Inst

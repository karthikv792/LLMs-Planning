// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#include <iostream>
#include <map>
#include <utility>
#include <vector>
//#include "Proposition.h"

#ifndef __POLYNOMIAL
#define __POLYNOMIAL

using std::make_pair;
using std::map;
using std::ostream;
using std::pair;
using std::vector;

//#define vector std::vector
namespace VAL {

  class Polynomial;
  class Comparison;
  class State;

  // typedef float CoScalar;
  typedef long double CoScalar;

  typedef pair< CoScalar, bool > intervalEnd;
  // value of interval end point, bool - true = closed, false = open

  struct Intervals {
    vector< pair< intervalEnd, intervalEnd > > intervals;

    Intervals(){};
    Intervals(bool b);
    Intervals(vector< pair< intervalEnd, intervalEnd > > ints);
    ~Intervals();

    bool operator==(const Intervals& ints) const;

    void write(ostream& o) const;
    void writeOffset(double t) const;
  };

  ostream& operator<<(ostream& o, const Intervals& i);

  class CtsFunction {
   public:
    CtsFunction(){};

    virtual ~CtsFunction(){};

    virtual vector< CoScalar > getRoots(CoScalar t) const = 0;  // on
                                                                // interval(0,t)
    // within accuracy
    virtual Polynomial getApproxPoly(CoScalar endInt) const = 0;
    virtual pair< vector< pair< intervalEnd, intervalEnd > >,
                  vector< CoScalar > >
        isolateRoots(CoScalar t, CoScalar accuracy) const = 0;
    virtual CoScalar evaluate(CoScalar t) const = 0;
    virtual bool isLinear() const = 0;
    virtual void write(ostream& o) const = 0;
    virtual Intervals getIntervals(const Comparison* comp, const State* s,
                                   CoScalar t) const = 0;
    virtual bool checkInvariant(const Comparison* comp, const State* s,
                                CoScalar t, bool rhsIntervalOpen) const = 0;
  };

  ostream& operator<<(ostream& o, const CtsFunction& cf);

  class Polynomial : public CtsFunction {
   private:
    map< unsigned int, CoScalar > coeffs;

   public:
    Polynomial(){};
    Polynomial(map< unsigned int, CoScalar > c) : coeffs(c){};

    ~Polynomial();

    static const CoScalar tooSmall;  // regarded as zero or of unknown sign if
                                     // abs
                                     // value less than this
    static CoScalar accuracy;        // accuracy to calculate poly roots

    CoScalar getCoeff(unsigned int pow) const;
    void setCoeff(unsigned int pow, CoScalar value);
    void addToCoeff(unsigned int pow, CoScalar value);
    unsigned int getDegree() const;

    Polynomial getApproxPoly(CoScalar endInt) const { return *this; };
    vector< CoScalar > getRoots(CoScalar t) const;  // on interval(0,t) within
                                                    // accuracy
    Intervals getIntervals(const Comparison* comp, const State* s,
                           CoScalar t) const;
    bool checkInvariant(const Comparison* comp, const State* s, CoScalar t,
                        bool rhsIntervalOpen) const;

    static void setAccuracy(CoScalar ac) { accuracy = ac / 10; };
    pair< Polynomial, Polynomial > divide(const Polynomial& d) const;
    Polynomial getGCD(const Polynomial& p) const;
    Polynomial removeRepeatedRoots() const;
    bool checkPolynomialCoeffs() const;
    void removeSmallCoeffs();
    pair< vector< pair< intervalEnd, intervalEnd > >, vector< CoScalar > >
        isolateRoots(CoScalar t, CoScalar accuracy = 0) const;
    bool rootsExist(CoScalar t) const;  // on open interval (0,t)

    Polynomial& operator+=(const Polynomial& p);
    Polynomial& operator+=(CoScalar num);
    Polynomial& operator-=(const Polynomial& p);
    Polynomial& operator-=(CoScalar num);
    Polynomial& operator*=(const Polynomial& p);
    Polynomial& operator*=(CoScalar num);
    bool operator==(const Polynomial& p) const;

    CoScalar evaluate(CoScalar t) const;

    bool isLinear() const;
    Polynomial diff() const;
    Polynomial integrate() const;

    void write(ostream& o) const;
  };

  ostream& operator<<(ostream& o, const Polynomial& p);

  class Exponential : public CtsFunction {
   private:
    CoScalar K, c;  // f(t) = K e^(poly) + c
    const Polynomial* poly;
    mutable CoScalar offSet;  // for comparisons of type y > k, where y is FE
                              // and
                              // k is offset
   public:
    Exponential() : K(1), c(0), poly(0), offSet(0){};
    Exponential(CoScalar K0, const Polynomial* p, CoScalar c0)
        : K(K0), c(c0), poly(p), offSet(0){};

    ~Exponential() { delete poly; };

    Polynomial getApproxPoly(CoScalar endInt) const;
    void setOffSet(double off) { offSet = off; };
    vector< CoScalar > getRoots(CoScalar t) const;  // on interval(0,t) within
                                                    // accuracy
    pair< vector< pair< intervalEnd, intervalEnd > >, vector< CoScalar > >
        isolateRoots(CoScalar t, CoScalar accuracy) const;
    CoScalar evaluate(CoScalar t) const;
    bool isLinear() const;
    const Polynomial* getPolynomial() const { return poly; };
    Intervals getIntervals(const Comparison* comp, const State* s,
                           CoScalar t) const;
    bool checkInvariant(const Comparison* comp, const State* s, CoScalar t,
                        bool rhsIntervalOpen) const;
    bool rootsExist(CoScalar t) const;  // on open interval (0,t)
    CoScalar getK() const { return K; };
    CoScalar getc() const { return c; };
    unsigned int getNoTerms(CoScalar endInt) const;  // number of terms to take
                                                     // from power series for
                                                     // approx poly

    void write(ostream& o) const;
  };

  // only subclasses instances of this class to be actually used
  class NumericalSolution : public CtsFunction {
   protected:
    map< double, CoScalar > points;  // time value maps to value of the function
    mutable CoScalar offSet;  // for comparisons of type y > k, where y is FE
                              // and
                              // k is offset
   public:
    NumericalSolution(){};
    NumericalSolution(map< double, CoScalar > pts, CoScalar off)
        : points(pts), offSet(off){};

    virtual ~NumericalSolution(){};
    void buildPoints(CoScalar a0, CoScalar b0, CoScalar y,
                     CoScalar accuracy);  // initial value given at time t0
    void setOffSet(double off) { offSet = off; };
    double getOffSet() const { return offSet; };
    map< double, CoScalar > getPoints() const { return points; };
    Intervals getIntervals(const Comparison* comp, const State* s,
                           CoScalar t) const;
    bool checkInvariant(const Comparison* comp, const State* s, CoScalar t,
                        bool rhsIntervalOpen) const;
    vector< CoScalar > getRoots(CoScalar t) const;  // on interval(0,t) within
                                                    // accuracy
    Polynomial getApproxPoly(CoScalar endInt) const;
    pair< vector< pair< intervalEnd, intervalEnd > >, vector< CoScalar > >
        isolateRoots(CoScalar t, CoScalar accuracy) const;
    CoScalar evaluate(CoScalar t) const;
    bool isLinear() const;
    virtual CoScalar evaluateDiff(CoScalar t, CoScalar y) = 0;
  };

  class BatteryCharge : public NumericalSolution {
    // for diff eqn:   dy/dt = p(t) (m - y) - k(t),   for some poly p(t) and
    // some
    // constant m and discharge k(t)
    const Polynomial* poly;
    CoScalar m;
    vector< pair< const CtsFunction*, bool > > discharge;

   public:
    BatteryCharge() : poly(0), m(0), discharge(){};
    BatteryCharge(const Polynomial* p, CoScalar m0,
                  vector< pair< const CtsFunction*, bool > > d, CoScalar t0,
                  CoScalar t1, CoScalar y0, CoScalar accuracy)
        : poly(p), m(m0), discharge(d) {
      offSet = 0;
      buildPoints(t0, t1, y0, accuracy);
    };

    BatteryCharge(CoScalar off, map< double, CoScalar > pts,
                  const Polynomial* p, CoScalar m0,
                  vector< pair< const CtsFunction*, bool > > d)
        : NumericalSolution(pts, off), poly(p), m(m0), discharge(d){};

    ~BatteryCharge(){
        // if(poly != 0) delete poly;
        //    for(vector<pair<const CtsFunction *,bool> >::const_iterator i =
        //    discharge.begin(); i != discharge.end(); ++i)
        // {
        //    delete i->first;
        // };
    };
    CoScalar evaluateDiff(CoScalar t, CoScalar y);

    void write(ostream& o) const;
  };

  Polynomial operator+(const Polynomial& p, const Polynomial& q);
  Polynomial operator+(CoScalar num, const Polynomial& p);
  Polynomial operator+(const Polynomial& p, CoScalar num);

  Polynomial operator-(const Polynomial& p, const Polynomial& q);
  Polynomial operator-(CoScalar num, const Polynomial& p);
  Polynomial operator-(const Polynomial& p, CoScalar num);

  Polynomial operator*(const Polynomial& p, const Polynomial& q);
  Polynomial operator*(CoScalar num, const Polynomial& p);
  Polynomial operator*(const Polynomial& p, CoScalar num);

  Polynomial operator-(const Polynomial& p);
  Polynomial operator/(const Polynomial& p, CoScalar num);

};  // namespace VAL

#endif

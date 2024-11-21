// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

#include "valLib.h"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

using namespace std;

void assertFV(void *vld, vector< string > &wds, double v) {
  LPCSTR *wdsC = new LPCSTR[wds.size()];
  for (int i = 0; i < wds.size(); ++i) {
    wdsC[i] = wds[i].c_str();
  }
  unsigned long x = getFunCode(vld, wdsC);
  assertFunValue(vld, x, v);
  delete[] wdsC;
}

void assertLV(void *vld, vector< string > &wds, bool b) {
  LPCSTR *wdsC = new LPCSTR[wds.size()];
  for (int i = 0; i < wds.size(); ++i) {
    wdsC[i] = wds[i].c_str();
  }
  unsigned long x = getLitCode(vld, wdsC);
  assertLitValue(vld, x, b);
  delete[] wdsC;
}

int main(int argc, char *argv[]) {
  ifstream script(argv[1]);
  // void * vld = makeValidatorFromFiles(argv[1],argv[2],0.001);
  int aID = 1;
  void *vld = 0;
  string domain, problem, planner;
  stringstream ss;
  stringstream out;
  stringstream theplans;
  stringstream newObjects;
  stringstream extInit;
  bool newProb = false;

  while (true) {
    string s;
    script >> s;
    if (s == "q") {
      break;
    }

    if (s == "plan") {
      script >> planner >> domain >> problem;
      ostringstream cline;
      cline << planner << " " << domain << " " << problem << " > plantmpfile";
      cout << "Planning " << domain << "/" << problem << "\n";
      system(cline.str().c_str());
      cout << "Completed planning\n";
      ifstream plan("plantmpfile");
      while (!plan.eof()) {
        string s;
        getline(plan, s);
        if (s[0] != ';') {
          theplans << s << "\n";
        }
      }
      plan.close();
      // Got the plan
    } else if (s == "executeto") {
      newProb = false;
      script >> s;
      ss.str("");
      if (s == "end") {
        cout << "Reading plan\n";
        system("PlanToVALstep plantmpfile > tmpscript");
        // if(vld) cleanUp(vld);
        cout << "Ready to use script\n";
        cout << "Looking for files " << domain.c_str() << " and "
             << problem.c_str() << "\n";
        vld = makeValidatorFromFiles(domain.c_str(), problem.c_str(), 0.001);
        ifstream newscript("tmpscript");
        cout << "Executing all of plan\n";
        aID = 1;
        while (!newscript.eof()) {
          vector< string > wds;
          newscript >> s;
          if (s == "x") {
            // cout << "Executing one\n";
            executeNext(vld);
            /*
            int n;
            unsigned long * vals = getChangedLits(vld,n);
            cout << "Seeing " << n << " changed lits\n";
            for(int i = 0;i < n;++i)
            {
                int x;
                LPCSTR * nm = whatLit(vld,vals[i],x);
                for(int j = 0;j < x;++j)
                {
                    cout << nm[j] << " ";
                }
                cout << "- now " <<
            (queryLiteralCode(vld,vals[i])?"true":"false") << "\n";
                cleanName(nm);
            }

            cleanStore(vals);
            vals = getChangedFuns(vld, n);
            for(int i = 0;i < n;++i)
            {
                int x;
                LPCSTR * nm = whatFun(vld,vals[i],x);
                for(int j = 0;j < x;++j)
                {
                    cout << nm[j] << " ";
                }
                cout << "- now " << (queryValueCode(vld,vals[i])) << "\n";
                cleanName(nm);
            }
            cleanStore(vals);
            */
          } else if (s == "end") {
            int a;
            newscript >> a;
            newscript >> s;
            double t;
            newscript >> t;
            post(vld, a, 0, false, t);
          } else {
            while (true) {
              newscript >> s;
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
            newscript >> t;
            post(vld, aID, act, true, t);
            delete[] act;
            ++aID;
          }
        }
      } else if (s == "startof") {
        vector< string > wds1;
        stringstream xx;
        while (true) {
          script >> s;
          if (s.find("(", 0) == 0) {
            s.erase(0, 1);
          }

          if (s.find(")") != string::npos) {
            s.erase(s.find(")"), 1);
            wds1.push_back(s);
            xx << s;
            break;
          } else {
            xx << s << " ";
            wds1.push_back(s);
          }
        }

        cout << "Reading plan\n";
        system("PlanToVALstep plantmpfile > tmpscript");
        // if(vld) cleanUp(vld);
        cout << "Ready to use script\n";
        cout << "Looking for files " << domain.c_str() << " and "
             << problem.c_str() << "\n";
        vld = makeValidatorFromFiles(domain.c_str(), problem.c_str(), 0.001);
        ifstream newscript("tmpscript");
        cout << "Executing to start of " << xx.str() << "\n";
        aID = 1;
        bool matcher = false;
        while (!newscript.eof()) {
          vector< string > wds;
          newscript >> s;

          if (s == "x") {
            executeNext(vld);
            if (matcher) break;
          } else if (s == "end") {
            int a;
            newscript >> a;
            newscript >> s;
            double t;
            newscript >> t;
            post(vld, a, 0, false, t);
          } else {
            while (true) {
              newscript >> s;
              if (s == "@") {
                break;
              }
              wds.push_back(s);
            }
            LPCSTR *act = new LPCSTR[wds.size()];
            matcher = true;
            for (unsigned int i = 0; i < wds.size(); ++i) {
              act[i] = wds[i].c_str();

              matcher &= i < wds1.size() && (wds[i] == wds1[i]);
            }

            double t;
            newscript >> t;
            post(vld, aID, act, true, t);
            delete[] act;
            ++aID;
          }
        }
      }
    } else if (s == "extractpredicates[") {
      vector< string > wds;
      while (true) {
        script >> s;
        if (s == "]") {
          // cout << "State is:\n" << ss.str() << "\n";
          break;
        }
        if (s.find("(", 0) == 0) {
          s.erase(0, 1);
        }

        if (s.find(")") != string::npos) {
          s.erase(s.find(")"), 1);
          wds.push_back(s);
          // cout << "Test " << wds[0] << "\n";
          LPCSTR *tmplit = new LPCSTR[wds.size()];
          for (int i = 0; i < wds.size(); ++i) {
            tmplit[i] = wds[i].c_str();
          }
          if (queryLiteralNamed(vld, tmplit)) {
            ss << "    (";
            for (int i = 0; i < wds.size(); ++i) {
              ss << wds[i];
              if (i < wds.size() - 1) ss << " ";
            }
            ss << ")\n";
          }
          delete[] tmplit;
          wds.clear();
        } else {
          wds.push_back(s);
        }
      }
    } else if (s == "prepareNewProblem") {
      newObjects.str("");
      extInit.str("");
      newProb = true;
    } else if (s == "assert[") {
      vector< string > wdsloc;
      while (true) {
        script >> s;
        // cout << "Reading " << s << "\n";
        if (s == "]") {
          break;
        }
        if (s == "(=") {
          wdsloc.clear();
          script >> s;
          while (s.find(")") == string::npos) {
            if (s.find("(") != string::npos) {
              wdsloc.push_back(s.substr(1, string::npos));
            } else
              wdsloc.push_back(s);
            script >> s;
          }
          if (s.find("(") != string::npos) {
            wdsloc.push_back(s.substr(1, s.length() - 2));
          } else
            wdsloc.push_back(s.substr(0, s.length() - 1));
          script >> s;
          s = s.substr(0, s.length() - 1);
          double v = atof(s.c_str());
          /*cout << "Ready to assert (";
          for(int i = 0;i < wdsloc.size();++i)
          {
              cout << wdsloc[i] << " ";
          }
          cout << ") has value " << v << "\n";
          */

          assertFV(vld, wdsloc, v);
        } else if (s == "(not") {
          wdsloc.clear();
          script >> s;
          while (s.find(")") == string::npos) {
            if (s.find("(") != string::npos) {
              wdsloc.push_back(s.substr(1, string::npos));
            } else
              wdsloc.push_back(s);
            script >> s;
          }
          if (s.find("(") != string::npos) {
            wdsloc.push_back(s.substr(1, s.length() - 3));
          } else
            wdsloc.push_back(s.substr(0, s.length() - 2));
          /*                    cout << "Ready to assert (not (";
                              for(int i = 0;i < wdsloc.size();++i)
                              {
                                  cout << wdsloc[i] << " ";
                              }
                              cout << "))\n";
          */
          assertLV(vld, wdsloc, false);
        } else {
          wdsloc.clear();
          // script >> s;
          while (s.find(")") == string::npos) {
            if (s.find("(") != string::npos) {
              wdsloc.push_back(s.substr(1, string::npos));
            } else
              wdsloc.push_back(s);
            script >> s;
          }
          if (s.find("(") != string::npos) {
            wdsloc.push_back(s.substr(1, s.length() - 2));
          } else
            wdsloc.push_back(s.substr(0, s.length() - 1));
          /*cout << "Ready to assert (";
          for(int i = 0;i < wdsloc.size();++i)
          {
              cout << wdsloc[i] << " ";
          }
          cout << ")\n";
          */

          assertLV(vld, wdsloc, true);
        }
      }
    } else if (s == "declareNewObject") {
      // This will trigger the collection of new declarations and thus a new
      // problem file creation
      string t;
      getline(script, t);
      newObjects << "\n" << t << "\n";
    } else if (s == "assertNewInit[") {
      extInit << "\n";
      while (true) {
        script >> s;
        if (s == "]") {
          break;
        } else {
          extInit << s;
          if (s.find(")") != string::npos) {
            extInit << "\n";
          } else
            extInit << " ";
        }
      }
    } else if (s == "extractfunctions[") {
      vector< string > wds;
      while (true) {
        script >> s;
        if (s == "]") {
          // cout << "State is:\n" << ss.str() << "\n";

          break;
        }
        if (s.find("(", 0) == 0) {
          s.erase(0, 1);
        }

        if (s.find(")") != string::npos) {
          s.erase(s.find(")"), 1);
          wds.push_back(s);
          // cout << "Test " << wds[0] << "\n";
          LPCSTR *tmpfun = new LPCSTR[wds.size()];
          for (int i = 0; i < wds.size(); ++i) {
            tmpfun[i] = wds[i].c_str();
          }
          ss << "    (= (";
          for (int i = 0; i < wds.size(); ++i) {
            ss << wds[i];
            if (i < wds.size() - 1) ss << " ";
          }
          ss << ") ";
          ss << queryValueNamed(vld, tmpfun);
          ss << ")\n";

          delete[] tmpfun;
          wds.clear();
        } else {
          wds.push_back(s);
        }
      }
    } else if (s == "cat") {
      out.str("");
      script >> s;
      while (s != "to") {
        if (s == "extract") {
          out << ss.str();
          // cout << "Added " << ss.str() << "\n";
        } else {
          ifstream f(s.c_str());
          out << f.rdbuf();
        }
        script >> s;
      }
      script >> s;
      ofstream f(s.c_str());
      f << out.str();
      f.close();
    } else if (s == "write") {
      script >> s;
      ofstream f(s.c_str());
      // cout << "Get the state...\n";
      LPCSTR state = getState(vld);

      if (!newProb) {
        f << state;
      } else {
        string wholeProb(state);
        // cout << "Here is the state:\n" << wholeProb << "\n\n";
        // cout << wholeProb.find(":objects") << "\n" << newObjects.str() <<
        // "\n";
        int x = wholeProb.find(":objects");
        wholeProb.insert(x + 8, newObjects.str());
        x = wholeProb.find(":init");
        wholeProb.insert(x + 5, extInit.str());
        f << wholeProb;
      }

      f.close();
      cleanState(state);
    }
  }

  if (vld) cleanUp(vld);

  cout << "===================\n" << theplans.str() << "===============\n";
  return 0;
}

/*


















        if(s == "x")
        {
            executeNext(vld);
            int n;
            unsigned long * vals = getChangedLits(vld,n);
            cout << "Seeing " << n << " changed lits\n";
            for(int i = 0;i < n;++i)
            {
                int x;
                LPCSTR * nm = whatLit(vld,vals[i],x);
                for(int j = 0;j < x;++j)
                {
                    cout << nm[j] << " ";
                }
                cout << "- now " <<
(queryLiteralCode(vld,vals[i])?"true":"false") << "\n"; cleanName(nm);
            }
            cleanStore(vals);
            vals = getChangedFuns(vld, n);
            for(int i = 0;i < n;++i)
            {
                int x;
                LPCSTR * nm = whatFun(vld,vals[i],x);
                for(int j = 0;j < x;++j)
                {
                    cout << nm[j] << " ";
                }
                cout << "- now " << (queryValueCode(vld,vals[i])) << "\n";
                cleanName(nm);
            }
            cleanStore(vals);
        }
        else if(s == "w")
        {
            cin >> s;
            ofstream output(s.c_str(),ofstream::out);
            LPCSTR state = getState(vld);
            output << state;
            output.close();
            cleanState(state);
        }
        else if(s == "end")
        {
            int a;
            cin >> a;
            cin >> s;
            double t;
            cin >> t;
            post(vld,a,0,false,t);
        }
        else
        {
            while(true)
            {
                cin >> s;
                if(s == "@")
                {
                    break;
                }
                wds.push_back(s);
            }
            LPCSTR act[wds.size()];
            for(unsigned int i = 0;i < wds.size();++i)
            {
                act[i] = wds[i].c_str();
            }
            double t;
            cin >> t;
            post(vld,aID,act,true,t);
            cout << "Posted action " << aID <<"\n";
            ++aID;
        }
    }


    LPCSTR state = getState(vld);
    cout << state;
    cleanUp(vld);
    cleanState(state);
    return 0;
}
*/

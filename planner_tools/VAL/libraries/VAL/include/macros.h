// Copyright 2019 - University of Strathclyde, King's College London and Schlumberger Ltd
// This source code is licensed under the BSD license found in the LICENSE file in the root directory of this source tree.

/**
 * Macros for diagnostic output.
 * These generate code to output both name and value of a field,
 * and to deal with pretty-printing indentation in a consistent way.
 * 
 * Edit this file to change the output format of the syntax trees.
 */

// #  expands arg into quoted string
// ## concatenates arg

// Output NAME - used for name of class
#define TITLE(NAME) \
  indent(ind);      \
  cout << '(' << #NAME << ')';

// Display a data member that is a parse_category
#define FIELD(NAME)         \
  indent(ind);              \
  cout << #NAME << ": ";    \
  if (NAME != NULL)         \
    NAME->display(ind + 1); \
  else                      \
    cout << "(NULL)";

// Used for display of list element
#define ELT(NAME)               \
  {                             \
    if ((NAME) != NULL)         \
      (NAME)->display(ind + 1); \
    else                        \
      cout << "(NULL)";         \
  }

// Display NAME only
#define LABEL(NAME) \
  indent(ind);      \
  cout << #NAME << ':';

// Output a data member that is not a parse_category
#define LEAF(NAME)       \
  indent(ind);           \
  cout << #NAME << ": "; \
  cout << NAME;

extern void indent(int ind);

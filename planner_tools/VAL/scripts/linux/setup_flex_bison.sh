#!/bin/bash

PWD=`pwd`
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

SRC_DIR=$DIR/../..

cd $SRC_DIR/libraries/VAL/src/Parser
flex++ -o pddl+.lex.yy.h pddl+.l
bison pddl+.y -o pddl+.cpp

cd $PWD

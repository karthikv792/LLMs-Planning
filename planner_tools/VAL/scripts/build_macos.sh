#!/bin/bash

BASH=`which bash`
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

BUILD_TARGET=all
BUILD_CONFIGURATION=Release

$BASH $DIR/macos/clean.sh
cleanStatus=$?
if [ $cleanStatus -ne 0 ]; then 
  exit $cleanStatus
fi

$BASH $DIR/macos/build.sh $BUILD_TARGET $BUILD_CONFIGURATION
buildStatus=$?
if [ $buildStatus -ne 0 ]; then 
  exit $buildStatus
fi

$BASH $DIR/macos/test.sh $BUILD_CONFIGURATION
testStatus=$?
if [ $testStatus -ne 0 ]; then 
  exit $testStatus
fi
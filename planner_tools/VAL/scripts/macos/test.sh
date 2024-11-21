#!/bin/bash

BASH=`which bash`
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

BUILD_CONFIGURATION=$1 # Debug / Release

echo "Test"

$BASH $DIR/test_macos64.sh $BUILD_CONFIGURATION
testStatus=$?
if [ $testStatus -ne 0 ]; then 
  exit $testStatus
fi

#!/bin/bash

BASH=`which bash`
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

$BASH $DIR/clean_documentation.sh
documentationStatus=$?
if [ $documentationStatus -ne 0 ]; then 
  exit $documentationStatus
fi

$BASH $DIR/clean_build.sh
buildStatus=$?
if [ $buildStatus -ne 0 ]; then 
  exit $buildStatus
fi

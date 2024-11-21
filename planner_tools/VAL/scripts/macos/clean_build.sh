#!/bin/bash

BASH=`which bash`
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

$BASH $DIR/clean_macos64.sh
macosStatus=$?
if [ $macosStatus -ne 0 ]; then 
  exit $macosStatus
fi

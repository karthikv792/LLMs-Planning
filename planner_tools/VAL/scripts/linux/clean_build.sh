#!/bin/bash

BASH=`which bash`
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

$BASH $DIR/clean_linux64.sh
linuxStatus=$?
if [ $linuxStatus -ne 0 ]; then 
  exit $linuxStatus
fi

$BASH $DIR/clean_win32.sh
windowsStatus=$?
if [ $windowsStatus -ne 0 ]; then 
  exit $windowsStatus
fi

$BASH $DIR/clean_win64.sh
windowsStatus=$?
if [ $windowsStatus -ne 0 ]; then 
  exit $windowsStatus
fi

#!/bin/bash

BASH=`which bash`
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

BUILD_TARGET=$1 # all
BUILD_CONFIGURATION=$2 # Debug / Release

echo "Build $BUILD_TARGET - $BUILD_CONFIGURATION"

$BASH $DIR/build_linux64.sh $BUILD_TARGET $BUILD_CONFIGURATION
linuxStatus=$?
if [ $linuxStatus -ne 0 ]; then 
  exit $linuxStatus
fi

$BASH $DIR/build_win32.sh $BUILD_TARGET $BUILD_CONFIGURATION
windowsStatus=$?
if [ $windowsStatus -ne 0 ]; then 
  exit $windowsStatus
fi

$BASH $DIR/build_win64.sh $BUILD_TARGET $BUILD_CONFIGURATION
windowsStatus=$?
if [ $windowsStatus -ne 0 ]; then 
  exit $windowsStatus
fi

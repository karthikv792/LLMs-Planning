#!/bin/bash

BASH=`which bash`
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

BUILD_TARGET=$1 # all
BUILD_CONFIGURATION=$2 # Debug / Release

echo "Build $BUILD_TARGET - $BUILD_CONFIGURATION"

$BASH $DIR/build_macos64.sh $BUILD_TARGET $BUILD_CONFIGURATION
macosStatus=$?
if [ $macosStatus -ne 0 ]; then 
  exit $macosStatus
fi
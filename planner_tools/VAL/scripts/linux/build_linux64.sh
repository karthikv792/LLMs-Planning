#!/bin/bash

echo "Build for linux64"

BUILD_GENERATOR="Unix Makefiles"
BUILD_TARGET=$1 # all
BUILD_CONFIGURATION=$2 # Debug / Release
BUILD_THREAD=4

PWD=`pwd`
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

BUILD_DIR=$DIR/../../build
TARGET_DIR=linux64

mkdir $BUILD_DIR
cd $BUILD_DIR

mkdir $TARGET_DIR
cd $TARGET_DIR

mkdir $BUILD_CONFIGURATION
cd $BUILD_CONFIGURATION
echo "Generate scripts"
cmake -G "$BUILD_GENERATOR" -DCMAKE_BUILD_TYPE=$BUILD_CONFIGURATION -DCMAKE_INSTALL_PREFIX=`pwd`/install ../../..
generateStatus=$?
if [ $generateStatus -ne 0 ]; then
  echo "Scripts generation failed."
  exit $generateStatus
else
  echo "Scripts generation succesful."
fi
echo "Build using the scripts"
cmake --build . --config $BUILD_CONFIGURATION --target $BUILD_TARGET -- -j $BUILD_THREAD
buildStatus=$?
if [ $buildStatus -ne 0 ]; then
  echo "Build failed."
  exit $buildStatus
else
  echo "Build succesful."
fi
echo "Install using the scripts"
cmake --build . --target install
installStatus=$?
if [ $installStatus -ne 0 ]; then
  echo "Install failed."
  exit $installStatus
else
  echo "Install succesful."
fi
echo "Package using the scripts"
cpack ../../..
packageStatus=$?
if [ $packageStatus -ne 0 ]; then
  echo "Package failed."
  exit $packageStatus
else
  echo "Package succesful."
fi
cd ..

cd ..

cd $PWD

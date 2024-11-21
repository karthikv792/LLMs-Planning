#!/bin/bash

echo "Generate documentation"

DOXYGEN=`which doxygen`
PWD=`pwd`
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

PROJECT_DIR=$DIR/../..
BUILD_DIR=$PROJECT_DIR/build

mkdir $BUILD_DIR

cd $PROJECT_DIR
$DOXYGEN doxygen.config

cd $BUILD_DIR
zip -r documentation.zip documentation
cd $PWD

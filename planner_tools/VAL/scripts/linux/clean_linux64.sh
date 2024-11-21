#!/bin/bash

echo "Clean linux64"

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

BUILD_DIR=$DIR/../../build/linux64

rm -rf $BUILD_DIR
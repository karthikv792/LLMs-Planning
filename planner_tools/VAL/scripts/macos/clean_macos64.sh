#!/bin/bash

echo "Clean macos64"

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

BUILD_DIR=$DIR/../../build/macos64

rm -rf $BUILD_DIR
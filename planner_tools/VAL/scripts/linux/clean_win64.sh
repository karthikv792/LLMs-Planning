#!/bin/bash

echo "Clean win64"

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

BUILD_DIR=$DIR/../../build/linux_win64

rm -rf $BUILD_DIR
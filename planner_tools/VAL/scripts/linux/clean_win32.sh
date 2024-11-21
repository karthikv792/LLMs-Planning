#!/bin/bash

echo "Clean win32"

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

BUILD_DIR=$DIR/../../build/linux_win32

rm -rf $BUILD_DIR
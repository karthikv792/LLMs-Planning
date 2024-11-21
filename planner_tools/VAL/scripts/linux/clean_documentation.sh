#!/bin/bash

echo "Clean documentation"

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

DOCUMENTATION_DIR=$DIR/../../documentation

rm -rf $DOCUMENTATION_DIR

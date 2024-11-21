#!/bin/bash

BASH=`which bash`

export BUILD_SOURCESDIRECTORY="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )/.."
export BUILD_BUILDNUMBER="dev"

COMMAND="$BASH $BUILD_SOURCESDIRECTORY/scripts/linux/clean.sh"
echo $COMMAND
$COMMAND

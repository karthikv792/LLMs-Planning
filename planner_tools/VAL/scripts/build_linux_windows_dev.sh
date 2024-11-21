#!/bin/bash

BASH=`which bash`

export BUILD_SOURCESDIRECTORY="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )/.."
export BUILD_REPOSITORY_NAME="ai-planning-tool-valex"
export BUILD_SOURCEBRANCH="refs/pull/dev"
export BUILD_BUILDNUMBER="dev"

COMMAND="$BASH $BUILD_SOURCESDIRECTORY/scripts/build_linux_windows.sh"
echo $COMMAND
$COMMAND

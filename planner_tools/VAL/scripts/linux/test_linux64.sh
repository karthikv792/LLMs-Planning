#!/bin/bash

BASH=`which bash`

PWD=`pwd`
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

BUILD_CONFIGURATION=$1 # Debug / Release

pushd $DIR/../../build/linux64/$BUILD_CONFIGURATION
unzip Val-$BUILD_BUILDNUMBER-Linux.zip

EXECUTABLE_DIR=./Val-$BUILD_BUILDNUMBER-Linux/bin

echo "Display libraries."
ldd $EXECUTABLE_DIR/Validate

IFS=$','
while read -r domain problem plan path || [ -n "$domain" ]
do
    echo "Validating domain:$domain problem:$problem plan:$plan"
    $EXECUTABLE_DIR/Validate -t 0.001 $DIR/../$path$domain $DIR/../$path$problem $DIR/../$path$plan

    validateStatus=$?
    if [ $validateStatus -ne 0 ]; then
        echo "Failed to validate $domain $problem $plan"
        exit $validateStatus
    else
        echo "Successfully validated $domain $problem $plan"
    fi
    printf "\n\n"
done < "$DIR/../validate_testcases.txt"

popd

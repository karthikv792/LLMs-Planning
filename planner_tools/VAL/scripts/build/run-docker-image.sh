#!/bin/bash

if [ "$#" -gt 1 ]; then
    buildDockerImageTag="$1"
    buildCommand="${@:2}"
else
    echo "Usage:"
    echo "- $0 <BUILD_DOCKER_IMAGE_TAG> <BUILD_COMMAND>"
    exit 1
fi

printenv | grep '^BUILD_' > $BUILD_SOURCESDIRECTORY/build.env

SOURCESDIRECTORY=$BUILD_SOURCESDIRECTORY
SOURCESDIRECTORY=${SOURCESDIRECTORY/\/mnt\/c\//c\:\/}
SOURCESDIRECTORY=${SOURCESDIRECTORY/\/mnt\/d\//d\:\/}

DOCKER=`which docker`
BUILD_COMMAND="$DOCKER run --rm -v $SOURCESDIRECTORY:/src --env-file $BUILD_SOURCESDIRECTORY/build.env $buildDockerImageTag $buildCommand"
echo $BUILD_COMMAND
$BUILD_COMMAND

buildStatus=$?

rm -f $BUILD_SOURCESDIRECTORY/build.env

exit $buildStatus

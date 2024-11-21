#!/bin/bash

if [ "$#" -eq 1 ]; then
    buildDockerImageTag="$1"
else
    echo "Usage:"
    echo "- $0 <BUILD_DOCKER_IMAGE_TAG>"
    exit 1
fi

echo "Docker image tag: $buildDockerImageTag"

DOCKER=`which docker`
BUILD_COMMAND="$DOCKER rmi -f $buildDockerImageTag"
echo $BUILD_COMMAND
$BUILD_COMMAND

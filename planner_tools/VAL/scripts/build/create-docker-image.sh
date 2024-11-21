#!/bin/bash

if [ "$#" -eq 2 ]; then
    buildDockerImageFolder="$1"
    buildDockerImageTag="$2"
else
    echo "Usage:"
    echo "- $0 <BUILD_DOCKER_IMAGE_FOLDER> <BUILD_DOCKER_IMAGE_TAG>"
    exit 1
fi

echo "Docker image definition folder: $buildDockerImageFolder"
echo "Docker image tag: $buildDockerImageTag"

DOCKER=`which docker`
BUILD_COMMAND="$DOCKER build -f $buildDockerImageFolder/Dockerfile -t $buildDockerImageTag $buildDockerImageFolder"
echo $BUILD_COMMAND
$BUILD_COMMAND

FROM debian:9-slim

RUN apt-get update -y && \
  apt-get install --no-install-recommends -y -q \
  # install cmake
  cmake \
  # install make g++
  make g++ \
  # install clang-format
  clang-format-3.9 \
  # install doxygen
  doxygen \
  graphviz \
  # install mingw
  mingw-w64 \
  # install zip
  zip unzip \
  && \
  apt-get clean && \
  rm /var/lib/apt/lists/*_*

ARG SRC_DIR=/src

RUN mkdir ${SRC_DIR}
WORKDIR ${SRC_DIR}

# Declare volume for the sources to build
VOLUME ${SRC_DIR}
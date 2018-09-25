#!/bin/sh
set -e


mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DPYTHON_DEFAULT_EXECUTABLE=/usr/bin/python3 ../
make
ctest

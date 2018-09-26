#!/bin/sh
set -e


mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS=-Werror -DCMAKE_C_FLAGS=-Werror -DPYTHON_DEFAULT_EXECUTABLE=/usr/bin/python3 ../
make
/usr/bin/python2 /usr/bin/run-clang-tidy-3.8.py -checks=-*,clang-analyzer-*,-clang-analyzer-cplusplus*
ctest

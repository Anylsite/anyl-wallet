#!/bin/sh
set -e


mkdir -p build && cd build
# even though we set EXPORT_COMPILE_COMMANDS in the CMakeLists.txt, we have to specify it here - CI uses older cmake version
cmake -GNinja -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS=-Werror -DCMAKE_C_FLAGS=-Werror -DPYTHON_DEFAULT_EXECUTABLE=/usr/bin/python3 ../
ninja
/usr/bin/python2 /usr/bin/run-clang-tidy-3.8.py -checks=-*,clang-analyzer-*,-clang-analyzer-cplusplus*
ctest

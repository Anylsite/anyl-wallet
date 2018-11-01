#!/bin/sh
set -e


mkdir -p build && cd build
# even though we set EXPORT_COMPILE_COMMANDS in the CMakeLists.txt, we have to specify it here - CI uses older cmake version
cmake -GNinja -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-Werror $CMAKE_CXX_FLAGS" -DCMAKE_C_FLAGS="-Werror $CMAKE_C_FLAGS" -DPYTHON_DEFAULT_EXECUTABLE=/usr/bin/python3 ../
ninja
python3 /usr/bin/run-clang-tidy -checks=-*,clang-analyzer-*,-clang-analyzer-cplusplus*
ctest

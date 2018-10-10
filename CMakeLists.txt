# P3 si required for abi compiler
find_package( PythonInterp 3 REQUIRED )
include(cmake/solc.cmake)
# export compile commands for linter
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(BUILD_XCOMPILE)
    # this is a zephyr build
    cmake_minimum_required(VERSION 3.8.2)
    include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
    project(NONE)
    include(cmake/zephyr.cmake)
    include(cmake/wallet.cmake)
else()
    # generic x86 build - CI etc
    cmake_minimum_required(VERSION 3.0.0)
    project("wallet-x86")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -pedantic -Wall -Wextra")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14 -pedantic -Wall -Wextra")
# enable testing for x86 builds
    enable_testing()

    include(cmake/wallet.cmake)
    include(cmake/trezor-crypto.cmake)
    add_subdirectory(tests)

endif()

# P3 si required for abi compiler
find_package( PythonInterp 3 REQUIRED )
# export compile commands for linter
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if ((${CMAKE_BUILD_TYPE} MATCHES "Debug"))
    SET(CMAKE_CXX_FLAGS_DEBUG "-O0")
    SET(CMAKE_C_FLAGS_DEBUG "-O0")
endif()

if(BUILD_XCOMPILE)
    # this is a zephyr build
    cmake_minimum_required(VERSION 3.8.2)
    set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
    set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
    link_directories(${CMAKE_CURRENT_BINARY_DIR}/install/lib/)
    add_compile_options (-fdiagnostics-color=always)
    include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
    project(NONE)
    include(cmake/solc.cmake)
    include_directories(helpers/zephyr)
    include(cmake/zephyr.cmake)
    include(cmake/trezor-crypto.cmake)
    include(cmake/nanopb.cmake)
    include(cmake/wallet.cmake)
else()
    # generic x86 build - CI etc
    cmake_minimum_required(VERSION 3.0.0)
    project("wallet-x86")
    include(cmake/solc.cmake)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -pedantic -Wall -Wextra")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14 -pedantic -Wall -Wextra")
# enable testing for x86 builds
    enable_testing()

    include(cmake/wallet.cmake)
    include(cmake/trezor-crypto.cmake)
    include(cmake/nanopb.cmake)
    add_subdirectory(tests)

endif()

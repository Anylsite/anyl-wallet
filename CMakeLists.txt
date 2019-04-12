#set(BUILD_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/install CACHE STRING "install destination for 3rd party libs and plugins")
set(BUILD_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/install CACHE STRING "")
set(WALLET_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "")

# P3 is required
find_package( PythonInterp 3 REQUIRED )

if ((${CMAKE_BUILD_TYPE} MATCHES "Debug"))
    SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
    SET(CMAKE_C_FLAGS_DEBUG "-O0 -g")
endif()

include(cmake/solc.cmake)
if(BUILD_XCOMPILE)
    project(NONE)
    include_directories(helpers/zephyr)
    link_directories(${CMAKE_CURRENT_BINARY_DIR}/install/lib/)
    include(cmake/zephyr.cmake)
    add_subdirectory(zephyr-wallet)
else()
    cmake_minimum_required(VERSION 3.0.0)
    project("wallet-x86")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -pedantic -Wall -Wextra")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14 -pedantic -Wall -Wextra")
    message(STATUS "x86 build!")

    # enable testing for x86 builds
    enable_testing()
    add_subdirectory(examples/tests)
    include_directories( ${CMAKE_SOURCE_DIR}/helpers )

endif()
include(cmake/trezor-crypto.cmake)
include(cmake/nanopb.cmake)
include(cmake/wallet.cmake)

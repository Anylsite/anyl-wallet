# boilerplate file for building wallet targets. This sets up the environment for different builds.
# to crosscompile using Zephyr define -DBOARD=<board> and -DBUILD_XCOMPILE=1 when running cmake

# link_directories applies only for targets created after it is called!
link_directories(${CMAKE_CURRENT_BINARY_DIR}/wallet/install/lib/)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../)

# P3 is required
find_package( PythonInterp 3 REQUIRED )

# do Zephyr magic if we crosscompile
if(BUILD_XCOMPILE)
    cmake_minimum_required(VERSION 3.8.2)
    set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
    set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
    add_compile_options (-fdiagnostics-color=always)
    include_directories(helpers/zephyr)
    include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
else()
    # generic x86 build - CI etc
    cmake_minimum_required(VERSION 3.0.0)
    project("wallet-x86")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -pedantic -Wall -Wextra")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14 -pedantic -Wall -Wextra")
endif()

# build our wallet helpers/library
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../ wallet)

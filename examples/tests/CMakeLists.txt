# build wallet sources
#include(../../cmake/boilerplate.cmake)
# project must be set to NONE for reasons only cmake devs know
project(NONE)
cmake_minimum_required(VERSION 3.8.2)


if(BUILD_XCOMPILE)
    message(FATAL_ERROR "This is a google test build - Zephyr not supported.")
else()
    enable_testing()
    add_subdirectory(src)
    include_directories(${CMAKE_SOURCE_DIR}/helpers)

endif()

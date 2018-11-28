# build wallet sources
include(../../cmake/boilerplate.cmake)
# project must be set to NONE for reasons only cmake devs know
project(NONE)
cmake_minimum_required(VERSION 3.8.2)
if(BUILD_XCOMPILE)
    target_link_libraries(app PUBLIC al-zephyr)
else()
    message(FATAL_ERROR "x86 build not available")
endif()

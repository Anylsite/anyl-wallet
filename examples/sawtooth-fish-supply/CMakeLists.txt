# build wallet sources
include(../../cmake/boilerplate.cmake)
# project must be set to NONE for reasons only cmake devs know
project(NONE)
cmake_minimum_required(VERSION 3.8.2)
if(BUILD_XCOMPILE)
    file(GLOB APP_SOURCES "app/*.c")
    add_subdirectory(payload/)
    target_sources(app PRIVATE ${APP_SOURCES})
    target_link_libraries(app PUBLIC fish-demo al-zephyr)
    target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
else()
    message(STATUS "x86 build")
endif()

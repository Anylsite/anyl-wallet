cmake_minimum_required(VERSION 3.0)

project(anyledger-wallet-helpers)
include_directories(..)

file(GLOB SOURCES "*.c")


add_library(helpers STATIC ${SOURCES})
target_include_directories( helpers PUBLIC  ${PROJECT_SOURCE_DIR})


if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    include_directories(macos)
endif()

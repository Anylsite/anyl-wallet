cmake_minimum_required(VERSION 3.5)
find_package(GTest REQUIRED)
find_package (Threads REQUIRED)
set (CMAKE_CXX_FLAGS "-std=c++11 -g")

include_directories(${GTEST_INCLUDE_DIRS})

project(anyledger-wallet-test)
include_directories(..)

file(GLOB SOURCES "*.c" "*.cpp")

add_executable(testX test_rlp.cpp)
enable_testing()
target_link_libraries(
    testX
    ${GTEST_BOTH_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    helpers
    crypto
    )
add_test(MyTest testX)

cmake_minimum_required(VERSION 3.0.0)
project("wallet-x86")
include_directories(.)


file(GLOB SOURCES "*.c")

add_library(trezor-crypto STATIC ${SOURCES})

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
target_compile_definitions(trezor-crypto PRIVATE USE_ETHEREUM=1)
target_compile_definitions(trezor-crypto PRIVATE USE_KECCAK=1)

install(TARGETS trezor-crypto ARCHIVE DESTINATION lib)

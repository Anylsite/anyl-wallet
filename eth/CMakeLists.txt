cmake_minimum_required(VERSION 3.0)

project(anyledger-wallet-eth)
include_directories(..)

file(GLOB SOURCES "*.c")

add_library(eth STATIC ${SOURCES})
add_dependencies(eth trezor-crypto-lib)

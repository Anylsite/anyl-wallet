cmake_minimum_required(VERSION 3.0)

project(anyledger-wallet-sawtooth)
include_directories(..)

file(GLOB SOURCES "*.c")

add_library(sawtooth STATIC ${SOURCES})
add_dependencies(sawtooth nanopb)

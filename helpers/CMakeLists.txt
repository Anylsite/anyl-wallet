cmake_minimum_required(VERSION 3.5)

project(anyledger-wallet-helpers)
include_directories(..)

file(GLOB SOURCES "*.c")

add_library(helpers SHARED ${SOURCES})


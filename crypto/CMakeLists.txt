cmake_minimum_required(VERSION 3.0)

project(anyledger-wallet-crypto)
include_directories(..)

file(GLOB SOURCES "*.c")

add_library(crypto SHARED ${SOURCES})

cmake_minimum_required(VERSION 3.5)

project(anyledger-wallet-eth)
include_directories(..)

file(GLOB SOURCES "*.c")

add_library(eth SHARED ${SOURCES})

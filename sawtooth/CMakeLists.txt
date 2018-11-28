cmake_minimum_required(VERSION 3.0)

include_directories(..)

nanopb_compile(${CMAKE_CURRENT_SOURCE_DIR}/protos/transaction.proto TX_SRC TX_HDR)
nanopb_compile(${CMAKE_CURRENT_SOURCE_DIR}/protos/batch.proto BATCH_SRC BATCH_HDR)

file(GLOB SOURCES "*.c")
add_library(sawtooth STATIC ${SOURCES} ${TX_SRC} ${BATCH_SRC})
target_include_directories(sawtooth
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)
target_link_libraries(sawtooth
    PUBLIC
        protobuf-nanopb
        helpers
        )
add_dependencies(sawtooth trezor-crypto-lib)
add_dependencies(sawtooth nanopb)

cmake_minimum_required(VERSION 3.0)
find_package(GTest REQUIRED)
find_package (Threads REQUIRED)
find_package(PkgConfig)

include_directories(${GTEST_INCLUDE_DIRS})

include_directories(..)
include_directories(${PC_JSON-C_INCLUDE_DIRS})

include_directories(${CMAKE_SOURCE_DIR})
link_directories(${CMAKE_SOURCE_DIR})




solc_compile_abi(${CMAKE_CURRENT_SOURCE_DIR}/TestContract.sol ${CMAKE_CURRENT_BINARY_DIR}/contracts_abi.json)
solc_abi_to_c(${CMAKE_CURRENT_BINARY_DIR}/contracts_abi.json ${CMAKE_CURRENT_BINARY_DIR}/contracts.h h)
solc_abi_to_c(${CMAKE_CURRENT_BINARY_DIR}/contracts_abi.json ${CMAKE_CURRENT_BINARY_DIR}/contracts.c c)
solc_abi_to_c(${CMAKE_CURRENT_BINARY_DIR}/contracts_abi.json ${CMAKE_CURRENT_BINARY_DIR}/test_abi_compiler.cpp test)

add_executable(
    test_abi_compiler
    ${CMAKE_CURRENT_BINARY_DIR}/test_abi_compiler.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/contracts_abi.json
    ${CMAKE_CURRENT_BINARY_DIR}/contracts.c
    ${CMAKE_CURRENT_BINARY_DIR}/contracts.h

    )




add_executable(test_data test_data.cpp)
add_executable(test_rlp test_rlp.cpp)
add_executable(test_helpers test_helpers.cpp)
add_executable(test_crypto test_crypto.cpp)
add_executable(test_tx test_tx.cpp)
add_executable(test_web3 test_web3.cpp)
add_executable(test_sawtooth test_sawtooth.cpp)


target_compile_definitions(
    test_abi_compiler
    PRIVATE
    ABI_TEST_DATA_JSON="${CMAKE_CURRENT_BINARY_DIR}/abi_test_data.json"
    )
target_link_libraries(
    test_rlp
    ${GTEST_BOTH_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    helpers
    )
target_link_libraries(
    test_data
    ${GTEST_BOTH_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    helpers
    eth
    )
target_link_libraries(
    test_abi_compiler
    ${GTEST_BOTH_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    helpers
    eth
    )
target_link_libraries(
    test_helpers
    ${GTEST_BOTH_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    helpers
    )
target_link_libraries(
    test_crypto
    ${GTEST_BOTH_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    eth
    trezor-crypto
    helpers
    )
target_link_libraries(
    test_tx
    ${GTEST_BOTH_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    eth
    trezor-crypto
    helpers
    )
target_link_libraries(
    test_web3
    ${GTEST_BOTH_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    eth
    helpers
    trezor-crypto
    )
target_include_directories(test_sawtooth PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/../sawtooth)
target_link_libraries(
    test_sawtooth
    ${GTEST_BOTH_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    helpers
    sawtooth
    eth
    protobuf-nanopb
    trezor-crypto
    )

message("---------------")

add_test(test_rlp test_rlp)
add_test(test_data test_data)
add_test(test_abi test_abi_compiler)
add_test(test_helpers test_helpers)
add_test(test_crypto test_helpers)
add_test(test_tx test_tx)
add_test(test_web3 test_web3)
add_test(test_sawtooth test_sawtooth)

cmake_minimum_required(VERSION 3.0.0)
project("trezor-crypto-lib")
include_directories(.)

file(GLOB SOURCES "*.c")
file(GLOB HEADERS "*.h")
file(GLOB HEADERS_AES "aes/*.h")
file(GLOB HEADERS_CHACHA "chacha20poly1305/*.h")
file(GLOB HEADERS_DONNA "ed25519-donna/*.h")

add_library(trezor-crypto STATIC ${SOURCES})
set_target_properties(trezor-crypto PROPERTIES LINKER_LANGUAGE C)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
target_compile_definitions(trezor-crypto PRIVATE USE_ETHEREUM=1)
target_compile_definitions(trezor-crypto PRIVATE USE_KECCAK=1)
target_compile_definitions(trezor-crypto PRIVATE RAND_PLATFORM_INDEPENDENT=1)

install(TARGETS trezor-crypto ARCHIVE DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include/trezor-crypto)
install(FILES ${HEADERS_CHACHA} DESTINATION include/trezor-crypto/chacha20poly1305/)
install(FILES ${HEADERS_DONNA} DESTINATION include/trezor-crypto/ed25519-donna/)

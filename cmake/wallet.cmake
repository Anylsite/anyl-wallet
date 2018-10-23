# recompile abi compiler targets if compiler source changes
file(GLOB ABI_COMPILER_SOURCES "abi_compiler/*.py")

link_directories(${CMAKE_CURRENT_BINARY_DIR}/install/lib/)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/install/include/)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/install/include/trezor-crypto)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(eth)
add_subdirectory(helpers)

add_library(wallet STATIC wallet/wallet.c)
set_target_properties(wallet PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(wallet eth helpers)
install(TARGETS wallet ARCHIVE DESTINATION lib)


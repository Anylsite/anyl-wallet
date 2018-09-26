# recompile abi compiler targets if compiler source changes
file(GLOB ABI_COMPILER_SOURCES "abi_compiler/*.py")

add_subdirectory(crypto)
add_subdirectory(eth)
add_subdirectory(helpers)

add_library(wallet STATIC)
set_target_properties(wallet PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(wallet crypto eth helpers)
install(TARGETS wallet ARCHIVE DESTINATION lib)

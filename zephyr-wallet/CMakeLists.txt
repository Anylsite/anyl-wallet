if(NOT BUILD_XCOMPILE)
    message(FATAL_ERROR "${CMAKE_CURRENT_SOURCE_DIR} should be compiled only for Zephyr builds!")
endif()
file(GLOB SOURCES "*.c")
# ERC20 contract ABI
set(ERC20_JSON ${CMAKE_CURRENT_BINARY_DIR}/erc20_token.json)
solc_compile_abi(contracts/Token.sol ${ERC20_JSON})
solc_abi_to_c(${ERC20_JSON} ${CMAKE_CURRENT_BINARY_DIR}/erc20_abi.h h)
solc_abi_to_c(${ERC20_JSON} ${CMAKE_CURRENT_BINARY_DIR}/erc20_abi.c c)

# keyfile generator
file(GLOB KEYFILE_GEN_SOURCES "tools/pk_gen.py")
function(generate_zephyr_keyfile output)
add_custom_command(
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/
    COMMAND ${PYTHON_EXECUTABLE} -m tools.pk_gen --auto --keyfile ${CMAKE_CURRENT_SOURCE_DIR}/pk.txt > ${output}
    DEPENDS ${KEYFILE_GEN_SOURCES} ${input}
    OUTPUT ${output}
    COMMENT "Generating private key file"
)
endfunction()


set(APP_KEYFILE "${CMAKE_CURRENT_BINARY_DIR}/keyfile.c")
generate_zephyr_keyfile(${APP_KEYFILE})

zephyr_library_named(al-zephyr)
target_sources(al-zephyr PRIVATE ${SOURCES} ${APP_KEYFILE} ${CMAKE_CURRENT_BINARY_DIR}/erc20_abi.c ${CMAKE_CURRENT_BINARY_DIR}/erc20_abi.h)
target_link_libraries(al-zephyr PUBLIC eth trezor-crypto helpers)
target_include_directories(al-zephyr PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

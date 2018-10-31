# Zephyr build dependencies
# external lib build example
# https://github.com/zephyrproject-rtos/zephyr/blob/master/samples/application_development/external_lib/CMakeLists.txt#L17
set_property(GLOBAL PROPERTY CSTD c11)
include(ExternalProject)
# this is zephyr app
file(GLOB SOURCES "zephyr/*.c")


# ERC20 contract ABI
set(ERC20_JSON ${CMAKE_CURRENT_BINARY_DIR}/zephyr/erc20_token.json)
solc_compile_abi(zephyr/contracts/Token.sol ${ERC20_JSON})
solc_abi_to_c(${ERC20_JSON} ${CMAKE_CURRENT_BINARY_DIR}/zephyr/erc20_abi.h h)
solc_abi_to_c(${ERC20_JSON} ${CMAKE_CURRENT_BINARY_DIR}/zephyr/erc20_abi.c c)

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
set(APP_KEYFILE "${CMAKE_CURRENT_BINARY_DIR}/zephyr/keyfile.c")
generate_zephyr_keyfile( ${APP_KEYFILE})

target_sources(app PRIVATE ${SOURCES} ${APP_KEYFILE} ${CMAKE_CURRENT_BINARY_DIR}/zephyr/erc20_abi.c ${CMAKE_CURRENT_BINARY_DIR}/zephyr/erc20_abi.h)
zephyr_get_include_directories_for_lang_as_string(       C includes)
zephyr_get_system_include_directories_for_lang_as_string(C system_includes)
zephyr_get_compile_definitions_for_lang_as_string(       C definitions)
zephyr_get_compile_options_for_lang_as_string(           C options)


# extend global cflags with zephyr includes etc.
set(external_project_cflags
  "${includes} ${definitions} ${options} ${system_includes}"
  )
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${external_project_cflags}")

set_target_properties(app PROPERTIES COMPILE_FLAGS "-Wall -Wextra")
include_directories(zephyr/)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/zephyr/)

add_dependencies(app trezor-crypto-lib)
add_dependencies(app wallet)
target_link_libraries(app PUBLIC wallet trezor-crypto)

# helper function to compile .sol to json abi
function(solc_compile_abi input output)
add_custom_command(
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND solc ${input} --combined-json abi,bin 2>/dev/null > ${output}
    DEPENDS ${input}
    OUTPUT ${output}
    COMMENT "Compiling ${input} ABI"
)
endfunction()

# helper function to compile json ABI to .c, .h and .cpp gtest files
function(solc_abi_to_c input output target)
add_custom_command(
    WORKING_DIRECTORY "${WALLET_ROOT_DIR}"
    COMMAND ${PYTHON_EXECUTABLE} -m abi_compiler --output ${target} --abi ${input} > ${output}
    DEPENDS ${ABI_COMPILER_SOURCES} ${input}
    OUTPUT ${output}
    COMMENT "SOLC abi: ${input} -> ${output}"
)
endfunction()

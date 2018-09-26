cmake_minimum_required(VERSION 3.0)
enable_testing()
find_package( PythonInterp 3 REQUIRED )
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic -std=gnu11 -Wall -Wextra")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -std=gnu++14 -Wall -Wextra")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_definitions(-D__STATIC_INLINE=static\ inline)
add_definitions(-D__REV=__rev)
include_directories(SYSTEM ../nRF5_SDK_15.0.0_a53641a/nRF5_SDK_15.0.0_a53641a/components/libraries/experimental_log/)
include_directories(SYSTEM ../nRF5_SDK_15.0.0_a53641a/nRF5_SDK_15.0.0_a53641a/components/libraries/experimental_log/src/)
include_directories(SYSTEM ../nRF5_SDK_15.0.0_a53641a/nRF5_SDK_15.0.0_a53641a/components/libraries/util/)
include_directories(SYSTEM ../nRF5_SDK_15.0.0_a53641a/nRF5_SDK_15.0.0_a53641a/components/libraries/balloc/)
include_directories(SYSTEM ../nRF5_SDK_15.0.0_a53641a/nRF5_SDK_15.0.0_a53641a/components/libraries/strerror/)
include_directories(SYSTEM ../nRF5_SDK_15.0.0_a53641a/nRF5_SDK_15.0.0_a53641a/components/drivers_nrf/nrf_soc_nosd/)
include_directories(SYSTEM ../nRF5_SDK_15.0.0_a53641a/nRF5_SDK_15.0.0_a53641a/components/libraries/experimental_section_vars/)
include_directories(SYSTEM ../nRF5_SDK_15.0.0_a53641a/nRF5_SDK_15.0.0_a53641a/components/libraries/experimental_memobj/)
include_directories(SYSTEM ../nRF5_SDK_15.0.0_a53641a/nRF5_SDK_15.0.0_a53641a/modules/nrfx/mdk/)
include_directories(pca10056/blank/config/)

# recompile abi compiler targets if compiler source changes
file(GLOB ABI_COMPILER_SOURCES "abi_compiler/*.py")

# helper function to compile .sol to json abi
function(solc_compile_abi input output)
add_custom_command(
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND solc ${input} --combined-json abi,bin 2>/dev/null > ${output}
    DEPENDS ${input}
    OUTPUT ${output}
    COMMENT "Compiling ${input} ABI"
)
endfunction()

# helper function to compile json ABI to .c, .h and .cpp gtest files
function(solc_abi_to_c input output target)
add_custom_command(
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/../
    COMMAND ${PYTHON_EXECUTABLE} -m abi_compiler --output ${target} --abi ${input} > ${output}
    DEPENDS ${ABI_COMPILER_SOURCES} ${input}
    OUTPUT ${output}
    COMMENT "Generating contract's ${target} file"
)
endfunction()

add_subdirectory(crypto)
add_subdirectory(eth)
add_subdirectory(helpers)
add_subdirectory(tests)
#add_subdirectory(contracts)



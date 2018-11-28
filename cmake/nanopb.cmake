# clones & builds `nanopb` library
include(ExternalProject)

set(mylib_build_dir ${CMAKE_CURRENT_BINARY_DIR}/nanopb)
set(mylib_source_dir ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/nanopb)

ExternalProject_Add(
  nanopb

  PREFIX     ${mylib_build_dir} # Root dir for entire project
  GIT_REPOSITORY "https://github.com/nanopb/nanopb.git"
  GIT_TAG "master"
  SOURCE_DIR ${mylib_source_dir}
  UPDATE_COMMAND ""
  PATCH_COMMAND ${CMAKE_COMMAND} -E copy
  "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/nanopb/CMakeLists.txt" ${mylib_source_dir}/CMakeLists.txt
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/install
             -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
             -DCMAKE_RANLIB=${CMAKE_RANLIB}
             -DCMAKE_AR=${CMAKE_AR}
             -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
             -DCMAKE_EXE_LINKER_FLAGS=${options}
             -DCMAKE_C_COMPILER_WORKS=1
             -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
)
function(nanopb_compile input SRCS HDRS)
    # compiled file name w/o the suffix
    get_filename_component(PROTO_NAME ${input} NAME_WE)
    get_filename_component(PROTO_PATH ${input} DIRECTORY)
    execute_process(
        COMMAND ${PYTHON_EXECUTABLE} -c
        "from distutils import sysconfig; print(sysconfig.get_python_lib(prefix='${BUILD_INSTALL_DIR}')+'/nanopb/');"
            OUTPUT_VARIABLE NANOPB_INST_DIR
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    add_custom_command(
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMAND protoc --plugin=${NANOPB_INST_DIR}/protoc-gen-nanopb
            -I${PROTO_PATH} --nanopb_out=${CMAKE_CURRENT_BINARY_DIR}
            ${input}
        DEPENDS ${input} nanopb
        OUTPUT ${PROTO_NAME}.pb.c
        COMMENT "Compiling ${input} protobuf"
    )
    set(${SRCS} ${PROTO_NAME}.pb.c PARENT_SCOPE)
    set(${HDRS} ${PROTO_NAME}.pb.h PARENT_SCOPE)
endfunction()

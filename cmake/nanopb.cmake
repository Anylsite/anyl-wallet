# clones & builds `nanopb` library
include(ExternalProject)

set(mylib_build_dir ${CMAKE_CURRENT_BINARY_DIR}/nanopb)
set(mylib_source_dir ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/nanopb)


ExternalProject_Add(
  nanopb
  PREFIX ${mylib_build_dir}
  DOWNLOAD_DIR ${mylib_build_dir}
  URL https://jpa.kapsi.fi/nanopb/download/nanopb-0.3.9.3-linux-x86.tar.gz
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
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
    set(NANOPB_BINARY "${mylib_build_dir}/src/nanopb/generator-bin/protoc-gen-nanopb")
    message("THIS IS NANOPB_BINARY ${NANOPB_BINARY}")

    add_custom_command(
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMAND ${NANOPB_BINARY} -I${PROTO_PATH} --nanopb_out=${CMAKE_CURRENT_BINARY_DIR} ${input}
        DEPENDS ${input} nanopb
        OUTPUT ${PROTO_NAME}.pb.c
        COMMENT "Compiling ${input} protobuf"
    )
    set(${SRCS} ${PROTO_NAME}.pb.c PARENT_SCOPE)
    set(${HDRS} ${PROTO_NAME}.pb.h PARENT_SCOPE)
endfunction()

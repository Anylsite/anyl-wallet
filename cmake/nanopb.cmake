# clones & builds `nanopb` library
include(ExternalProject)

set(mylib_build_dir ${CMAKE_CURRENT_BINARY_DIR}/nanopb)
set(mylib_source_dir ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/nanopb)

ExternalProject_Add(
  nanopb

  PREFIX     ${mylib_build_dir} # Root dir for entire project
  GIT_REPOSITORY "https://github.com/nanopb/nanopb.git"
  GIT_TAG "master"
  
  UPDATE_COMMAND ""
  
  SOURCE_DIR ${mylib_source_dir}
  
  TEST_COMMAND ""
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/install
             -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
             -DCMAKE_RANLIB=${CMAKE_RANLIB}
             -DCMAKE_AR=${CMAKE_AR}
             -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
             -DCMAKE_EXE_LINKER_FLAGS=${options}
             -DCMAKE_C_COMPILER_WORKS=1
             -DCMAKE_CXX_COMPILER_WORKS=1
)


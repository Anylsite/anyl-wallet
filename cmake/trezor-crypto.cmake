# clones & builds `trezor-crypto` library
include(ExternalProject)

set(mylib_build_dir ${CMAKE_CURRENT_BINARY_DIR}/trezor-crypto)
set(mylib_source_dir ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/trezor-crypto)

ExternalProject_Add(
  trezor-crypto-lib

  PREFIX     ${mylib_build_dir} # Root dir for entire project
  GIT_REPOSITORY "https://github.com/trezor/trezor-crypto.git"
  GIT_TAG "master"
  
  UPDATE_COMMAND ""
  PATCH_COMMAND ${CMAKE_COMMAND} -E copy
  "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/trezor-crypto/CMakeLists.txt" ${mylib_source_dir}/CMakeLists.txt
  
  SOURCE_DIR ${mylib_source_dir}
  
  TEST_COMMAND ""
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/install
)

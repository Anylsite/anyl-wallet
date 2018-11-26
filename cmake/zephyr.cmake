# Zephyr build dependencies
# external lib build example
# https://github.com/zephyrproject-rtos/zephyr/blob/master/samples/application_development/external_lib/CMakeLists.txt#L17
set_property(GLOBAL PROPERTY CSTD c11)
include(ExternalProject)

zephyr_get_include_directories_for_lang_as_string(       C includes)
zephyr_get_system_include_directories_for_lang_as_string(C system_includes)
zephyr_get_compile_definitions_for_lang_as_string(       C definitions)
zephyr_get_compile_options_for_lang_as_string(           C options)

# extend global cflags with zephyr includes etc.
target_sources(app PRIVATE "app/main.c")
set(external_project_cflags
  "${includes} ${definitions} ${options} ${system_includes}"
  )
# set CFLAGS for both root-project (which is the app the user is going to build)
#   and also for a local scope
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${external_project_cflags}" PARENT_SCOPE)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${external_project_cflags}")

#set_target_properties(app PROPERTIES COMPILE_FLAGS "-Wall -Wextra")
#include_directories(zephyr/)
#include_directories(${CMAKE_CURRENT_BINARY_DIR}/zephyr/)

#add_dependencies(app trezor-crypto-lib)
#add_dependencies(app wallet)
#target_link_libraries(app PUBLIC wallet trezor-crypto)

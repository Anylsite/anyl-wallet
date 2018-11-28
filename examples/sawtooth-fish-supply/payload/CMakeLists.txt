cmake_minimum_required(VERSION 3.0)
# compile protobuf sources into C

nanopb_compile(${CMAKE_CURRENT_SOURCE_DIR}/protos/agent.proto AGENT_SRC AGENT_HDR)
nanopb_compile(${CMAKE_CURRENT_SOURCE_DIR}/protos/payload.proto PAYLOAD_SRC PAYLOAD_HDR)
nanopb_compile(${CMAKE_CURRENT_SOURCE_DIR}/protos/property.proto PROPERTY_SRC PROPERTY_HDR)
nanopb_compile(${CMAKE_CURRENT_SOURCE_DIR}/protos/proposal.proto PROPOSAL_SRC PROPOSAL_HDR)
nanopb_compile(${CMAKE_CURRENT_SOURCE_DIR}/protos/record.proto RECORD_SRC RECORD_HDR)
file(GLOB LIB_SOURCES "*.c")

# create an archive
add_library(fish-demo STATIC ${LIB_SOURCES} ${AGENT_SRC} ${PAYLOAD_SRC} ${PROPERTY_SRC} ${PROPOSAL_SRC} ${RECORD_SRC})
target_link_libraries(fish-demo wallet sawtooth eth trezor-crypto helpers)
target_include_directories(fish-demo PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

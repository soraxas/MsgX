cmake_minimum_required(VERSION 3.0)

set(LIB_name msgx_capnp)
set(CAPNP_MSGS msgx.capnp ndarray.capnp)
capnp_generate_cpp(CAPNP_SRC CAPNP_HEADER ${CAPNP_MSGS})

# example library with sources and headers located at ${MY_LIB_SOURCE_DIR}
add_library(${LIB_name} ${CAPNP_SRC})
target_link_libraries(${LIB_name} CapnProto::capnp)
# allows linked target to access the generated header
target_include_directories(${LIB_name} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

install(FILES ${CAPNP_HEADER} DESTINATION "include")

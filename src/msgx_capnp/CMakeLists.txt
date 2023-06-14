cmake_minimum_required(VERSION 3.23)

set(LIB_name msgx_capnp)
set(CAPNP_MSGS msgx.capnp)
capnp_generate_cpp(CAPNP_SRC CAPNP_HEADER ${CAPNP_MSGS})

# include_directories(${CAPNP_INCLUDE_DIRS})
# add_definitions(${CAPNP_DEFINITIONS})
#
# include_directories(${CMAKE_SOURCE_DIR}/include)
#
# Build library add_library(${LIB_name} ${CAPNP_SRC})
# target_link_libraries(${LIB_name} CapnProto::capnp) message(${LIB_name}
# PRIVATE ${CMAKE_CURRENT_BINARY_DIR} " rss " ${CAPNP_HEADER})
# target_include_directories(${LIB_name} PRIVATE include)
# target_include_directories(${LIB_name} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
#
# message("hihi" ${CMAKE_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR})
#
# target_include_directories( ${LIB_name} PUBLIC
# $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:> #
# <prefix>/include/mylib )
#
# set(HEADER_FILES
# /home/soraxas/git-repo/MsgX/build/src/msgx_capnp//msgx.capnp.h )
#
# Installation setup set_target_properties( ${LIB_name} PROPERTIES PUBLIC_HEADER
# "${HEADER_FILES}" IMPORTED_LINK_DEPENDENT_LIBRARIES "${LINK_LIBARARIES}")
#
# install( TARGETS ${LIB_name} EXPORT ${PROJECT_NAME}Targets LIBRARY DESTINATION
# ${INSTALL_LIB_DIR} PUBLIC_HEADER DESTINATION ${INSTALL_INCLUDE_DIR} INCLUDES
# DESTINATION ${INSTALL_INCLUDE_DIR})
#
# install(DIRECTORY "msgx/" # source directory DESTINATION
# ${INSTALL_INCLUDE_DIR} # target directory FILES_MATCHING # install only
# matched files PATTERN "*.h" # select header files )

# example library with sources and headers located at ${MY_LIB_SOURCE_DIR}
add_library(${LIB_name} ${CAPNP_SRC})
target_link_libraries(${LIB_name} CapnProto::capnp)
target_include_directories(${LIB_name} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# source files get added as before target_sources(${LIB_name} PUBLIC
# ${CAPNP_SRC})

# header files get added via FILE_SET BASE_DIR should point to the same
# directory that is used as the target_include_directory, otherwise the install
# paths may be off.
target_sources(
  ${LIB_name} PUBLIC FILE_SET HEADERS BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR}
                     FILES ${CAPNP_HEADER})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# install command needs to reference the file set again
install(TARGETS ${LIB_name} FILE_SET HEADERS)

# message(${CMAKE_CURRENT_BINARY_DIR} __hahahahahaaaa${CAPNP_HEADER})

#
# example library with sources and headers located at ${MY_LIB_SOURCE_DIR}
# add_library(${LIB_name}) target_include_directories(${LIB_name} PUBLIC
# ${MY_LIB_SOURCE_DIR})
#
# source files get added as before target_sources(${LIB_name} PUBLIC
# ${MY_LIB_SOURCE_DIR}/${LIB_name}/${LIB_name}.cpp)
#
# header files get added via FILE_SET BASE_DIR should point to the same
# directory that is used as the target_include_directory, otherwise the install
# paths may be off. target_sources(${LIB_name} PUBLIC FILE_SET HEADERS BASE_DIRS
# ${MY_LIB_SOURCE_DIR} FILES ${MY_LIB_SOURCE_DIR}/${LIB_name}/${LIB_name}.hpp)
#
# install command needs to reference the file set again install(TARGETS
# ${LIB_name} FILE_SET HEADERS)

#
# install the generated protobuf header get_target_property(PROTO_HEADER
# proto_plotmsg_cpp PUBLIC_HEADER) install(FILES ${PROTO_HEADER} DESTINATION
# ${INSTALL_INCLUDE_DIR}/_impl )
#
# message(FATAL_ERROR ${PROTO_HEADER} )
#
# Add all targets to the build-tree export set export( TARGETS ${LIB_name}
# APPEND FILE "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")

cmake_minimum_required(VERSION 3.5.1)
project(MsgX CXX)
set(CMAKE_CXX_STANDARD 14)


# add dependencies
include(cmake/CPM.cmake)

#CPMAddPackage("gh:fmtlib/fmt#7.1.3")
#CPMAddPackage("gh:nlohmann/json@3.10.5")
#CPMAddPackage("gh:catchorg/Catch2@3.2.1")
CPMAddPackage("gh:capnproto/capnproto@0.10.4")

find_package(cppzmq QUIET)
#find_package(Protobuf REQUIRED)
#find_package(CapnProto)

#set(CapnProto_DIR )

#find_package(CapnProto REQUIRED)
#find_package(CapnProto )
#include_directories(${CAPNP_INCLUDE_DIRS})
#add_definitions(${CAPNP_DEFINITIONS})


## Set output directories for libraries and executables
#set(BASE_DIR ${CMAKE_SOURCE_DIR})
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BASE_DIR}/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BASE_DIR}/bin)
#set(PYTHON_PKG_INSTALL_DIR ${BASE_DIR}/built_python_pkg)

# Installation locations
set(INSTALL_LIB_DIR ${CMAKE_INSTALL_PREFIX}/lib)
set(INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include/msgx)
set(INSTALL_CMAKE_DIR ${CMAKE_INSTALL_PREFIX}/lib/cmake/msgx)

# +-----------------------------------------------------------------------------
# | Build configuration options
# +-----------------------------------------------------------------------------
option(BUILD_CTAGS "Build ctag file?" FALSE)
option(RUN_TESTS "Run Tests?" FALSE)

# +-----------------------------------------------------------------------------
# | Library search and setup
# +-----------------------------------------------------------------------------

# Build shared libraries by default
#set(BUILD_SHARED_LIBS True)



## Use local include and library directories
#include_directories(${PROJECT_SOURCE_DIR})
#link_directories(${BASE_DIR}/lib)

# +-----------------------------------------------------------------------------
# | Custom targets and macros
# +-----------------------------------------------------------------------------

if (BUILD_CTAGS)
    # Custom target that builds ctags for the source code
    add_custom_target(
            generate_ctags ALL
            ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .
            WORKING_DIRECTORY ${BASE_DIR}/src
            COMMENT "Regenerating ctags file")
endif ()


#include_directories(${CAPNP_INCLUDE_DIRS})
#add_definitions(${CAPNP_DEFINITIONS})

#   capnp_generate_cpp(CAPNP_SRCS CAPNP_HDRS schema.capnp)
#   add_executable(a a.cc ${CAPNP_SRCS} ${CAPNP_HDRS})
#   target_link_library(a ${CAPNP_LIBRARIES})
#

# +-----------------------------------------------------------------------------
# | Compile code
# +-----------------------------------------------------------------------------

include_directories(include)

add_subdirectory(src)


add_executable(test main.c++)
target_link_libraries(test msgx)
#target_link_libraries(test msgx)

## python scripts
#add_custom_target(
#  copy-python-pkg-template ALL
#  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/scripts
#          ${PYTHON_PKG_INSTALL_DIR}/
#  DEPENDS ${MY_TARGET})
#add_dependencies(proto_msgx_py copy-python-pkg-template)
## configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/scripts/
## ${PYTHON_PKG_INSTALL_DIR}/msgx.py )

## demo
#if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
#  # only build demo if this is the root project
#  add_executable(pub "publisher_example.cpp")
#  target_link_libraries(pub msgx ${link_eigen})
#
#  find_package(Eigen3)
#  if(TARGET Eigen3::Eigen)
#    # Use the imported target
#    set(link_eigen Eigen3::Eigen)
#    add_definitions(-DWITH_EIGEN)
#
#    target_include_directories(pub PRIVATE ${EIGEN3_INCLUDE_DIR})
#  endif(TARGET Eigen3::Eigen)
#endif()

# +-----------------------------------------------------------------------------
# | Installation setup
# +-----------------------------------------------------------------------------

## Export the package for use from the build tree. This registers the build tree
## with a global CMake registry.
#export(PACKAGE ${PROJECT_NAME})
#
## Determine the relative path between CMake configuration file installation
## location and header files installation directory
#file(RELATIVE_PATH REL_INCLUDE_DIR ${INSTALL_CMAKE_DIR} ${INSTALL_INCLUDE_DIR})
#
## Create the ${ProjectName}Config.cmake file for the build tree.
#set(CONF_INCLUDE_DIRS ${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR})
#configure_file(${PROJECT_NAME}Config.cmake.in
#        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" @ONLY)
#
## Create the ${PROJECT_NAME}Config.cmake file for the install tree.
#set(CONF_INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/include)
#configure_file(
#        ${PROJECT_NAME}Config.cmake.in
#        "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake"
#        @ONLY)
#
## Install the ${PROJECT_NAME}Config.cmake file
#install(
#        FILES
#        "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake"
#        DESTINATION "${INSTALL_CMAKE_DIR}"
#        COMPONENT dev)
#
## Install the export set for use with the install tree
#install(
#        EXPORT ${PROJECT_NAME}Targets
#        #  EXPORT ${PROJECT_NAME}Targets
#        DESTINATION "${INSTALL_CMAKE_DIR}"
#        COMPONENT dev)

# include_directories(${CMAKE_SOURCE_DIR}/include)

message(include_directories (${CMAKE_SOURCE_DIR}/include)
        okok${CAPNP_INCLUDE_DIRS}haie)

set(SOURCE_FILES main.cpp opaque_item/mappings.cpp opaque_item/item.cpp
                 opaque_item/bindable_item.cpp)
set(HEADER_FILES
    ${CAPNP_HEADER}
    # msgx/main.hpp msgx/_impl/core.hpp msgx/_impl/dictionary.hpp
    # msgx/_impl/figure.hpp msgx/_impl/trace.hpp msgx/_impl/series_any.hpp
    # msgx/_impl/index_proxy_access.hpp msgx/_impl/helpers.hpp
    # msgx/template/core.hpp msgx/template/ompl.hpp msgx/ms /mappings.h
)
set(LINK_LIBARARIES CapnProto::capnp ${CAPNP_LIBRARIES} zmq ${ZMQ_LIBRARIES}
                    msgx_capnp)

find_package(spdlog REQUIRED)
add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)

#
# target_link_libraries(${PROJECT_NAME} )
# target_include_directories(${PROJECT_NAME} PRIVATE
# ${CMAKE_CURRENT_BINARY_DIR})

# Build library
add_library(msgx ${SOURCE_FILES})
target_link_libraries(msgx PUBLIC ${LINK_LIBARARIES})
target_link_libraries(msgx PUBLIC spdlog::spdlog doctest::doctest)

# CPMAddPackage("gh:doctest/doctest@2.4.11")
# include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
#
# add_executable(exe_with_static_libs ../../tests/msgx_test.c++ ${SOURCE_FILES})
# target_link_libraries(exe_with_static_libs doctest::doctest
# ${LINK_LIBARARIES}) doctest_discover_tests(exe_with_static_libs)

# target_include_directories(msgx PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
# target_include_directories(msgx PRIVATE include)

message("hihi" ${CMAKE_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR})
#
# target_include_directories( msgx PUBLIC
# $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:> #
# <prefix>/include/mylib )
#
# Installation setup set_target_properties( msgx PROPERTIES PUBLIC_HEADER
# "${HEADER_FILES}" IMPORTED_LINK_DEPENDENT_LIBRARIES "${LINK_LIBARARIES}")
#
# install( TARGETS msgx EXPORT ${PROJECT_NAME}Targets LIBRARY DESTINATION
# ${INSTALL_LIB_DIR} PUBLIC_HEADER DESTINATION ${INSTALL_INCLUDE_DIR} INCLUDES
# DESTINATION ${INSTALL_INCLUDE_DIR})
#
# install(DIRECTORY "msgx/" # source directory DESTINATION
# ${INSTALL_INCLUDE_DIR} # target directory FILES_MATCHING # install only
# matched files PATTERN "*.h" # select header files )
#
# install the generated protobuf header get_target_property(PROTO_HEADER
# proto_plotmsg_cpp PUBLIC_HEADER) install(FILES ${PROTO_HEADER} DESTINATION
# ${INSTALL_INCLUDE_DIR}/_impl )
#
# message(FATAL_ERROR ${PROTO_HEADER} )
#
# Add all targets to the build-tree export set export( TARGETS msgx APPEND FILE
# "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")

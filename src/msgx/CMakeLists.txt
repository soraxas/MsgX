# include_directories(${CMAKE_SOURCE_DIR}/include)

message(include_directories (${CMAKE_SOURCE_DIR}/include)
        okok${CAPNP_INCLUDE_DIRS}haie)

set(SOURCE_FILES main.cpp opaque_item/mappings.cpp opaque_item/item.cpp
                 opaque_item/bindable_item.cpp)
set(HEADER_FILES ${CAPNP_HEADER} ../../include/msgx/message.h)
set(PUBLIC_LINK_LIBARARIES CapnProto::capnp ${CAPNP_LIBRARIES} msgx_capnp)
set(PRIVATE_LINK_LIBARARIES zmq ${ZMQ_LIBRARIES} msgx_capnp spdlog::spdlog)

# target_link_libraries(${PROJECT_NAME} )
# target_include_directories(${PROJECT_NAME} PRIVATE
# ${CMAKE_CURRENT_BINARY_DIR})

# Build library
add_library(msgx ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(msgx PUBLIC ${PUBLIC_LINK_LIBARARIES})
target_link_libraries(msgx PRIVATE ${PUBLIC_LINK_LIBARARIES})

target_include_directories(
  msgx PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
              $<INSTALL_INTERFACE:> <prefix>/include/msgx)

target_include_directories(msgx PUBLIC ../../include)

# Installation setup
set_target_properties(
  msgx PROPERTIES PUBLIC_HEADER "${HEADER_FILES}"
                  IMPORTED_LINK_DEPENDENT_LIBRARIES "${PUBLIC_LINK_LIBARARIES}")

install(
  TARGETS msgx
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION ${INSTALL_LIB_DIR}
  PUBLIC_HEADER DESTINATION ${INSTALL_INCLUDE_DIR}
  INCLUDES
  DESTINATION ${INSTALL_INCLUDE_DIR})

install(
  DIRECTORY "msgx/" # source directory
  DESTINATION ${INSTALL_INCLUDE_DIR} # target directory
  FILES_MATCHING # install only matched files
  PATTERN "*.h" # select header files
)

# install the generated protobuf header get_target_property(PROTO_HEADER
# proto_plotmsg_cpp PUBLIC_HEADER) install(FILES ${PROTO_HEADER} DESTINATION
# ${INSTALL_INCLUDE_DIR}/_impl )

# message(FATAL_ERROR ${PROTO_HEADER} )

# Add all targets to the build-tree export set
export(
  TARGETS msgx
  APPEND
  FILE "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")

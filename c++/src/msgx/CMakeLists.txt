set(SOURCE_FILES
    # cmake-format: sortable
    comms/zmq.c++
    dispatcher/builder_adopt_dispatcher.c++
    kwargs.c++
    message.c++
    opaque_item/bindable_item.c++
    opaque_item/item.c++
    opaque_item/mappings.c++
)
set(HEADER_FILES
    # cmake-format: sortable
    ${CAPNP_HEADER}
    ../../include/msgx/conversion/anylist_interface.h
    ../../include/msgx/conversion/mapping_interface.h
    ../../include/msgx/dispatcher/builder_adopt_dispatcher.h
    ../../include/msgx/message.h
)
set(PUBLIC_LINK_LIBRARIES CapnProto::capnp ${CAPNP_LIBRARIES} msgx_capnp zmq
                          ${ZMQ_LIBRARIES}
                          )

if (BUILD_WITH_SPDLOG)
  set(PUBLIC_LINK_LIBRARIES ${PUBLIC_LINK_LIBRARIES} spdlog::spdlog)
endif()

# set(PRIVATE_LINK_LIBRARIES spdlog::spdlog)

# target_link_libraries(${PROJECT_NAME} )
# target_include_directories(${PROJECT_NAME} PRIVATE
# ${CMAKE_CURRENT_BINARY_DIR})

# Build library
add_library(msgx ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(msgx PUBLIC ${PUBLIC_LINK_LIBRARIES})
target_link_libraries(msgx PRIVATE ${PUBLIC_LINK_LIBRARIES})

target_include_directories(
  msgx PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
              $<INSTALL_INTERFACE:> <prefix>/include/msgx ../../include
)

# Installation setup
set_target_properties(
  msgx PROPERTIES PUBLIC_HEADER "${HEADER_FILES}" #
                  IMPORTED_LINK_DEPENDENT_LIBRARIES "${PUBLIC_LINK_LIBRARIES}"
)

install(
  TARGETS msgx
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION ${INSTALL_LIB_DIR}
  PUBLIC_HEADER DESTINATION ${INSTALL_INCLUDE_DIR}
  INCLUDES
  DESTINATION ${INSTALL_INCLUDE_DIR}
)

install(
  DIRECTORY "msgx/" # source directory
  DESTINATION ${INSTALL_INCLUDE_DIR} # target directory
  FILES_MATCHING # install only matched files
  PATTERN "*.h" # select header files
)

# Add all targets to the build-tree export set
export(
  TARGETS msgx
  APPEND
  FILE "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake"
)

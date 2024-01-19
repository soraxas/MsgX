set(SOURCE_FILES
    # cmake-format: sortable
    comms/zmq.c++
    dispatcher/builder_adopt_dispatcher.c++
    kwargs.c++
    message.c++
    opaque_item/bindable_item.c++
    opaque_item/item.c++
    opaque_item/mappings.c++
    conversion/anylist_interface.c++
)
file(GLOB_RECURSE HEADER_FILES  ../../include/**/*.h )
set(HEADER_FILES ${HEADER_FILES} ${CAPNP_HEADER})


set(PUBLIC_LINK_LIBRARIES CapnProto::capnp ${CAPNP_LIBRARIES} msgx_capnp zmq
                          ${ZMQ_LIBRARIES}
                          )
set(PRIVATE_LINK_LIBRARIES cppzmq)

if (BUILD_WITH_SPDLOG)
  set(PUBLIC_LINK_LIBRARIES ${PUBLIC_LINK_LIBRARIES} easyspdlog)
#  set(PUBLIC_LINK_LIBRARIES ${PUBLIC_LINK_LIBRARIES} spdlog::spdlog)
endif()

# set(PRIVATE_LINK_LIBRARIES spdlog::spdlog)

# target_link_libraries(${PROJECT_NAME} )
# target_include_directories(${PROJECT_NAME} PRIVATE
# ${CMAKE_CURRENT_BINARY_DIR})

# Build library
add_library(msgx ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(msgx PUBLIC ${PUBLIC_LINK_LIBRARIES})
target_link_libraries(msgx PRIVATE ${PRIVATE_LINK_LIBRARIES})


target_include_directories(
  msgx PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
              # $<INSTALL_INTERFACE:> <prefix>/include/msgx ../../include
)

# Installation setup
# set_target_properties(
#   msgx PROPERTIES PUBLIC_HEADER "${HEADER_FILES}" #
#                   IMPORTED_LINK_DEPENDENT_LIBRARIES "${PUBLIC_LINK_LIBRARIES}"
# )

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../include/"
        DESTINATION "include"
        FILES_MATCHING
        PATTERN "*.h*"
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

install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        DESTINATION ${INSTALL_CMAKE_DIR})

# install(
#   FILES
#     "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake"
#   DESTINATION "${INSTALL_CMAKE_DIR}"
#   COMPONENT dev
# )
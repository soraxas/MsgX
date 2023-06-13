

CPMAddPackage("gh:doctest/doctest@2.4.11")

add_executable(msgx_test msgx_test.c++)


set(CMAKE_MODULE_PATH "${doctest_SOURCE_DIR}/scripts/cmake" ${CMAKE_MODULE_PATH})

#ExternalProject_Get_Property(doctest source_dir)
#set(DOCTEST_INCLUDE_DIR ${source_dir}/doctest CACHE INTERNAL "Path to include folder for doctest")
#target_include_directories(msgx_test PUBLIC ${DOCTEST_INCLUDE_DIR})

add_dependencies(msgx_test doctest)

target_link_libraries(msgx_test msgx doctest::doctest)

enable_testing()
include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
doctest_discover_tests(msgx_test)
cpmaddpackage("gh:doctest/doctest@2.4.11")

# format code
cpmaddpackage("gh:TheLartians/Format.cmake@1.7.3")

add_executable(msgx_test msgx_test.c++)

set(CMAKE_MODULE_PATH "${doctest_SOURCE_DIR}/scripts/cmake"
                      ${CMAKE_MODULE_PATH})

add_dependencies(msgx_test doctest)

target_link_libraries(msgx_test msgx doctest::doctest)

enable_testing()
include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
doctest_discover_tests(msgx_test)

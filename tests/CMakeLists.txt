cpmaddpackage("gh:doctest/doctest@2.4.11")

# format code
cpmaddpackage("gh:TheLartians/Format.cmake@1.7.3")

# Eigen
find_package(Eigen3 3 QUIET)
if (NOT Eigen3_FOUND)
  cpmaddpackage(
    NAME
    Eigen
    VERSION
    3
    DOWNLOAD_ONLY
    YES
    URL
    https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz
  )
  if (Eigen_ADDED)
    add_library(Eigen3::Eigen INTERFACE IMPORTED)
    target_include_directories(Eigen3::Eigen INTERFACE ${Eigen_SOURCE_DIR})
  endif ()
endif ()

add_executable(msgx_test msgx_test.c++ test_eigen.c++ test_primitives_lists.c++)

set(CMAKE_MODULE_PATH "${doctest_SOURCE_DIR}/scripts/cmake"
                      ${CMAKE_MODULE_PATH}
)

add_dependencies(msgx_test doctest)

target_link_libraries(msgx_test msgx doctest::doctest)
target_link_libraries(msgx_test msgx Eigen3::Eigen)

enable_testing()
include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
doctest_discover_tests(msgx_test)

cmake_minimum_required(VERSION 3.26)

project(exodusIIcpp
    VERSION 3.0.0
    LANGUAGES CXX C
)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(FetchContent)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(CodeCoverage)
include(Sanitization)

set(LibraryTypeValues "SHARED;STATIC")
if(NOT EXODUSIICPP_LIBRARY_TYPE)
    set(EXODUSIICPP_LIBRARY_TYPE "SHARED" CACHE STRING "Build as a shared or static library." FORCE)
endif()
set_property(CACHE EXODUSIICPP_LIBRARY_TYPE PROPERTY STRINGS ${LibraryTypeValues})

option(EXODUSIICPP_BUILD_TESTS "Build tests" NO)
option(EXODUSIICPP_BUILD_TOOLS "Build tools" YES)
option(EXODUSIICPP_INSTALL "Install the library" ON)
option(EXODUSIICPP_WITH_PYTHON "Build python wrapper" NO)
mark_as_advanced(FORCE EXODUSIICPP_INSTALL)

find_package(fmt 11 REQUIRED)
find_package(NetCDF 4.9 REQUIRED)
find_package(HDF5 1.10 REQUIRED COMPONENTS C)
find_package(ExodusII REQUIRED)

add_subdirectory(src)
if (EXODUSIICPP_BUILD_TOOLS)
    add_subdirectory(tools)
endif()

if (EXODUSIICPP_WITH_PYTHON)
    add_subdirectory(python)
endif()

# Tests

if (EXODUSIICPP_BUILD_TESTS)
    set(EXODUSIICPP_UNIT_TEST_ASSETS ${CMAKE_SOURCE_DIR}/test/assets CACHE PATH "" FORCE)
    mark_as_advanced(FORCE EXODUSIICPP_UNIT_TEST_ASSETS)

    enable_testing()
    add_subdirectory(test)
endif()

add_subdirectory(docs)

cmake_minimum_required(VERSION 3.16)

project(exodusIIcpp
    VERSION 1.5
    LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(FetchContent)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(${PROJECT_SOURCE_DIR}/cmake/CodeCoverage.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/Sanitization.cmake)

set(LibraryTypeValues "SHARED;STATIC")
if(NOT EXODUSIICPP_LIBRARY_TYPE)
    set(EXODUSIICPP_LIBRARY_TYPE "SHARED" CACHE STRING "Build as a shared or static library." FORCE)
endif()
set_property(CACHE EXODUSIICPP_LIBRARY_TYPE PROPERTY STRINGS ${LibraryTypeValues})

option(EXODUSIICPP_BUILD_TESTS "Build tests" NO)

find_package(fmt 8.0 REQUIRED)
find_package(NetCDF 4.5 REQUIRED)
find_package(HDF5 1.10 REQUIRED COMPONENTS C)
find_package(ExodusII REQUIRED)

if (EXODUSIICPP_BUILD_TESTS)
    FetchContent_Declare(
      googletest
      GIT_REPOSITORY https://github.com/google/googletest.git
      GIT_TAG        release-1.11.0
    )
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    mark_as_advanced(FORCE
        BUILD_GMOCK
        INSTALL_GTEST
        FETCHCONTENT_SOURCE_DIR_GOOGLETEST
        FETCHCONTENT_UPDATES_DISCONNECTED_GOOGLETEST
    )

    FetchContent_MakeAvailable(googletest)
    mark_as_advanced(FORCE
        FETCHCONTENT_BASE_DIR
        FETCHCONTENT_FULLY_DISCONNECTED
        FETCHCONTENT_QUIET
        FETCHCONTENT_UPDATES_DISCONNECTED
    )
endif()

add_subdirectory(src)

# Tests

if (EXODUSIICPP_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

add_subdirectory(docs)

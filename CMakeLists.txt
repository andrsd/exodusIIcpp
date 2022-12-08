cmake_minimum_required(VERSION 3.16)

project(exodusIIcpp
    VERSION 1.2.0
    LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(FetchContent)
include(${PROJECT_SOURCE_DIR}/cmake/CodeCoverage.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/Sanitization.cmake)

option(EXODUSIICPP_BUILD_TESTS "Build tests" NO)

find_package(fmt REQUIRED)
find_package(NetCDF REQUIRED)
find_package(HDF5 COMPONENTS C)
find_package(ExodusII REQUIRED)

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

# Library

add_library(${PROJECT_NAME}
    SHARED
        src/exodusIIcppElementBlock.cpp
        src/exodusIIcppFile.cpp
        src/exodusIIcppNodeSet.cpp
        src/exodusIIcppSideSet.cpp
)
if(CMAKE_PROJECT_NAME STREQUAL "exodusIIcpp")
target_code_coverage(${PROJECT_NAME})
target_sanitization(${PROJECT_NAME})
endif()

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/exodusIIcpp>
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        fmt::fmt
    PRIVATE
        ${EXODUSII_LIBRARY}
        ${NETCDF_LIBRARY}
        ${HDF5_LIBRARIES}
)

# Tests

if (EXODUSIICPP_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

add_subdirectory(docs)

# Install

install(
    TARGETS exodusIIcpp
    EXPORT exodusIIcppTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    FILES_MATCHING PATTERN "*.h"
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(exodusIIcppConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(
    EXPORT exodusIIcppTargets
    FILE exodusIIcppTargets.cmake
    NAMESPACE exodusIIcpp::
    DESTINATION lib/cmake/exodusIIcpp
)

install(
    FILES
        "${PROJECT_SOURCE_DIR}/cmake/exodusIIcppConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/exodusIIcppConfigVersion.cmake"
    DESTINATION
        lib/cmake/exodusIIcpp
)

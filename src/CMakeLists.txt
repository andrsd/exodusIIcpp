project(exodusIIcpp
    VERSION 1.0
    LANGUAGES CXX
)

add_library(${PROJECT_NAME}
    SHARED
        exodusIIcppElementBlock.cpp
        exodusIIcppFile.cpp
        exodusIIcppNodeSet.cpp
        exodusIIcppSideSet.cpp
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
        fmt::fmt
        ${NETCDF_LIBRARY}
        ${EXODUSII_LIBRARY}
        ${HDF5_LIBRARIES}
)

install(
    TARGETS
        exodusIIcpp
    EXPORT
        exodusIIcppTargets
    LIBRARY DESTINATION
        lib
    ARCHIVE DESTINATION
        lib
    RUNTIME DESTINATION
        bin
    INCLUDES DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}
    PUBLIC_HEADER DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    DIRECTORY
        ${PROJECT_SOURCE_DIR}
    DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING
        PATTERN "*.h"
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    exodusIIcppConfigVersion.cmake
    VERSION
        ${PROJECT_VERSION}
    COMPATIBILITY
        AnyNewerVersion
)

install(
    EXPORT
        exodusIIcppTargets
    FILE
        exodusIIcppTargets.cmake
    NAMESPACE
        exodusIIcpp::
    DESTINATION
        lib/cmake/exodusIIcpp
)

install(
    FILES
        "${CMAKE_SOURCE_DIR}/cmake/exodusIIcppConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/exodusIIcppConfigVersion.cmake"
    DESTINATION
        lib/cmake/exodusIIcpp
)

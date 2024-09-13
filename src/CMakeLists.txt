add_library(
    ${PROJECT_NAME}
    ${EXODUSIICPP_LIBRARY_TYPE}
        element_block.cpp
        exception.cpp
        file.cpp
        node_set.cpp
        side_set.cpp
)
if(CMAKE_PROJECT_NAME STREQUAL "exodusIIcpp")
    target_code_coverage(${PROJECT_NAME})
    target_sanitization(${PROJECT_NAME})
endif()

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        POSITION_INDEPENDENT_CODE ON
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        $<BUILD_INTERFACE:${EXODUSII_INCLUDE_DIR}>
        $<BUILD_INTERFACE:${NETCDF_INCLUDE_DIRS}>
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
        fmt::fmt
        exodusii::exodusii
    PRIVATE
        ${NETCDF_LIBRARY}
        ${HDF5_LIBRARIES}
)

# Install

configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/cmake/exodusiicpp-config.cmake.in
    exodusiicpp-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/exodusIIcpp
    NO_SET_AND_CHECK_MACRO
)
write_basic_package_version_file(exodusiicpp-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

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
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h"
)

install(
    EXPORT exodusIIcppTargets
    FILE exodusiicpp-targets.cmake
    NAMESPACE exodusIIcpp::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/exodusIIcpp
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/exodusiicpp-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/exodusiicpp-config-version.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/exodusIIcpp
)

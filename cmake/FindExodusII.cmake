# Find ExodusII
#
# Once done this will define
#  EXODUSII_FOUND - System has ExodusII
#  EXODUSII_INCLUDE_DIR - The ExodusII include directory
#  EXODUSII_LIBRARY - The ExodusII library
#  EXODUSII_VERSION - The ExodusII version

find_path(
    EXODUSII_INCLUDE_DIR
        exodusII.h
    PATHS
        $ENV{EXODUSII_DIR}/include
)

find_library(
    EXODUSII_LIBRARY
        exodus exoIIv2c
    PATHS
        $ENV{EXODUSII_DIR}/lib
)

set(EXODUSII_VERSION "unknown")
find_file(EXODUSII_H exodusII.h
    PATHS
        $ENV{EXODUSII_DIR}/include
)
mark_as_advanced(EXODUSII_H)
if (EXODUSII_H)
    file(READ ${EXODUSII_H} EXODUSII_H_FILE)
    string(REGEX MATCH "define[ ]+EXODUS_VERSION_MAJOR[ ]+([0-9]+)" TMP "${EXODUSII_H_FILE}")
    if (CMAKE_MATCH_1)
        set(EXODUSII_VERSION_MAJOR ${CMAKE_MATCH_1})
    endif()
    string(REGEX MATCH "define[ ]+EXODUS_VERSION_MINOR[ ]+([0-9]+)" TMP "${EXODUSII_H_FILE}")
    if (CMAKE_MATCH_1)
        set(EXODUSII_VERSION_MINOR ${CMAKE_MATCH_1})
    endif()
    if (EXODUSII_VERSION_MAJOR AND EXODUSII_VERSION_MINOR)
        set(EXODUSII_VERSION "${EXODUSII_VERSION_MAJOR}.${EXODUSII_VERSION_MINOR}")
    endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    ExodusII
    REQUIRED_VARS EXODUSII_LIBRARY EXODUSII_INCLUDE_DIR
    VERSION_VAR EXODUSII_VERSION
)

mark_as_advanced(FORCE
    EXODUSII_INCLUDE_DIR
    EXODUSII_LIBRARY
)

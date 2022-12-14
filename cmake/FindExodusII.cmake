# Find ExodusII
#
# Once done this will define
#  EXODUSII_FOUND - System has ExodusII
#  EXODUSII_INCLUDE_DIR - The ExodusII include directory
#  EXODUSII_LIBRARY - The ExodusII library

find_path(
    EXODUSII_INCLUDE_DIR
        exodusII.h
    PATHS
        $ENV{EXODUSII_DIR}/include
)

find_library(
    EXODUSII_LIBRARY
        exoIIv2c exodus
    PATHS
        $ENV{EXODUSII_DIR}/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    ExodusII
    DEFAULT_MSG
    EXODUSII_LIBRARY
    EXODUSII_INCLUDE_DIR
)

mark_as_advanced(FORCE
    EXODUSII_INCLUDE_DIR
    EXODUSII_LIBRARY
)

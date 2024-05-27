# Find NetCDF
#
# Once done this will define
#  NETCDF_FOUND - System has NetCDF
#  NETCDF_INCLUDE_DIRS - The NetCDF include directories
#  NETCDF_LIBRARY - The NetCDF library
#  NETCDF_VERSION - The NetCDF library

find_path(
    NETCDF_INCLUDE_DIR
        netcdf.h
    PATHS
        $ENV{NETCDF_DIR}/include
)

find_library(
    NETCDF_LIBRARY
        netcdf
    PATHS
        $ENV{NETCDF_DIR}/lib
)
set(NETCDF_INCLUDE_DIRS ${NETCDF_INCLUDE_DIR})

set(NETCDF_VERSION "unknown")
find_file(NETCDF_META_H netcdf_meta.h
    PATHS
        $ENV{NETCDF_DIR}/include
)
mark_as_advanced(NETCDF_META_H)
if (NETCDF_META_H)
    file(READ ${NETCDF_META_H} NETCDF_META_FILE)
    string(REGEX MATCH "define[ ]+NC_VERSION_MAJOR[ ]+([0-9]+)" TMP "${NETCDF_META_FILE}")
    set(NETCDF_VERSION_MAJOR ${CMAKE_MATCH_1})
    string(REGEX MATCH "define[ ]+NC_VERSION_MINOR[ ]+([0-9]+)" TMP "${NETCDF_META_FILE}")
    set(NETCDF_VERSION_MINOR ${CMAKE_MATCH_1})
    string(REGEX MATCH "define[ ]+NC_VERSION_PATCH[ ]+([0-9]+)" TMP "${NETCDF_META_FILE}")
    set(NETCDF_VERSION_PATCH ${CMAKE_MATCH_1})
    set(NETCDF_VERSION "${NETCDF_VERSION_MAJOR}.${NETCDF_VERSION_MINOR}.${NETCDF_VERSION_PATCH}")
    string(REGEX MATCH "define[ ]+NC_HAS_PARALLEL[ ]+([0-9]+)" TMP "${NETCDF_META_FILE}")
    set(NETCDF_PARALLEL ${CMAKE_MATCH_1})
endif()

if (NETCDF_PARALLEL EQUAL "1")
    find_package(MPI REQUIRED)
    list(APPEND NETCDF_INCLUDE_DIRS ${MPI_C_INCLUDE_DIRS})
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    NetCDF
    REQUIRED_VARS NETCDF_LIBRARY NETCDF_INCLUDE_DIR
    VERSION_VAR NETCDF_VERSION
)

mark_as_advanced(FORCE
    NETCDF_INCLUDE_DIRS
    NETCDF_INCLUDE_DIR
    NETCDF_LIBRARY
)

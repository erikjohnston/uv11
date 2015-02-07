cmake_minimum_required(VERSION 3.1)

find_package(PkgConfig)
if (PKG_CONFIG_FOUND)
    pkg_check_modules(PC_LIBUV QUIET libuv>=${LIBUV_FIND_VERSION})
endif()

find_path(LIBUV_INCLUDE_DIR uv.h
    HINTS ${PC_LIBUV_INCLUDEDIR} ${PC_LIBUV_INCLUDE_DIRS} ${EXTERNALS_DIR}/include
)

find_library(
  LIBUV_LIBRARY NAMES uv
  HINTS ${PC_LIBUV_LIBDIR} ${PC_LIBUV_LIBRARY_DIRS} ${EXTERNALS_DIR}/lib
)

find_library(
    LIBUV_STATIC_LIBRARY NAMES ${CMAKE_STATIC_LIBRARY_PREFIX}uv${CMAKE_STATIC_LIBRARY_SUFFIX}
    HINTS ${PC_LIBUV_LIBDIR} ${PC_LIBUV_LIBRARY_DIRS} ${EXTERNALS_DIR}/lib
)

mark_as_advanced(LIBUV_INCLUDE_DIR LIBUV_LIBRARY)

set(LIBUV_LIBRARIES ${LIBUV_LIBRARY})
set(LIBUV_INCLUDE_DIRS ${LIBUV_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    LibUV DEFAULT_MSG
    LIBUV_LIBRARY LIBUV_INCLUDE_DIR
)

if (LIBUV_FOUND)
    add_library(uv SHARED IMPORTED GLOBAL)
    set_target_properties(uv PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${LIBUV_INCLUDE_DIR}"
        IMPORTED_LOCATION "${LIBUV_LIBRARY}"
    )

    add_library(uvStatic STATIC IMPORTED GLOBAL)
    set_target_properties(uvStatic PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${LIBUV_INCLUDE_DIR}"
        IMPORTED_LOCATION "${LIBUV_STATIC_LIBRARY}"
    )
endif(LIBUV_FOUND)
cmake_minimum_required(VERSION 3.1)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUV QUIET libuv>=${LIBUV_FIND_VERSION})

if (NOT LIBUV_FOUND AND NOT LIBUV_STATIC_FOUND)
    MESSAGE(STATUS "Pulling in LibUV from github")
    if (NOT LIBUV_FIND_VERSION)
        set(LIBUV_FIND_VERSION "v1.1.0")
    endif (NOT LIBUV_FIND_VERSION)

    include(ExternalProject)

    if (NOT LIBUV_EXTERNAL_PREFIX)
        set (LIBUV_EXTERNAL_PREFIX ${CMAKE_SOURCE_DIR}/externals/libuv)
    endif (NOT LIBUV_EXTERNAL_PREFIX)

    # Right, lets download it then!
    ExternalProject_Add (
        libuv-proj
        GIT_REPOSITORY "git@github.com:libuv/libuv.git"
        GIT_TAG ${LIBUV_FIND_VERSION}
        INSTALL_DIR ${LIBUV_EXTERNAL_PREFIX}
        UPDATE_COMMAND ""
        CONFIGURE_COMMAND ./autogen.sh COMMAND ./configure --prefix=<INSTALL_DIR>
        BUILD_COMMAND make
        BUILD_IN_SOURCE 1
        INSTALL_COMMAND make install
    )

    # pkg_check_modules(PC_LIBUV QUIET libuv>=${LIBUV_FIND_VERSION})

    # Set up all the different variables pkg_check_modules would
    set(LIBUV_INCLUDE_DIRS ${LIBUV_EXTERNAL_PREFIX}/include)
    set(LIBUV_LIBRARY_DIRS ${LIBUV_EXTERNAL_PREFIX}/lib)
    set(LIBUV_LIBRARIES uv rt pthread nsl dl)
    set(LIBUV_LDFLAGS)
    set(LIBUV_LDFLAGS_OTHER)
    set(LIBUV_CFLAGS)
    set(LIBUV_CFLAGS_OTHER)
    set(LIBUV_VERSION ${LIBUV_FIND_VERSION})
    set(LIBUV_DEPENDENCY libuv-proj)
    set(LIBUV_FOUND True)
    set(LIBUV_LIBDIR ${LIBUV_EXTERNAL_PREFIX}/lib)
endif(NOT LIBUV_FOUND AND NOT LIBUV_STATIC_FOUND)

if (LIBUV_FOUND)
    add_library(LibUV SHARED IMPORTED GLOBAL)
    set(
        LIBUV_LIBRARY
        "${LIBUV_LIBDIR}/${CMAKE_SHARED_LIBRARY_PREFIX}uv${CMAKE_SHARED_LIBRARY_SUFFIX}"
    )
    set_target_properties(LibUV PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${LIBUV_INCLUDE_DIRS}"
        INTERFACE_LINK_LIBRARIES "${LIBUV_LIBRARIES}"
        IMPORTED_LOCATION "${LIBUV_LIBRARY}"
    )
endif(LIBUV_FOUND)

if (LIBUV_STATIC_FOUND)
    add_library(LibUVStatic STATIC IMPORTED GLOBAL)
    set(
        LIBUV_STATIC_LIBRARY
        ${LIBUV_STATIC_LIBDIR} "/" ${CMAKE_STATIC_LIBRARY_PREFIX} "uv" ${CMAKE_STATIC_LIBRARY_SUFFIX}
    )
    set_target_properties(LibUVStatic PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${LIBUV_STATIC_INCLUDE_DIRS}"
        INTERFACE_LINK_LIBRARIES "${LIBUV_STATIC_LIBRARIES}"
        IMPORTED_LOCATION "${LIBUV_STATIC_LIBRARY}"
    )
endif(LIBUV_STATIC_FOUND)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    LibUV DEFAULT_MSG
    LIBUV_LIBRARIES LIBUV_INCLUDE_DIRS
)

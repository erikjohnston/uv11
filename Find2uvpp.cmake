cmake_minimum_required(VERSION 3.1)

find_package(2uvpp CONFIG)

if (NOT 2UVPP_FOUND)
    MESSAGE(STATUS "Pulling in 2uvpp from git")
    include(ExternalProject)

    if (NOT 2UVPP_EXTERNAL_PREFIX)
        set (2UVPP_EXTERNAL_PREFIX ${CMAKE_SOURCE_DIR}/externals/2uvpp)
    endif (NOT 2UVPP_EXTERNAL_PREFIX)

    # Right, lets download it then!
    ExternalProject_Add (
        2uvpp-proj
        GIT_REPOSITORY "git@jki.re:2uvpp.git"
        GIT_TAG ""
        INSTALL_DIR ${2UVPP_EXTERNAL_PREFIX}
        UPDATE_COMMAND ""
        CONFIGURE_COMMAND ./autogen.sh COMMAND ./configure --prefix=<INSTALL_DIR>
        BUILD_COMMAND make
        BUILD_IN_SOURCE 1
        INSTALL_COMMAND make install
    )

    # pkg_check_modules(PC_LIBUV QUIET libuv>=${LIBUV_FIND_VERSION})

    set(2UVPP_INCLUDE_DIRS ${2UVPP_EXTERNAL_PREFIX}/include)
    set(2UVPP_LIBRARY_DIRS ${2UVPP_EXTERNAL_PREFIX}/lib)
    set(2UVPP_LIBRARIES uv rt pthread nsl dl)
    set(2UVPP_LDFLAGS)
    set(2UVPP_LDFLAGS_OTHER)
    set(2UVPP_CFLAGS)
    set(2UVPP_CFLAGS_OTHER)
    set(2UVPP_DEPENDENCY libuv-proj)
endif(NOT 2UVPP_FOUND)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    LibUV DEFAULT_MSG
    LIBUV_LIBRARIES LIBUV_INCLUDE_DIRS
)


cmake_minimum_required(VERSION 3.1)

function(download_libuv path version)
    include(ExternalProject)

    get_filename_component(install_dir ${path} REALPATH)
    MESSAGE(STATUS "install_dir ${install_dir}")

    ExternalProject_Add (
        libuv-proj
        URL "https://github.com/libuv/libuv/tarball/v${version}"
        INSTALL_DIR ${install_dir}
        DOWNLOAD_NAME "libuv-${version}.tar.gz"
        UPDATE_COMMAND ""
        CONFIGURE_COMMAND ./autogen.sh COMMAND ./configure --prefix=<INSTALL_DIR>
        BUILD_COMMAND make
        BUILD_IN_SOURCE 1
        INSTALL_COMMAND make install
    )

    set(LIBUV_INCLUDE_DIR ${install_dir}/include)
    set(LIBUV_INCLUDE_DIR ${install_dir}/include PARENT_SCOPE)

    set(LIBUV_LIBRARY ${install_dir}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}uv${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(LIBUV_LIBRARY ${install_dir}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}uv${CMAKE_SHARED_LIBRARY_SUFFIX} PARENT_SCOPE)
    set(LIBUV_STATIC_LIBRARY ${LIBUV_LIBDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}uv${CMAKE_STATIC_LIBRARY_SUFFIX})
    set(LIBUV_STATIC_LIBRARY ${LIBUV_LIBDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}uv${CMAKE_STATIC_LIBRARY_SUFFIX} PARENT_SCOPE)
    set(LIBUV_DEPENDENCY libuv-proj PARENT_SCOPE)
    set(LIBUV_FOUND True PARENT_SCOPE)

    add_library(uv SHARED IMPORTED GLOBAL)
    add_dependencies(uv libuv-proj)
    set_target_properties(uv PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${LIBUV_INCLUDE_DIR}"
        IMPORTED_LOCATION "${LIBUV_LIBRARY}"
    )

    add_library(uvStatic STATIC IMPORTED GLOBAL)
    add_dependencies(uvStatic libuv-proj)

    set_target_properties(uvStatic PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${LIBUV_INCLUDE_DIR}"
        IMPORTED_LOCATION "${LIBUV_STATIC_LIBRARY}"
    )

endfunction(download_libuv)

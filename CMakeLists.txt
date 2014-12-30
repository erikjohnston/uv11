cmake_minimum_required(VERSION 3.0)

set(LIBUV_VERSION "v1.0.2")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Wno-c++98-compat -Wno-padded -Wno-exit-time-destructors -Wno-global-constructors")

include(ExternalProject)

set(libuv_install_dir ${CMAKE_CURRENT_SOURCE_DIR}/externals/libuv)

# Retrieve and build libuv of the correct version
ExternalProject_Add (
    libuv-proj
    GIT_REPOSITORY "git@github.com:libuv/libuv.git"
    GIT_TAG ${LIBUV_VERSION}
    INSTALL_DIR ${libuv_install_dir}
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ./autogen.sh COMMAND ./configure --prefix=<INSTALL_DIR>
    BUILD_COMMAND make
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND make install
)

add_library(uv STATIC IMPORTED)
add_dependencies(uv libuv-proj)
set_property(TARGET uv APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
    $<BUILD_INTERFACE:${libuv_install_dir}/include>
    $<INSTALL_INTERFACE:include/>
)
set_property(TARGET uv PROPERTY IMPORTED_LOCATION ${libuv_install_dir}/lib/libuv.a)

set(
    SOURCES
    src/buffer.cpp src/dns.cpp src/handles.cpp src/loop.cpp src/requests.cpp
    src/streams.cpp src/types.cpp
)
add_library(2uvpp ${SOURCES})
target_link_libraries(2uvpp uv pthread)

target_include_directories(2uvpp PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_executable(main src/main.cpp)
target_link_libraries(main 2uvpp uv pthread)


install(TARGETS 2uvpp EXPORT 2uvpp-export
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
)
install(DIRECTORY include/ DESTINATION include)

install(EXPORT 2uvpp-export NAMESPACE uvpp_ DESTINATION lib/2uvpp)


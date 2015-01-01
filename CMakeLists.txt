cmake_minimum_required(VERSION 3.0)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Wno-c++98-compat -Wno-padded -Wno-exit-time-destructors -Wno-global-constructors")

find_package(LibUV REQUIRED)
link_directories(${LIBUV_LIBRARY_DIRS})

set(
    SOURCES
    src/buffer.cpp src/dns.cpp src/handles.cpp src/loop.cpp src/requests.cpp
    src/streams.cpp src/types.cpp
)

add_library(2uvpp SHARED ${SOURCES})
add_library(2uvppStatic STATIC ${SOURCES})

function(setup_2uvpp_lib target)
    add_dependencies(${target} ${LIBUV_DEPENDENCY})
    target_link_libraries(${target} uv)

    target_include_directories(${target} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    )
endfunction(setup_2uvpp_lib)

setup_2uvpp_lib(2uvpp)
setup_2uvpp_lib(2uvppStatic)

add_executable(main src/main.cpp)
target_include_directories(main PRIVATE ${LIBUV_INCLUDE_DIRS})
target_link_libraries(main 2uvpp)

install(TARGETS 2uvpp 2uvppStatic EXPORT 2uvppConfig
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
)
install(DIRECTORY include/ DESTINATION include)

install(EXPORT 2uvppConfig DESTINATION lib/2uvpp)

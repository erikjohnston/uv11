cmake_minimum_required(VERSION 3.0)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Wno-c++98-compat -Wno-padded -Wno-exit-time-destructors -Wno-global-constructors")

if (NOT EXTERNALS_DIR)
    set(EXTERNALS_DIR ${CMAKE_CURRENT_LIST_DIR}/externals/)
endif(NOT EXTERNALS_DIR)

find_package(LibUV REQUIRED)

set(
    SOURCES
    src/buffer.cpp src/dns.cpp src/handles.cpp src/loop.cpp src/requests.cpp
    src/streams.cpp src/types.cpp src/tcp.cpp src/poll.cpp src/async.cpp
    src/timer.cpp
)

add_library(uv11Objects OBJECT ${SOURCES})


target_include_directories(uv11Objects PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(uv11Objects PRIVATE ${LIBUV_INCLUDE_DIR})
target_compile_options(uv11Objects PUBLIC "-std=c++11" "-fPIC")


add_library(uv11 SHARED $<TARGET_OBJECTS:uv11Objects>)
target_link_libraries(uv11 uv)
target_include_directories(uv11 INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_options(uv11 PUBLIC "-std=c++11")

add_library(uv11Static STATIC $<TARGET_OBJECTS:uv11Objects>)
target_link_libraries(uv11Static uvStatic)
target_include_directories(uv11Static INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_options(uv11Static PUBLIC "-std=c++11")

install(TARGETS uv11 uv11Static EXPORT uv11Config
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
install(DIRECTORY include/uv11 DESTINATION include)

install(EXPORT uv11Config DESTINATION lib/uv11)

add_executable(main src/main.cpp)
target_link_libraries(main uv11)

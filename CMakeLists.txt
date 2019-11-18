cmake_minimum_required(VERSION 3.9)
project(async)

set(CMAKE_CXX_STANDARD 14)

#SET(CMAKE_CXX_COMPILER clang++-6.0)

#SET(CLANG_COROUTINES_COMPILE_FLAGS "-Wall -Wextra -std=gnu++2a -fcoroutines-ts -stdlib=libc++")

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CLANG_COROUTINES_COMPILE_FLAGS}")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_library(async SHARED async.cpp async.h)

add_library(logger SHARED logger.cpp logger.h)

add_executable(async_cli main.cpp)

target_link_libraries(async_cli async)

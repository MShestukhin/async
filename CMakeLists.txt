cmake_minimum_required(VERSION 3.9)

if($ENV{TRAVIS_BUILD_NUMBER})
    project(async VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else ()
    project(async VERSION 0.0.1)
endif()

set(CMAKE_CXX_STANDARD 14)

#SET(CMAKE_CXX_COMPILER clang++-6.0)

#SET(CLANG_COROUTINES_COMPILE_FLAGS "-Wall -Wextra -std=gnu++2a -fcoroutines-ts -stdlib=libc++")

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CLANG_COROUTINES_COMPILE_FLAGS}")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_library(async SHARED async.cpp async.h)

add_library(logger SHARED logger.cpp logger.h)

add_executable(async_cli main.cpp)

target_link_libraries(async_cli async)

set_target_properties(async PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(async
    PRIVATE "${CMAKE_BINARY_DIR}"
)

install(TARGETS async_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)


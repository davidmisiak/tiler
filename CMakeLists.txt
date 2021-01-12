cmake_minimum_required(VERSION 3.12)

project(Tiler)
set(CMAKE_CXX_STANDARD 17)

if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

include(cmake/CompilerWarnings.cmake)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# load src directory, excluding main.cpp
include_directories(src)
file(GLOB_RECURSE PROJECT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
set(PROJECT_MAIN "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
list(REMOVE_ITEM PROJECT_SOURCE ${PROJECT_MAIN})

# compile "tilerlib" - entire source without main.cpp so we don't have to recompile
# it when building tests
add_library(tilerlib STATIC ${PROJECT_SOURCE})

# build main executable
add_executable(tiler ${PROJECT_MAIN})
target_link_libraries(tiler tilerlib ${CONAN_LIBS})

# build test executable
option(BUILD_TESTS "Build the testing executable (from the 'test' directory)." OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
    # see test/CMakeLists.txt
endif()

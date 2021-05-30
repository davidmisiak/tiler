cmake_minimum_required(VERSION 3.12)

project(Tiler)
set(CMAKE_CXX_STANDARD 17)

if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

include(cmake/CompilerWarnings.cmake)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
set(CONAN_SYSTEM_INCLUDES ON)  # so that compiler warnings don't apply to conan dependencies
conan_basic_setup()

# load src directory, excluding main.cpp
include_directories(src)
file(GLOB_RECURSE PROJECT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
set(PROJECT_MAIN "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
list(REMOVE_ITEM PROJECT_SOURCE ${PROJECT_MAIN})

# external libraries
set(EXTERNAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external")

option(CADICAL "Include the CaDiCaL SAT solver." ON)
if(CADICAL)
    add_compile_definitions(CADICAL)
    include_directories(SYSTEM "${EXTERNAL_DIR}/cadical/src")
    link_directories("${EXTERNAL_DIR}/cadical/build")
    set(MY_LINK_OPTIONS ${MY_LINK_OPTIONS} cadical)
else()
    list(REMOVE_ITEM PROJECT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/solvers/sat_utils/cadical_wrapper.cpp")
endif()

option(CRYPTOMINISAT "Include the CryptoMiniSat SAT solver." ON)
if(CRYPTOMINISAT)
    add_compile_definitions(CRYPTOMINISAT)
    include_directories(SYSTEM "${EXTERNAL_DIR}/cryptominisat/build/include")
    link_directories("${EXTERNAL_DIR}/cryptominisat/build/lib")
    set(MY_LINK_OPTIONS ${MY_LINK_OPTIONS} cryptominisat5)
else()
    list(REMOVE_ITEM PROJECT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/solvers/sat_utils/cryptominisat_wrapper.cpp")
endif()

# compile "tilerlib" - entire source without main.cpp so we don't have to recompile
# it when building tests
add_library(tilerlib STATIC ${PROJECT_SOURCE})
target_link_libraries(tilerlib ${MY_LINK_OPTIONS})
target_compile_options(tilerlib PRIVATE ${PROJECT_WARNINGS})

# build main executable
add_executable(tiler ${PROJECT_MAIN})
target_link_libraries(tiler tilerlib ${CONAN_LIBS})
target_compile_options(tiler PRIVATE ${PROJECT_WARNINGS})

# build test executable
option(BUILD_TESTS "Build the testing executable (from the 'test' directory)." OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
    # see test/CMakeLists.txt
endif()

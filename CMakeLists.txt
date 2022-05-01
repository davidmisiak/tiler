cmake_minimum_required(VERSION 3.12)

project(Tiler)
set(CMAKE_CXX_STANDARD 17)

# default build type is Release
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
set(SOLVERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/solvers")

option(CADICAL "Include the CaDiCaL SAT solver." ON)
if(CADICAL)
    add_compile_definitions(CADICAL)
    include_directories(SYSTEM "${EXTERNAL_DIR}/cadical/src")
    link_directories("${EXTERNAL_DIR}/cadical/build")
    set(MY_LINK_OPTIONS ${MY_LINK_OPTIONS} cadical)
else()
    list(REMOVE_ITEM PROJECT_SOURCE "${SOLVERS_DIR}/sat/cadical_wrapper.cpp")
endif()

option(CRYPTOMINISAT "Include the CryptoMiniSat SAT solver." ON)
if(CRYPTOMINISAT)
    add_compile_definitions(CRYPTOMINISAT)
    include_directories(SYSTEM "${EXTERNAL_DIR}/cryptominisat/build/include")
    link_directories("${EXTERNAL_DIR}/cryptominisat/build/lib")
    set(MY_LINK_OPTIONS ${MY_LINK_OPTIONS} cryptominisat5)
else()
    list(REMOVE_ITEM PROJECT_SOURCE "${SOLVERS_DIR}/sat/cryptominisat_wrapper.cpp")
endif()

option(BREAKID "Include the BreakID CNF symmetry breaking library." ON)
if(BREAKID)
    add_compile_definitions(BREAKID)
    include_directories(SYSTEM "${EXTERNAL_DIR}/breakid/build/include")
    link_directories("${EXTERNAL_DIR}/breakid/build/lib")
    set(MY_LINK_OPTIONS ${MY_LINK_OPTIONS} breakid)
else()
    list(REMOVE_ITEM PROJECT_SOURCE "${SOLVERS_DIR}/sat/breakid_wrapper.cpp")
endif()

if(CADICAL OR CRYPTOMINISAT)
    add_compile_definitions(PBLIB)
    include_directories(SYSTEM "${EXTERNAL_DIR}/pblib/pblib")
    link_directories("${EXTERNAL_DIR}/pblib/build")
    set(MY_LINK_OPTIONS ${MY_LINK_OPTIONS} pb)
else()
    list(REMOVE_ITEM PROJECT_SOURCE "${SOLVERS_DIR}/sat/pblib_wrapper.cpp")
endif()

option(GUROBI "Link against the Gurobi ILP solver." ON)
if(GUROBI)
    add_compile_definitions(GUROBI)
    include_directories(SYSTEM "$ENV{GUROBI_HOME}/include")
    set(MY_LINK_OPTIONS ${MY_LINK_OPTIONS} gurobi95 gurobi_g++5.2)
else()
    list(REMOVE_ITEM PROJECT_SOURCE "${SOLVERS_DIR}/ilp/gurobi_wrapper.cpp")
endif()

option(DLX "Include Knuth's DLX algorithm." ON)
if(DLX)
    add_compile_definitions(DLX)
    link_directories("${EXTERNAL_DIR}/dlx1/build")
    set(MY_LINK_OPTIONS ${MY_LINK_OPTIONS} dlx)
else()
    list(REMOVE_ITEM PROJECT_SOURCE "${SOLVERS_DIR}/dlx/dlx_wrapper.cpp")
endif()

option(MINIZINC "Include the MiniZinc and Gecode constraint solving tools." ON)
if(MINIZINC)
    add_compile_definitions(MINIZINC)
    include_directories(SYSTEM "${EXTERNAL_DIR}/libminizinc/include")
    include_directories(SYSTEM "${EXTERNAL_DIR}/libminizinc/build/include")
    link_directories("${EXTERNAL_DIR}/libminizinc/build")
    link_directories("${EXTERNAL_DIR}/gecode/build")
    set(MY_LINK_OPTIONS ${MY_LINK_OPTIONS} mzn gecodedriver gecodeflatzinc gecodefloat gecodeint gecodekernel gecodeminimodel gecodesearch gecodeset gecodesupport)
else()
    list(REMOVE_ITEM PROJECT_SOURCE "${SOLVERS_DIR}/csp/minizinc_wrapper.cpp")
endif()

option(CHUFFED "In addition to MiniZinc with Gecode, include also the Chuffed solver." ON)
if(CHUFFED)
    add_compile_definitions(CHUFFED)
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
    file(GLOB_RECURSE TEST_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")
    add_executable(tilertest ${TEST_SOURCE})
    target_link_libraries(tilertest tilerlib ${CONAN_LIBS})
endif()

# build benchmark executable
option(BUILD_BENCHMARK "Build the benchmark executable (from the 'benchmark' directory)." OFF)
if(BUILD_BENCHMARK)
    file(GLOB_RECURSE BENCHMARK_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/benchmark/*.cpp")
    add_executable(tilerbm ${BENCHMARK_SOURCE})
    target_link_libraries(tilerbm tilerlib ${CONAN_LIBS})
endif()

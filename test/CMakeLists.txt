file(GLOB_RECURSE TEST_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(tilertest ${TEST_SOURCE})
target_link_libraries(tilertest tilerlib ${CONAN_LIBS})

# add test so that we can use ctest
add_test(
    NAME tilertest
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${CMAKE_BINARY_DIR}/bin/tilertest --use-colour yes
)

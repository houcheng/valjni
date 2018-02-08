# Make test executable
file(GLOB TEST_SERVER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_executable(test-jni-server ${TEST_SERVER_SOURCES})
set(CMAKE)
include_directories(${CMAKE_SOURCE_DIR}/jni_server)
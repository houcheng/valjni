# Usage:
#   cmake -DCMAKE_BUILD_TYPE=Debug

cmake_minimum_required(VERSION 2.8.8)

set(PROJECT_NAME jni_server)
project(${PROJECT_NAME} C CXX)

# set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)
include_directories(lib)

#-----------------
# sub directories
#-----------------
add_subdirectory(lib)
add_subdirectory(bin)
add_subdirectory(test)

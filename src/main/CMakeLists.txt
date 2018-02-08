# export JAVA_HOME=/usr/lib/jvm/java-1.8.0-openjdk-amd64
# cmake $<this-directory>
cmake_minimum_required(VERSION 3.5)

#------------------
# flags
#------------------
# set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -O0 -D_DEBUG -DDEBUG -Wall")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -g -O0 -D_DEBUG -DDEBUG -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2 -s -DNDEBUG -Wall")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O2 -s -DNDEBUG -Wall")

#------------------
# JNI
#------------------
find_package(JNI)
include_directories(${JNI_INCLUDE_DIRS})

include_directories(jni/common)
#------------------
# folder
#------------------

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(jni_server)
add_subdirectory(jni)
add_subdirectory(test)

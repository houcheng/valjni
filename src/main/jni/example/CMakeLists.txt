cmake_minimum_required(VERSION 2.8)

project(example)
set(CMAKE_BUILD_TYPE Debug)

find_package(JNI)

include_directories(${JNI_INCLUDE_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES Example.c)

add_library(example SHARED ${SOURCE_FILES})

# TODO: cmake script for building jni header

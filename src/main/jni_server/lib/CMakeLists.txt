set(PROJECT_NAME jni_server)

project(${PROJECT_NAME} C CXX)

file(GLOB SOURCES *.cpp)

set(HEADERS )

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})

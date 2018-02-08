set(PROJECT_NAME jni-server)

project(${PROJECT_NAME} C CXX)

file(GLOB SOURCES *.cpp)

file(GLOB HEADERS *.hpp)

add_executable(jni-server JniServerMain.cpp ${HEADERS})
target_link_libraries(jni-server jni_server jni_common)
target_link_libraries(jni-server dl bfd rt)

add_executable(jsontest JsonTestMain.cpp ${HEADERS})
target_link_libraries(jsontest jni_server jni_common)
target_link_libraries(jsontest dl bfd rt)


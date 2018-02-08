file(GLOB TEST_CLIENT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_executable(test-client-library ${TEST_CLIENT_SOURCES} TestJniEnvWrapper.cpp TestJniArgumentRepository.cpp)
target_link_libraries(test-client-library jni_client bfd rt dl)

include_directories(${CMAKE_SOURCE_DIR}/jni/client)

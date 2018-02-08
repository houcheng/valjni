file(GLOB TEST_COMMON_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_executable(test-common-library ${TEST_COMMON_SOURCES})
target_link_libraries(test-common-library jni_common bfd dl rt)

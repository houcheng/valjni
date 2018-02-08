project(jni_client)

file(GLOB SOURCE_FILES *.cpp *.h)

add_library(jni_client SHARED ${SOURCE_FILES})
target_link_libraries(jni_client jni_common)
target_link_libraries(jni_client bfd rt dl)
include_directories(${JNI_INCLUDE_DIRS})

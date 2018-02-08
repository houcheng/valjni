set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
include_directories(catch_single_include)

add_subdirectory(src)

add_custom_target(run_test)
add_custom_command(
        TARGET run_test
        COMMENT "Run test"
        POST_BUILD
        COMMAND test-client-library)
add_custom_command(
        TARGET run_test
        COMMENT "Run test"
        POST_BUILD
        COMMAND test-common-library)
add_custom_command(
        TARGET run_test
        COMMENT "Run test"
        POST_BUILD
        COMMAND test-jni-server)

#include "JniArgumentRepository.h"

#include "catch.hpp"
#include "fakeit.hpp"
using namespace fakeit;


bool runNewGlobal = false;
bool runDelGlobal = false;
bool runNewLocal = false;
JNINativeInterface_ functions;

void clearRunFlags() {
    runNewGlobal = false;
    runDelGlobal = false;
    runNewLocal = false;
}

void setupMocks(JNIEnv* env) {
    auto* newGlobal = +[](JNIEnv* env, jobject lobj)  {
        runNewGlobal = true;
        return (jobject) 0xabcd;
    };

    auto* delGlobal = +[](JNIEnv* env, jobject lobj)  {
        runDelGlobal = true;
    };

    auto* newLocal = +[](JNIEnv* env, jobject lobj)  {
        runNewLocal = true;
        return (jobject) 0x1234;
    };

    clearRunFlags();
    functions.NewGlobalRef = newGlobal;
    functions.DeleteGlobalRef = delGlobal;
    functions.NewLocalRef = newLocal;
    env->functions = & functions;
}

TEST_CASE("Test argument repository with integer argument") {
    JniArgumentRepository repository;
    JNIEnv env;
    setupMocks(&env);

    SECTION("Test push and pop") {
        BasicJniValue jniValue(JniValueType::JNI_VALUE_INT);
        jniValue.setByInt(123);
        repository.pushArgument(&env, jniValue);
        REQUIRE_FALSE(repository.isEmpty());
        CHECK_FALSE(runNewGlobal);
        CHECK_FALSE(runDelGlobal);
        CHECK_FALSE(runNewLocal);

        BasicJniValue popJniValue = repository.popArgument(&env);
        CHECK(repository.isEmpty());
        CHECK(popJniValue.getType() == JniValueType::JNI_VALUE_INT);
        CHECK(popJniValue.getAsInt() == 123);
    }

    SECTION("Test push two items and pop") {
        BasicJniValue jniValue(JniValueType::JNI_VALUE_INT);
        jniValue.setByInt(123);
        repository.pushArgument(&env, jniValue);
        jniValue.setByInt(456);
        repository.pushArgument(&env, jniValue);

        BasicJniValue popJniValue = repository.popArgument(&env);
        REQUIRE_FALSE(repository.isEmpty());
        CHECK(popJniValue.getType() == JniValueType::JNI_VALUE_INT);
        CHECK(popJniValue.getAsInt() == 123);

        popJniValue = repository.popArgument(&env);
        CHECK(repository.isEmpty());
        CHECK(popJniValue.getType() == JniValueType::JNI_VALUE_INT);
        CHECK(popJniValue.getAsInt() == 456);
    }

}

TEST_CASE("Test argument repository with array argument") {
    JniArgumentRepository repository;
    JNIEnv env;

    setupMocks(&env);
    BasicJniValue jniValue(JniValueType::JNI_VALUE_JARRAY);
    jniValue.setByInt(0xabcd1234);
    repository.pushArgument(&env, jniValue);
    CHECK(runNewGlobal);
    CHECK_FALSE(runDelGlobal);
    CHECK_FALSE(runNewLocal);

    clearRunFlags();
    BasicJniValue popJniValue = repository.popArgument(&env);

    CHECK_FALSE(runNewGlobal);
    CHECK(runDelGlobal);
    CHECK(runNewLocal);
}


TEST_CASE("Test argument repository with null jobject argument should not create global reference") {
    JniArgumentRepository repository;
    JNIEnv env;

    setupMocks(&env);
    BasicJniValue jniValue(JniValueType::JNI_VALUE_JARRAY);
    jniValue.setByInt(0x0);
    repository.pushArgument(&env, jniValue);
    CHECK_FALSE(runNewGlobal);
    CHECK_FALSE(runDelGlobal);
    CHECK_FALSE(runNewLocal);

    clearRunFlags();
    BasicJniValue popJniValue = repository.popArgument(&env);

    CHECK_FALSE(runNewGlobal);
    CHECK_FALSE(runDelGlobal);
    CHECK_FALSE(runNewLocal);
}

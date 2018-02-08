#include "catch.hpp"
#include "fakeit.hpp"
#include "JniEnvWrapper.h"
using namespace fakeit;


jclass JNICALL f1(JNIEnv* env, const char* name) {
    return (jclass) nullptr;
}

bool runMethod = false;

void setup() {
    runMethod = false;
}

int intBuffer[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

#if 0

TEST_CASE("Test JniEnvWrapper.") {
    JNINativeInterface_ ni;
    JNIEnv_ jniEnv;
    jniEnv.functions = &ni;

    JniEnvWrapper wrapper(&jniEnv);

    setup();

    SECTION("Check wrapping call to JNIEnv's FindClass()") {
        auto call = JniEnvCall::builder()
        .setMethodName("FindClass")
        .pushArgument(make_unique<CStringJniEnvValue>("ClassName"))
        .setReturnValue(make_unique<CStringJniEnvValue>(""))
        .build();

        auto* p = +[](JNIEnv* env, const char* name)  {
            runMethod = true;
            return (jclass) nullptr;
        };
        ni.FindClass = p;
        REQUIRE_FALSE(runMethod);
        wrapper.invoke(call.get());
        REQUIRE(runMethod);
    }

    SECTION("Check wrapping call to JNIEnv's GetIntArrayElements()") {
        unique_ptr<JniValue> jarray(new BasicJniValue(JniValueType::JNI_VALUE_JARRAY));
        auto isCopy = make_unique<BasicJniValue>(JniValueType::JNI_VALUE_BOOLEAN);
        isCopy->setByString("0");

        SharedMemory sharedMemory;
        CPointerSharedMemoryJniEnvValue* returnValue = new CPointerSharedMemoryJniEnvValue(sharedMemory);

        jarray->setByJobjectId((jobject) 0x1234);

        auto call = JniEnvCall::builder()
                .setMethodName("GetIntArrayElements")
                .pushArgument(move(jarray))
                .pushArgument(isCopy)
                .setReturnValue(returnValue)
                .build();

        auto* p = +[](JNIEnv *env, jintArray array, jboolean *isCopy) {
            runMethod = true;
            return (jint *) intBuffer;
        };

        auto* pp = +[](JNIEnv *env, _jarray* array) {
            return (jsize) sizeof(intBuffer);
        };

        ni.GetIntArrayElements = p;
        ni.GetArrayLength = pp;
        REQUIRE_FALSE(runMethod);
        CPointerSharedMemoryJniEnvValue* invokeReturnValue =
                (CPointerSharedMemoryJniEnvValue*) wrapper.invoke(call);
        REQUIRE(runMethod);

        int * sharedMemoryData = (int*) invokeReturnValue->getSharedMemory().getAddress();
        REQUIRE(sharedMemoryData[0] == 1);
        REQUIRE(sharedMemoryData[1] == 2);
        REQUIRE(sharedMemoryData[15] == 16);
    }
}
#endif
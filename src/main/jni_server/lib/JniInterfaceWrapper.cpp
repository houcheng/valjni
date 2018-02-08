#include "JniInterfaceWrapper.h"

#include "../../jni/common/JniEnvInterface.h"

JniInterfaceWrapper::JniInterfaceWrapper() {
    jniEnv.functions = &jniNativeInterface;
    initializeWrapper();
}

JniInterfaceWrapper::JniInterfaceWrapper(const JniEnvInterface* client) : JniInterfaceWrapper() {
    setJniEnvInterface(client);
}

void JniInterfaceWrapper::setJniEnvInterface(const JniEnvInterface* client) {
    jniNativeInterface.reserved0 = (void*) client;
}

JNIEnv* JniInterfaceWrapper::getJNIEnv() {
    return &jniEnv;
}

namespace jni_env_wrapper {

    jclass FindClass(JNIEnv *env, const char *name) {
        JniEnvInterface* client = (JniEnvInterface*) env->functions->reserved0;
        return client->FindClass(name);
    }

    jint * GetIntArrayElements(JNIEnv *env, jintArray array, jboolean *isCopy) {
        JniEnvInterface* client = (JniEnvInterface*) env->functions->reserved0;
        return client->GetIntArrayElements(array, isCopy);
    }
    jsize GetArrayLength(JNIEnv *env, jarray array) {
        JniEnvInterface* client = (JniEnvInterface*) env->functions->reserved0;
        return client->GetArrayLength(array);
    }

}

void JniInterfaceWrapper::initializeWrapper() {
    jniNativeInterface.FindClass = jni_env_wrapper::FindClass;
    jniNativeInterface.GetIntArrayElements = jni_env_wrapper::GetIntArrayElements;
    jniNativeInterface.GetArrayLength = jni_env_wrapper::GetArrayLength;
}

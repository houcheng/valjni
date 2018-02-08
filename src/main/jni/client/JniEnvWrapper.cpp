/*
 * JniEnvClientWrapper.cpp
 *
 *  Created on: Sep 29, 2017
 *      Author: Houcheng Lin
 */
#include <jni.h>
#include "JniEnvWrapper.h"


unique_ptr<JniValue> JniEnvWrapper::invoke(JniEnvCall* call) {
    if(call->getMethodName() == "FindClass") {
        return invokeFindClass(call);
    } else if (call->getMethodName() == "GetIntArrayElements") {
        return invokeGetIntArrayElements(call);
    } else if (call->getMethodName() == "GetArrayLength") {
        return invokeGetArrayLength(call);
    }

    throw JniException("Method: " + call->getMethodName() + " is not supported yet!");
}

unique_ptr<JniValue> JniEnvWrapper::invokeFindClass(JniEnvCall *call) {
    jobject jclassValue = jniEnv->FindClass((char *) call->getArguments()[0]->getAsString().c_str());
    return valueFactory.build(jclassValue);
}

JniEnvWrapper::JniEnvWrapper(JNIEnv *jniEnv1) {
    jniEnv = jniEnv1;
}

unique_ptr<JniValue> JniEnvWrapper::invokeGetIntArrayElements(JniEnvCall *call) {
    jintArray array = (jintArray) call->getArgument(0)->getAsJobjectId();
    jboolean isCopy = call->getArgument(1)->getAsBoolean();

    jint * retrievedJavaIntArray = jniEnv->GetIntArrayElements(array, &isCopy);

    size_t sizeInByte = jniEnv->GetArrayLength(array) * sizeof(jint);
    SharedMemory sharedMemory = SharedMemory::createSharedMemory(retrievedJavaIntArray, (int) sizeInByte);
    memcpy(sharedMemory.getAddress(), retrievedJavaIntArray, sizeInByte);

    return valueFactory.build(sharedMemory);
}

unique_ptr<JniValue> JniEnvWrapper::invokeGetArrayLength(JniEnvCall *call) {
    jarray argument0 = (jarray) call->getArgument(0)->getAsJobjectId();

    jsize arrayLength = jniEnv->GetArrayLength(argument0);

    return valueFactory.build((int) arrayLength);
}

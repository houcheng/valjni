#include <jni.h>
#include "com_valjni_caller_JniClientImplementation.h"
#include "JniClient.h"
#include "JniEnvWrapper.h"
#include "JniArgumentRepository.h"


/*
 * Class:     com_valjni_caller_ClientJniCaller
 * Method:    pushJobject
 * Signature: (Lcom/valjni/caller/JniCall;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_valjni_caller_JniClientImplementation_pushJobject(JNIEnv * env,
        jobject jniClientImplementation, jobject argument) {
    BasicJniValue basicJniValue(JniValueType::JNI_VALUE_JOBJECT);
    basicJniValue.setByJobjectId(argument);
    JniArgumentRepository::getInstance().pushArgument(env, basicJniValue);
}

/*
 * Class:     com_valjni_caller_ClientJniCaller
 * Method:    pushJarray
 * Signature: (Lcom/valjni/caller/JniCall;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_valjni_caller_JniClientImplementation_pushJarray(JNIEnv * env,
        jobject jniClientImplementation, jobject argument) {
    BasicJniValue basicJniValue(JniValueType::JNI_VALUE_JARRAY);
    basicJniValue.setByJobjectId(argument);
    JniArgumentRepository::getInstance().pushArgument(env, basicJniValue);
}

/*
 * Class:     com_valjni_caller_ClientJniCaller
 * Method:    pushJarray
 * Signature: (Lcom/valjni/caller/JniCall;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_valjni_caller_JniClientImplementation_pushJint(JNIEnv * env,
        jobject jniClientImplementation, jint argument) {
    BasicJniValue basicJniValue(JniValueType::JNI_VALUE_INT);
    basicJniValue.setByInt(argument);
    JniArgumentRepository::getInstance().pushArgument(env, basicJniValue);
}

/*
 * Class:     com_valjni_caller_ClientJniCaller
 * Method:    pushJarray
 * Signature: (Lcom/valjni/caller/JniCall;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_valjni_caller_JniClientImplementation_pushJfloat(JNIEnv * env,
        jobject jniClientImplementation, jfloat argument) {
    BasicJniValue basicJniValue(JniValueType::JNI_VALUE_FLOAT);
    basicJniValue.setByFloat(argument);
    JniArgumentRepository::getInstance().pushArgument(env, basicJniValue);
}

// TODO: pushJdouble, pushJbyte, pushJshort, etc.

/*
 * Class:     com_valjni_caller_ClientJniCaller
 * Method:    invokeJniNative
 * Signature: (Lcom/valjni/caller/JniCall;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_valjni_caller_JniClientImplementation_invokeJniClientMain(JNIEnv * env,
        jobject jniClientImplementation, jstring sharedObjectPath1, jstring methodName1) {

    JniEnvWrapper jniEnvProvider(env);
    JniClient jniClient(env, jniClientImplementation, &jniEnvProvider);

    const char* methodName = env->GetStringUTFChars(methodName1, 0);
    const char* sharedObjectPath = env->GetStringUTFChars(sharedObjectPath1, 0);

    jniClient.jniClientMain(sharedObjectPath, methodName);
}

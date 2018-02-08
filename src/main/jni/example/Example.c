#include <stdlib.h>
#include "com_valjni_example_JniExample.h"

JNIEXPORT jint JNICALL Java_com_valjni_example_JniExample_addJni(JNIEnv * jniEnv, jobject jobject, jint a, jint b) {

    jint* memoryLeakResult = malloc(sizeof(jint));

    fprintf(stderr, "The value is %d and %d\n", a, b);
    *memoryLeakResult = a + b;
    return *memoryLeakResult;
}

JNIEXPORT jfloat JNICALL Java_com_valjni_example_JniExample_addFloatJni(JNIEnv * jniEnv, jobject jobject, jfloat a, jfloat b) {

    jfloat* memoryLeakResult = malloc(sizeof(jfloat));

    fprintf(stderr, "The value is %f and %f\n", a, b);

    jclass c = (*jniEnv)->FindClass(jniEnv, "java/lang/String");
    fprintf(stderr, "The string class is %p\n", c);
    *memoryLeakResult = a + b;
    return *memoryLeakResult;
}

JNIEXPORT jfloat JNICALL Java_com_valjni_example_JniExample_add4Jni(JNIEnv * jniEnv, jobject jobject, jint a, jint b,
    jfloat c, jfloat d) {

    jfloat* memoryLeakResult = malloc(sizeof(jfloat));

    fprintf(stderr, "The value is %d, %d, %f and %f\n", a, b, c, d);
    *memoryLeakResult = a + b + c + d;
    return *memoryLeakResult;
}

// TODO: add ReleaseIntArrayElements()
JNIEXPORT jint JNICALL Java_com_valjni_example_JniExample_sumJni(JNIEnv * jniEnv, jobject jobject, jintArray intData) {
    jfloat* memoryLeakResult = malloc(sizeof(jfloat));
    *memoryLeakResult = 0.f;

    fprintf(stderr, "jniEnv is:%p the intData is %p\n", jniEnv, intData);

    int intDataLen = (*jniEnv)->GetArrayLength(jniEnv, intData);

    jboolean isCopy = JNI_TRUE;
    jint* intDataValues = (*jniEnv)->GetIntArrayElements(jniEnv, intData, &isCopy);
    fprintf(stderr, "The array's address is %p\n", intDataValues);
    jint intDataSum = 0;

    for (jint i = 0; i < intDataLen; i++) {
        intDataSum += intDataValues[i];
    }
    return intDataSum;
}

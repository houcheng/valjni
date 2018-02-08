#ifndef JNI_ENV_INTERFACE
#define JNI_ENV_INTERFACE

#include <jni.h>

/**
 * Base of JniEnvProxy at JniServer side and JniEnvService at Java side.
 */
class JniEnvInterface {
public:
    virtual jclass FindClass(const char *name) = 0;
    virtual jint * GetIntArrayElements(jintArray array, jboolean *isCopy) = 0;
    virtual jsize GetArrayLength(jarray array) = 0;

    virtual ~JniEnvInterface() = default;
};

#endif

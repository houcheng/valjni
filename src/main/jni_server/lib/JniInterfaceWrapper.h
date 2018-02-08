#ifndef JNI_ENV_WRAPPER
#define JNI_ENV_WRAPPER

#include <jni.h>

#include "JniEnvInterface.h"


/**
 * Wraps JniEnvInterface implementation into the standard JNIEnv callback interface.
 */
class JniInterfaceWrapper {
public:
    JniInterfaceWrapper();
    JniInterfaceWrapper(const JniEnvInterface* client);
    void setJniEnvInterface(const JniEnvInterface* client);
    JNIEnv* getJNIEnv();

private:
    void initializeWrapper();

private:

    // jniNativeInterface's reserve0 holds pointer of wrapped JniEnvInterface object.
    JNIEnv jniEnv;
    JNINativeInterface_ jniNativeInterface;
};

#endif

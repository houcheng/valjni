#ifndef JNI_ENV_PROXY
#define JNI_ENV_PROXY

#include <jni.h>
#include "JniEnvCall.h"
#include "JniMessage.h"
#include "JniEnvInterface.h"

/**
 * Forwards requests to Java side.
 */
class JniProcessor;

class JniEnvProxy : public JniEnvInterface {
public:
    jclass FindClass(const char *name) final;
    jint* GetIntArrayElements(jintArray array, jboolean *isCopy) final;
    jsize GetArrayLength(jarray array) final;
private:
    void invoke(JniEnvCall* jni_env_call);
    JniProcessor* jniProcessor;
public:
    void setJniProccessor(JniProcessor *jniProccessor);

    void processJniEnvReply(JniEnvReplyMessage *pMessage, JniEnvCall *pCall);
};

#endif

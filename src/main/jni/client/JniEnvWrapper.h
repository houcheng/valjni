/*
 * JNIEnvClientWrapper.h
 *
 *  Created on: Sep 29, 2017
 *      Author: Houcheng Lin
 */

#ifndef SRC_MAIN_JNI_CLIENT_JNIENVCLIENTWRAPPER_H_
#define SRC_MAIN_JNI_CLIENT_JNIENVCLIENTWRAPPER_H_

#include <JniValueFactory.h>
#include "JniEnvInterface.h"
#include "JniValue.h"
#include "JniEnvCall.h"

/**
 * Wraps the standard JNIEnv functions into a single invoke(JniEnvCall*) method.
 */
class JniEnvWrapper {
public:
    JniEnvWrapper(JNIEnv *pEnv_);
    unique_ptr<JniValue> invoke(JniEnvCall* call);

private:
    JNIEnv* jniEnv;
    JniValueFactory valueFactory;

    unique_ptr<JniValue> invokeFindClass(JniEnvCall *call);
    unique_ptr<JniValue> invokeGetIntArrayElements(JniEnvCall *call);
    unique_ptr<JniValue> invokeGetArrayLength(JniEnvCall *pCall);
};

#endif /* SRC_MAIN_JNI_CLIENT_JNIENVCLIENTWRAPPER_H_ */

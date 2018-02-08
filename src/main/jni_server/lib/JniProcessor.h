/*
 * JniCaller.h
 *
 *  Created on: Sep 9, 2017
 *      Author: Houcheng
 */

#ifndef SRC_LIB_JNIPROCESSOR_H_
#define SRC_LIB_JNIPROCESSOR_H_


#include <string>
using namespace std;

#include "JniMessage.h"
#include "JniEnvProxy.h"
#include "JniInterfaceWrapper.h"

/**
 * JNI server side processor executes jni C function and handles JNI env call replies.
 */
class JniProcessor {
public:
    JniProcessor(JNIEnv* jniEnv1);
    void process(string lineRequest);
    void processJniCall(JniCallMessage* message);
    bool getIsEnd();

private:
    bool isEnd = false;
    JNIEnv* jniEnv;

    void processJniEnvReply(JniEnvReplyMessage* jniEnvReplyMessage);
};


#endif /* SRC_LIB_JNIPROCESSOR_H_ */

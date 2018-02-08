/*
 * JniClient.h
 *
 *  Created on: Sep 21, 2017
 *      Author: houcheng
 */

#ifndef LIB_JNICLIENT_H_
#define LIB_JNICLIENT_H_

#include <jni.h>
#include <string>
#include "JniEnvWrapper.h"

using std::string;

class JniMessage;
class JniReplyMessage;
class JniEnvCallMessage;
class JniCallMessage;
class JniValue;

class JniClient {
public:
    JniClient(JNIEnv *jniEnv1, jobject jniClientImplementation1, JniEnvWrapper* jniEncService1);
    void jniClientMain(string sharedObjectPath, string methodName);

private:
    void forkChildProcess(int to_server_fds[], int from_server_fds[]);
    void executeInChildProcess(int new_stdin_fd, int new_stdout_fd);

    void sendInitialJniCallMessageToServer(const string &message_json);
    void tryProcessMessageLoop(const JniCallMessage* jniCallMessage);
    void processMessageLoop(const JniCallMessage* jniCallMessage);
    void processMessage(JniMessage *message);
    void processReplyMessage(JniReplyMessage *message);
    void processJniEnvCallMessage(JniEnvCallMessage* message);

private:
    JNIEnv* jniEnv = nullptr;
    jobject jniClientImplementation = nullptr;
    JniEnvWrapper* jniEnvService = nullptr;

    bool processDone = false;

    int toServerFd = 0;
    int fromServerFd = 0;

    unique_ptr<JniCallMessage> createJniCallMessge(string sharedObjectPath, string methodName);
};

#endif /* LIB_JNICLIENT_H_ */

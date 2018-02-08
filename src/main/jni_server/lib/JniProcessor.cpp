#include <stdlib.h>
#include <iostream>

#include "json.hpp"

using namespace std;
using JSON = nlohmann::json;

#include "JniMessage.h"
#include "JniProcessor.h"
#include "JniMessageFactory.h"
#include "SharedObjectLoader.h"

JniProcessor::JniProcessor(JNIEnv* jniEnv1) {
    jniEnv = jniEnv1;
}

void JniProcessor::process(string lineRequest) {
    JSON request_json = JSON::parse(lineRequest);
    JniMessageFactory factory;

    cerr << "[Server] Received: " + lineRequest << endl;
    auto jni_message = factory.createMessageFromJson(request_json);

    cerr << "[Server] creates message done. " << endl;

    if (JniCallMessage* jniCallMessage = dynamic_cast<JniCallMessage*>(jni_message.get())) {
        cerr << "[Server] process jni call." << endl;
        processJniCall(jniCallMessage);
    } else if(JniEnvReplyMessage* jniEnvReplyMessage = dynamic_cast<JniEnvReplyMessage*>(jni_message.get())) {
        cerr << "[Server] process jni env reply." << endl;
        processJniEnvReply(jniEnvReplyMessage);
    }
}

void JniProcessor::processJniCall(JniCallMessage* message) {
    Loader loader(jniEnv);
    auto returnValue = loader.callFunction(message, message->getJniCall()->getArguments());
    std::cerr << "[Server] The return value is:" << returnValue->getAsString() << endl;

    JniReplyMessage replyMessage(returnValue.get());
    cout << replyMessage.toJsonString() << endl;
}

bool JniProcessor::getIsEnd() {
    return isEnd;
}

void JniProcessor::processJniEnvReply(JniEnvReplyMessage *jniEnvReplyMessage) {
    cerr << "[Server] process jni env reply" << endl;
}

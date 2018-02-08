#include <iostream>
#include <string>

#include "JniValue.h"
#include "JniMessage.h"
#include "JniMessageFactory.h"

JniMessage::~JniMessage() {
}

JniCallMessage::JniCallMessage() {
    jniCall = unique_ptr<JniCall>(nullptr);
}

JniCallMessage::JniCallMessage(JniCall* jni_call) {
    jniCall = unique_ptr<JniCall>(jni_call->clone());
}

JniCall* JniCallMessage::getJniCall() const {
    return jniCall.get();
}

const string JniCallMessage::toJsonString() const {
    JSON jsonArray = JSON::array({
        "com.valjni.caller.JniCallMessage",
        jniCall->toJson(),
    });
    return jsonArray.dump();
}

JniReplyMessage::JniReplyMessage() {
    returnValue = unique_ptr<JniValue>(nullptr);
}

JniReplyMessage::JniReplyMessage(JniValue* returnValue0) {
    returnValue = unique_ptr<JniValue>(returnValue0->clone());
}

const string JniReplyMessage::toJsonString() const {
    JSON returnValueJson =  {
        { "returnValue", returnValue->getJson()}
    };
    JSON jsonArray = JSON::array({
        "com.valjni.caller.JniReplyMessage",
        returnValueJson,
    });
    return jsonArray.dump();

}

JniValue *JniReplyMessage::getReturnValue() const {
    return returnValue.get();
}

/**
 * JniEnvCall message
 */
JniEnvCallMessage::JniEnvCallMessage(JniEnvCall* jniEnvCall) {
    jniCall = unique_ptr<JniEnvCall>((JniEnvCall *)(jniEnvCall->clone()));
}

JniEnvCall *JniEnvCallMessage::getJniEnvCall() {
    return (JniEnvCall*) getJniCall();
}

const string JniEnvCallMessage::toJsonString() const {
    JSON jsonArray = JSON::array();
    jsonArray[0] = "com.valjni.caller.JniEnvCallMessage";
    jsonArray[1] = getJniCall()->toJson();

    return jsonArray.dump();
}

JniEnvReplyMessage::JniEnvReplyMessage(JniValue* returnValue) : JniReplyMessage(returnValue) {

}

const string JniEnvReplyMessage::toJsonString() const {
    JSON returnValueJson =  {
            { "returnValue", getReturnValue()->getJson()}
    };
    JSON jsonArray = JSON::array({
            "com.valjni.caller.JniEnvReplyMessage",
            returnValueJson,
    });

    return jsonArray.dump();
}

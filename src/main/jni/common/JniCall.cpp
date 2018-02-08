#include "JniValue.h"
#include "JniCall.h"

JniCall::JniCall() : CFunctionCall() {
}

JniCall::JniCall(const JSON& json) : CFunctionCall(json) {
    methodName = json["methodName"];
    sharedObjectPath = json["sharedObjectPath"];
}

const string &JniCall::getMethodName() const {
    return methodName;
}

const string &JniCall::getSharedObjectPath() const {
    return sharedObjectPath;
}

const string JniCall::toJsonString() const {
    string jsonString = toJson().dump();
    return jsonString;
}

JSON JniCall::toJson() const {
    JSON argumentsJson;
    int index = 0;
    for(auto&& argument: arguments) {
        argumentsJson[index++] = argument->getJson();
    }

    JSON returnValueJson;

    if (returnValue == nullptr) {
        returnValueJson = "null";
    } else {
        returnValueJson = getReturnValue()->getJson();
    }

    JSON json = {
            { "methodName", methodName },
            { "sharedObjectPath", sharedObjectPath },
            { "argumentValues", argumentsJson },
            { "returnValue", returnValueJson },
    };
    return json;
}

void JniCall::setMethodName(const string &methodName) {
    JniCall::methodName = methodName;
}

void JniCall::setSharedObjectPath(const string &sharedObjectPath) {
    JniCall::sharedObjectPath = sharedObjectPath;
}

JniCall *JniCall::clone() {
    JniCall* newJniCall = new JniCall();
    newJniCall->setMethodName(methodName);
    newJniCall->setSharedObjectPath(sharedObjectPath);
    if (returnValue) {
        newJniCall->setReturnValue(returnValue.get());
    }

    // TODO: Study why "auto &&" can iterate the STL unique_ptr container.
    for(auto&& argument: arguments) {
        newJniCall->pushArgument(argument.get());
    }
    return newJniCall;
}

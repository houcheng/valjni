#include "JniEnvCall.h"
#include "JniValue.h"

JniEnvCall::JniEnvCall(const JSON &json) : JniCall(json) {
}

JniEnvCall::Builder JniEnvCall::builder() {
    return JniEnvCall::Builder();
}

JniEnvCall::JniEnvCall() : JniCall() {

}

JniEnvCall::Builder& JniEnvCall::Builder::setMethodName(const char *method_name) {
    methodName = method_name;
    return *this;
}

JniEnvCall::Builder& JniEnvCall::Builder::setReturnValue(JniValue* return_value) {
    JniValue* newValue = return_value->clone();
    returnValue = unique_ptr<JniValue>(newValue);
    return *this;
}

JniEnvCall::Builder& JniEnvCall::Builder::pushArgument(JniValue* argument) {
    JniValue* newValue = argument->clone();
    arguments.push_back(unique_ptr<JniValue>(newValue));
    return *this;
}

unique_ptr<JniEnvCall> JniEnvCall::Builder::build() {
    auto jniEnvCall = make_unique<JniEnvCall>();
    jniEnvCall->setMethodName(methodName);
    for (auto&& argument: arguments) {
        jniEnvCall->pushArgument(argument.get());
    }
    jniEnvCall->setReturnValue(returnValue.get());

    return jniEnvCall;
}

#include <iostream>

#include "CFunctionCall.h"
#include "JniValue.h"
#include "JniValueFactory.h"

JniValue* CFunctionCall::getReturnValue() const {
    return returnValue.get();
}

vector<unique_ptr<JniValue>>& CFunctionCall::getArguments() {
    return arguments;
}

void CFunctionCall::setReturnValue(JniValue* returnValue1) {
    returnValue = unique_ptr<JniValue>(returnValue1->clone());
}

void CFunctionCall::pushArgument(JniValue* newArgument) {
    arguments.push_back(unique_ptr<JniValue>(newArgument->clone()));
}

CFunctionCall::CFunctionCall(const JSON& json) {
    JniValueFactory factory;
    for(JSON argumentJson : json["argumentValues"]) {
        pushArgument(factory.buildFromJson(argumentJson).get());
    }
    setReturnValue(factory.buildFromJson(json["returnValue"]).get());
}

CFunctionCall::CFunctionCall() {
    returnValue = unique_ptr<JniValue>(nullptr);
}

CFunctionCall::~CFunctionCall() {
}

const BasicJniValue *CFunctionCall::getArgument(int i) const {
    return (BasicJniValue*) arguments[i].get();
}

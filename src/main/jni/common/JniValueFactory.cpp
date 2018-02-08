#include <iostream>
#include "JniRawValue.h"
#include "JniValue.h"
#include "JniValueFactory.h"

unique_ptr<JniValue> JniValueFactory::buildFromJson(const JSON &json) {
    JniValue* jniValue = nullptr;
    string signature = json["signature"];

    if (BasicJniValue::getSupportedClasses().isSignatureMatch(signature)) {
        jniValue = new BasicJniValue(signature);
        jniValue->setByString(json["value"]);

    } else if(CStringJniEnvValue::getSupportedClasses().isSignatureMatch(signature)) {
        string valueString = json["value"];
        jniValue = new CStringJniEnvValue(valueString);
    } else if(CPointerSharedMemoryJniEnvValue::getSupportedClasses().isSignatureMatch(signature)) {
        SharedMemory sharedMemory(json["sharedMemory"]);
        jniValue = new CPointerSharedMemoryJniEnvValue(sharedMemory);
    }

    if(jniValue != nullptr) {
        return unique_ptr<JniValue>(jniValue);
    }

    throw JniException("Creating JniValue for the JSON string is supported: " + json.dump());
}

unique_ptr<JniValue> JniValueFactory::build(const string& signature, JniRawValue jniRawValue) {
    JniValue* jniValue = nullptr;

    if (BasicJniValue::getSupportedClasses().isSignatureMatch(signature)) {
        jniValue = new BasicJniValue(signature);
    } else if(CPointerSharedMemoryJniEnvValue::getSupportedClasses().isSignatureMatch(signature)) {
        throw JniException("Creating CPointerSharedMemoryJniEnvValue with raw value is not supported.");
    } else if(CStringJniEnvValue::getSupportedClasses().isSignatureMatch(signature)) {
        throw JniException("Creating CStringJniEnvValue with raw value is not supported.");
    }

    if (jniValue != nullptr) {
        jniValue->setByRaw(jniRawValue);
        return unique_ptr<JniValue>(jniValue);
    }

    throw JniException("Creating JniValue from (" + signature + ", " + std::to_string(jniRawValue) + ") is not supported.");
}

unique_ptr<JniValue> JniValueFactory::build(int value) {
    auto returnValue = new BasicJniValue(JniValueType::JNI_VALUE_JSIZE);
    returnValue->setByInt(value);
    return unique_ptr<JniValue>(returnValue);
}

unique_ptr<JniValue> JniValueFactory::build(const SharedMemory &value) {
    auto returnValue = new CPointerSharedMemoryJniEnvValue(value);
    return unique_ptr<JniValue>(returnValue);
}

unique_ptr<JniValue> JniValueFactory::build(jobject value) {
    auto returnValue = new BasicJniValue(JniValueType::JNI_VALUE_JOBJECT);
    returnValue->setByJobjectId(value);
    return unique_ptr<JniValue>(returnValue);
}

unique_ptr<JniValue> JniValueFactory::build(jfloat value) {
    auto returnValue = new BasicJniValue(JniValueType::JNI_VALUE_FLOAT);
    returnValue->setByFloat(value);
    return unique_ptr<JniValue>(returnValue);
}

unique_ptr<JniValue> JniValueFactory::build(jdouble value) {
    auto returnValue = new BasicJniValue(JniValueType::JNI_VALUE_DOUBLE);
    returnValue->setByDouble(value);
    return unique_ptr<JniValue>(returnValue);
}

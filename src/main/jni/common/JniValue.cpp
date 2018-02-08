#include <iostream>
#include <memory>
#include <string.h>
#include <boost/lexical_cast.hpp>

#include "jni.h"

#include "boost/format.hpp"
#include "JniRawValue.h"
#include "JniValue.h"

const JSON JniValue::getJson() const {
    JSON json = {
            { "signature", getSignature() },
            { "value", getAsString() },
    };
    return json;
};

const JniRawValue JniValue::getAsRaw() const {
    return raw_value_;
}

void JniValue::setByRaw(const JniRawValue raw_value) {
    raw_value_ = raw_value;
}


SupportedClasses::SupportedClasses(const unordered_map<string, JniValueType>& signatureToType1) {
    signatureToType = signatureToType1;
}

bool SupportedClasses::isSignatureMatch(const string signature) const {
    auto it = signatureToType.find(signature);
    return it != signatureToType.end();
}

JniValueType SupportedClasses::getTypeFromSignature(const string signature) const {
    return signatureToType.at(signature);
}

string SupportedClasses::getSignatureFromType(const JniValueType type) const {
    for(auto item : signatureToType) {
        JniValueType itemType = item.second;
        if (itemType == type) {
            return item.first;
        }
    }
    return nullptr;
}

/*
 * Primitive Jni Value
 */
unordered_map<string, JniValueType> primitiveSignatureToType = {
        { "B", JniValueType::JNI_VALUE_BYTE },
        { "C", JniValueType::JNI_VALUE_CHAR },

        { "S", JniValueType::JNI_VALUE_SHORT },
        { "I", JniValueType::JNI_VALUE_INT },
        { "J", JniValueType::JNI_VALUE_LONG },

        { "F", JniValueType::JNI_VALUE_FLOAT },
        { "D", JniValueType::JNI_VALUE_DOUBLE },

        { "VOID", JniValueType::JNI_VALUE_VOID },
        { "Z", JniValueType::JNI_VALUE_BOOLEAN },
        { "JSIZE", JniValueType::JNI_VALUE_JSIZE },
        { "JOBJECT", JniValueType::JNI_VALUE_JOBJECT },
        { "JARRAY", JniValueType::JNI_VALUE_JARRAY },
};

SupportedClasses primitiveSupportedClasses(primitiveSignatureToType);

const SupportedClasses& BasicJniValue::getSupportedClasses() {
    return primitiveSupportedClasses;
}

const string BasicJniValue::getAsString() const {
    switch(jniValueType) {
    case JniValueType::JNI_VALUE_BYTE:
        return ConvertToString<jbyte>(raw_value_);
    case JniValueType::JNI_VALUE_CHAR:
        return ConvertToString<jchar>(raw_value_);

    case JniValueType::JNI_VALUE_SHORT:
        return ConvertToString<jshort>(raw_value_);
    case JniValueType::JNI_VALUE_INT:
        return ConvertToString<jint>(raw_value_);
    case JniValueType::JNI_VALUE_LONG:
        return ConvertToString<jlong>(raw_value_);

    case JniValueType::JNI_VALUE_FLOAT:
        return ConvertToString<jfloat>(raw_value_);
    case JniValueType::JNI_VALUE_DOUBLE:
        return ConvertToString<jdouble>(raw_value_);

    case JniValueType::JNI_VALUE_VOID:
        return "";
    case JniValueType::JNI_VALUE_BOOLEAN:
        return ConvertToString<jboolean>(raw_value_);
    case JniValueType::JNI_VALUE_JSIZE:
        return ConvertToString<jsize>(raw_value_);
    case JniValueType::JNI_VALUE_JARRAY:
    case JniValueType::JNI_VALUE_JOBJECT:
        return ConvertPointerToString(raw_value_);
    default:
        throw JniException("BasicJniValue::getAsString() unsupported type!");
    }
}

const bool BasicJniValue::isRealNumber() const {
    return (jniValueType == JniValueType::JNI_VALUE_FLOAT) || (jniValueType == JniValueType::JNI_VALUE_DOUBLE);
}

void BasicJniValue::setByString(const string value_string) {
    if (value_string == "null") {
        raw_value_ = 0;
        return;
    }

    switch(jniValueType) {
        case JniValueType::JNI_VALUE_BYTE:
            raw_value_ = ConvertToJniRawValue<jbyte>(value_string);
            return;
        case JniValueType::JNI_VALUE_CHAR:
            raw_value_ = ConvertToJniRawValue<jchar>(value_string);
            return;
        case JniValueType::JNI_VALUE_SHORT:
            raw_value_ = ConvertToJniRawValue<jshort>(value_string);
            return;
        case JniValueType::JNI_VALUE_INT:
            raw_value_ = ConvertToJniRawValue<jint>(value_string);
            return;
        case JniValueType::JNI_VALUE_LONG:
            raw_value_ = ConvertToJniRawValue<jlong>(value_string);
            return;
        case JniValueType::JNI_VALUE_FLOAT:
            raw_value_ = ConvertToJniRawValue<jfloat>(value_string);
            return;
        case JniValueType::JNI_VALUE_DOUBLE:
            raw_value_ = ConvertToJniRawValue<jdouble>(value_string);
            return;
        case JniValueType::JNI_VALUE_VOID:
            return;
        case JniValueType::JNI_VALUE_BOOLEAN:
            raw_value_ = ConvertToJniRawValue<jboolean>(value_string);
            return;
        case JniValueType::JNI_VALUE_JSIZE:
            raw_value_ = ConvertToJniRawValue<jsize>(value_string);
            return;
        case JniValueType::JNI_VALUE_JOBJECT:
            raw_value_ = ConvertToJniRawValueAsNativePointer(value_string);
            return;
        case JniValueType::JNI_VALUE_JARRAY:
            raw_value_ = ConvertToJniRawValueAsNativePointer(value_string);
            return;
        default:
            throw JniException("BasicJniValue::setByString unsupported type!");
    }
}

const string BasicJniValue::getSignature() const {
    return getSupportedClasses().getSignatureFromType(jniValueType);
}

const jfloat BasicJniValue::getAsFloat() const {
    return ConvertToNative<jfloat>(raw_value_);
}

const jdouble BasicJniValue::getAsDouble() const {
    return ConvertToNative<jdouble>(raw_value_);
}

const jobject BasicJniValue::getAsJobjectId() const {
    return (jobject) ConvertToNativePointer(raw_value_);
}

BasicJniValue::BasicJniValue(JniValueType jniValueType1) {
    jniValueType = jniValueType1;
}

BasicJniValue::BasicJniValue(string signature) {
    jniValueType = getSupportedClasses().getTypeFromSignature(signature);
}

void BasicJniValue::setByJobjectId(jobject jid) {
        raw_value_ = (JniRawValue) jid;
}

const jboolean BasicJniValue::getAsBoolean() const {
    return ConvertToNative<jboolean>(raw_value_);
}

const jint BasicJniValue::getAsInt() const {
    return ConvertToNative<jint>(raw_value_);
}

const bool BasicJniValue::isJobject() const {
    return (jniValueType == JniValueType::JNI_VALUE_JARRAY) ||
            (jniValueType == JniValueType::JNI_VALUE_JOBJECT);
}

void BasicJniValue::setByInt(jint value) {
    raw_value_ = ConvertNativeToJniRawValue<jint>(value);
}

void BasicJniValue::setByFloat(jfloat value) {
    raw_value_ = ConvertNativeToJniRawValue<jfloat>(value);
}

void BasicJniValue::setByDouble(jdouble value) {
    raw_value_ = ConvertNativeToJniRawValue<jdouble>(value);
}

JniValue *BasicJniValue::clone() {
    JniValue * newValue = new BasicJniValue(jniValueType);
    newValue->setByRaw(raw_value_);
    return newValue;
}


/**
 * CStringJniEnvValue implementation
 */
unordered_map<string, JniValueType> cStringSignatureToType = {
        { "CSTRING", JniValueType::JNIENV_VALUE_STRING},
};

SupportedClasses cStringSupportedClasses(cStringSignatureToType);

const SupportedClasses& CStringJniEnvValue::getSupportedClasses() {
    return cStringSupportedClasses;
}

CStringJniEnvValue::CStringJniEnvValue(string value1) {
    stringValue = value1;
}

const string CStringJniEnvValue::getAsString() const {
    return stringValue;
}
const string CStringJniEnvValue::getSignature() const {
    return getSupportedClasses().getSignatureFromType(JniValueType::JNIENV_VALUE_STRING);
}

void CStringJniEnvValue::setByString(const string value1) {
    stringValue = value1;
}

JniValue *CStringJniEnvValue::clone() {
    JniValue * newValue = new CStringJniEnvValue(stringValue);
    return newValue;
}

/**
 * CPointerValue implementation.
 */
unordered_map<string, JniValueType> cPointerSignatureToType = {
        { "*", JniValueType::JNIENV_VALUE_C_POINTER },
};

SupportedClasses cPointerSupportedClasses(cPointerSignatureToType);

const SupportedClasses& CPointerSharedMemoryJniEnvValue::getSupportedClasses() {
    return cPointerSupportedClasses;
}

const string CPointerSharedMemoryJniEnvValue::getSignature() const {
    return getSupportedClasses().getSignatureFromType(jniValueType);
}

const JSON CPointerSharedMemoryJniEnvValue::getJson() const {
    JSON json = {
            { "signature", "*"},
            { "sharedMemory", sharedMemory.getJson() },
    };
    return json;
}

void CPointerSharedMemoryJniEnvValue::setByString(const string value) {
    throw JniException("CPointerSharedMemoryJniEnvValue::setByString is not supported.");
}

const string CPointerSharedMemoryJniEnvValue::getAsString() const {
    return getJson().dump();
}

const SharedMemory &CPointerSharedMemoryJniEnvValue::getSharedMemory() const {
    return sharedMemory;
}

CPointerSharedMemoryJniEnvValue::CPointerSharedMemoryJniEnvValue(const SharedMemory &sharedMemory1) {
    sharedMemory = sharedMemory1;
    jniValueType = JniValueType::JNIENV_VALUE_C_POINTER;
}

JniValue *CPointerSharedMemoryJniEnvValue::clone() {
    JniValue * newValue = new CPointerSharedMemoryJniEnvValue(sharedMemory);
    return newValue;
}

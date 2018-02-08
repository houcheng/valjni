/*
 * JniArgument.h
 *
 *  Created on: Sep 9, 2017
 *      Author: houcheng
 */

#ifndef SRC_LIB_JNIVALUE_H_
#define SRC_LIB_JNIVALUE_H_

#include <string>
#include <memory>
#include <unordered_map>
#include <jni.h>


#include "JniRawValue.h"
#include "json.hpp"
#include "SharedMemory.h"

using namespace std;
using JSON = nlohmann::json;

enum class JniValueType {
    // BasicJniValue
    JNI_VALUE_BYTE,
    JNI_VALUE_CHAR,

    JNI_VALUE_SHORT,
    JNI_VALUE_INT,
    JNI_VALUE_LONG,

    JNI_VALUE_FLOAT,
    JNI_VALUE_DOUBLE,

    JNI_VALUE_VOID,
    JNI_VALUE_BOOLEAN,
    JNI_VALUE_JSIZE,
    JNI_VALUE_JOBJECT,  // object id only
    JNI_VALUE_JARRAY,   // object id only

    // CStringJniEnvValue
    JNIENV_VALUE_STRING,    // Carries char * string

    // CPointerSharedMemoryJniEnvValue
    JNIENV_VALUE_C_POINTER, // Carries shared memory information.
};

class SupportedClasses {
public:
    SupportedClasses(const unordered_map<string, JniValueType>& signatureToType1);

    bool isSignatureMatch(string signature) const;
    JniValueType getTypeFromSignature(string signature) const;
    string getSignatureFromType(JniValueType type) const;
private:
    unordered_map<string, JniValueType> signatureToType;
};


/*
 * Carries getArguments value for JniCallMessage and return pointer value for
 * JniReplyMessage.
 */
class JniValue {
public:
    virtual const string getSignature() const = 0;
    virtual const string getAsString() const = 0;
    virtual void setByString(string value1) = 0;

    virtual const JSON getJson() const;

    virtual JniValue* clone() = 0;
    virtual ~JniValue() = default;

    const JniRawValue getAsRaw() const;
    void setByRaw(JniRawValue jniRawValue1);

protected:
    JniValueType jniValueType;
    JniRawValue raw_value_;

public:
    JniValueType getType() { return jniValueType; };

};


// TODO: Check the type information of JARRAY/ JOBJECT is adequate for every JniEnv's methods
// TODO: on both client and server side.
/**
 * There are 3 kinds of basic types used in JNI calls. including primitive, j* array and jobject types.
 */
class BasicJniValue : public JniValue {
public:
    static const SupportedClasses& getSupportedClasses();

    BasicJniValue(JniValueType jniValueType1);
    BasicJniValue(string signature);
    JniValue* clone() final;
    ~BasicJniValue() = default;

    const string getSignature() const final;

    const string getAsString() const final;
    void setByString(string value) final;

    const jfloat getAsFloat() const;
    void setByFloat(jfloat d);

    const jdouble getAsDouble() const;
    void setByDouble(jdouble d);

    const jboolean getAsBoolean() const;
    void setByBoolean(jboolean value);

    const jint getAsInt() const;
    void setByInt(jint d);

    const jobject getAsJobjectId() const;
    void setByJobjectId(jobject jid);

    const bool isRealNumber() const;
    const bool isJobject() const;

    // TODO: getter/ setter for short, byte, etc.
};

/**
 * Carries JniEnv call's C string parameter or result between processes.
 */
class CStringJniEnvValue: public JniValue {
public:
    static const SupportedClasses& getSupportedClasses();

    CStringJniEnvValue(string value1);
    JniValue* clone() final;

    const string getSignature() const final;

    const string getAsString() const final;
    void setByString(string value) final;

private:
    string stringValue;
};

/**
 * Carries JniENv call's memory parameter or result between processes.
 */
class CPointerSharedMemoryJniEnvValue : public JniValue {
public:
    static const SupportedClasses& getSupportedClasses();

    CPointerSharedMemoryJniEnvValue(const SharedMemory& sharedMemory1);
    JniValue* clone() final;

    const string getSignature() const final;
    const JSON getJson() const final;

private:
    SharedMemory sharedMemory;
public:
    const SharedMemory& getSharedMemory() const;

private: // unsupported: value(4)
    const string getAsString() const final;
    void setByString(string value) final;
};
#endif /* SRC_LIB_JNIVALUE_H_ */

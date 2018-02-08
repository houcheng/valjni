#ifndef SRC_LIB_JNIMESSAGE_H_
#define SRC_LIB_JNIMESSAGE_H_

#include "JniCall.h"
#include "JniEnvCall.h"

#define MAX_JNI_ARGUMENTS 8

class JniMessage {
public:
    virtual const string toJsonString() const = 0;
    virtual ~JniMessage();
};

class JniCallMessage : public JniMessage {
public:
    JniCallMessage();
    JniCallMessage(JniCall* jni_call);
    const string toJsonString() const override;

protected:
    unique_ptr<JniCall> jniCall = unique_ptr<JniCall>(nullptr);
public:
    JniCall* getJniCall() const;
};


class JniReplyMessage : public JniMessage {
public:
    JniReplyMessage();
    JniReplyMessage(JniValue* returnValue);
    const string toJsonString() const override;

private:
    unique_ptr<JniValue> returnValue;
public:
    JniValue* getReturnValue() const;
};

class JniEnvCallMessage: public JniCallMessage {
public:
    JniEnvCallMessage(JniEnvCall* jniEnvCall);
    const string toJsonString() const final;

public:
    JniEnvCall* getJniEnvCall();
};

class JniEnvReplyMessage: public JniReplyMessage {
public:
    JniEnvReplyMessage(JniValue* returnValue);
    const string toJsonString() const override;
};

#endif /* SRC_LIB_JNIMESSAGE_H_ */

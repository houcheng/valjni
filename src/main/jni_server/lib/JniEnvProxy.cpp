#include "JniEnvProxy.h"
#include "JniMessageFactory.h"
#include "JniProcessor.h"
#include "JniValue.h"
#include <iostream>

void JniEnvProxy::invoke(JniEnvCall *jniEnvCall) {
    JniEnvCallMessage message(jniEnvCall);
    cerr << "[Server] invoke: " << jniEnvCall->getMethodName() << endl;
    cout << message.toJsonString() << endl;

    string lineRequest;
    getline(std::cin, lineRequest);
    cerr << "[Server] Received: " << lineRequest << endl;

    JSON request_json = JSON::parse(lineRequest);
    JniMessageFactory factory;
    cerr << "[Server] generate: " << endl;
    auto jni_message = factory.createMessageFromJson(request_json);
    cerr << "[Server] generate: done " << endl;

    if (JniEnvReplyMessage *jniEnvReplyMessage = dynamic_cast<JniEnvReplyMessage *>(jni_message.get())) {
        processJniEnvReply(jniEnvReplyMessage, jniEnvCall);
    } else {
        throw JniException("Expecting JniEnvReply message in jni-server.");
    }
}

void JniEnvProxy::setJniProccessor(JniProcessor *jniProccessor) {
    JniEnvProxy::jniProcessor = jniProccessor;
}

void JniEnvProxy::processJniEnvReply(JniEnvReplyMessage *jniEnvReplyMessage, JniEnvCall *jniEnvCall) {
    cerr << "process JniEnvReply" << endl;

    // TODO: fix bad here,the original returnValue is lost!
    jniEnvCall->setReturnValue(jniEnvReplyMessage->getReturnValue());
}

jclass JniEnvProxy::FindClass(const char *name) {
    CStringJniEnvValue className(name);
    CStringJniEnvValue initReturnValue("");
    auto jniEnvCall = JniEnvCall::builder()
            .setMethodName("FindClass")
            .pushArgument(&className)
            .setReturnValue(&initReturnValue)
            .build();

    invoke(jniEnvCall.get());
    jclass returnValue = (jclass) ((BasicJniValue *) jniEnvCall->getReturnValue())->getAsJobjectId();
    cerr << "find class done." << endl;
    fprintf(stderr, "The jclss is %p\n", returnValue);
    return returnValue;
}

jint *JniEnvProxy::GetIntArrayElements(jintArray array, jboolean *isCopy) {
    BasicJniValue value(JniValueType::JNI_VALUE_JARRAY);
    value.setByJobjectId(array);
    SharedMemory nullSharedMemory;
    CPointerSharedMemoryJniEnvValue sharedMemoryValue(nullSharedMemory);

    auto jniEnvCall = JniEnvCall::builder()
            .setMethodName("GetIntArrayElements")
            .pushArgument(&value)
            .setReturnValue(&sharedMemoryValue)
            .build();

    invoke(jniEnvCall.get());
    SharedMemory remoteSharedMemory =
            ((CPointerSharedMemoryJniEnvValue *) jniEnvCall->getReturnValue())->getSharedMemory();

    SharedMemory localSharedMemory = SharedMemory::loadSharedMemory(
            remoteSharedMemory.getShmId(), remoteSharedMemory.getAddress(), remoteSharedMemory.getSize());
    return (jint *) localSharedMemory.getAddress();
}

// TODO: fixed the fake GetArrayLength()
jsize JniEnvProxy::GetArrayLength(jarray array) {
    BasicJniValue value(JniValueType::JNI_VALUE_JARRAY);
    value.setByJobjectId(array);

    BasicJniValue returnValue(JniValueType::JNI_VALUE_INT);

    auto jniEnvCall = JniEnvCall::builder()
            .setMethodName("GetArrayLength")
            .pushArgument(&value)
            .setReturnValue(&returnValue)
            .build();

    invoke(jniEnvCall.get());
    return ((BasicJniValue*) jniEnvCall->getReturnValue())->getAsInt();
 }


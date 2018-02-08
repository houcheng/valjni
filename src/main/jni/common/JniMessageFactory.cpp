#include "JniMessageFactory.h"
#include "JniValueFactory.h"


unique_ptr<JniMessage> JniMessageFactory::createMessageFromJson(const JSON& json) {
    std::cerr << "[JniMessageFactory] Parse message:" << endl;

    string class_path_name = json[0];
    string class_name = class_path_name.substr(class_path_name.rfind(".") + 1);
    std::cerr << "[JniMessageFactory] Check class type: " << class_path_name << endl;

    if (class_name == "JniCallMessage") {
        JniCall jniCall(json[1]);
        return make_unique<JniCallMessage>(&jniCall);
    } else if (class_name == "JniReplyMessage") {
        JniValueFactory jniValueFactory;
        auto jniValue = jniValueFactory.buildFromJson(json[1]["returnValue"]);
        return make_unique<JniReplyMessage>(jniValue.get());
    } else if (class_name == "JniEnvCallMessage") {
        std::cerr << "Json is:" << json[1].dump() << endl;
        auto jniEnvCall = make_unique<JniEnvCall>(json[1]);
        return std::make_unique<JniEnvCallMessage>(jniEnvCall.get());
    } else if (class_name == "JniEnvReplyMessage") {
        JniValueFactory jniValueFactory;
        auto jniValue = jniValueFactory.buildFromJson(json[1]["returnValue"]);
        return std::make_unique<JniEnvReplyMessage>(jniValue.get());
    }
    throw JniException("JniMessageFactory fails to create message from json:" + json.dump());
}


#include "JniArgumentRepository.h"

JniArgumentRepository JniArgumentRepository::argumentRepository;

JniArgumentRepository& JniArgumentRepository::getInstance() {
    return argumentRepository;
}

void JniArgumentRepository::pushArgument(JNIEnv * env, BasicJniValue& basicJniValue) {
    if (basicJniValue.isJobject() && basicJniValue.getAsJobjectId() != nullptr) {
        basicJniValue.setByJobjectId(
                env->NewGlobalRef(basicJniValue.getAsJobjectId()));
    }
    globalRefArguments.push_back(basicJniValue);
}


// TODO: test jni get and set by jobject-id.
// Note: the return must pass by value, as it only have local variable reference.
const BasicJniValue JniArgumentRepository::popArgument(JNIEnv * env) {
    BasicJniValue basicJniValue = globalRefArguments.front();
    globalRefArguments.pop_front();

    if (basicJniValue.isJobject() && basicJniValue.getAsJobjectId() != nullptr) {
        jobject newLocal = env->NewLocalRef(basicJniValue.getAsJobjectId());
        env->DeleteGlobalRef(basicJniValue.getAsJobjectId());
        basicJniValue.setByJobjectId(newLocal);
    }
    return basicJniValue;
}

const BasicJniValue JniArgumentRepository::popReturnResult() {
    BasicJniValue basicJniValue = globalRefArguments.back();
    globalRefArguments.pop_back();
    return basicJniValue;
}
void JniArgumentRepository::clearArguments(JNIEnv * env) {
    for(auto basicJniValue: globalRefArguments) {
        if (basicJniValue.isJobject() && basicJniValue.getAsJobjectId() != nullptr) {
            env->DeleteGlobalRef(basicJniValue.getAsJobjectId());
        }
    }
    globalRefArguments.clear();
}

bool JniArgumentRepository::isEmpty() {
    return globalRefArguments.empty();
}


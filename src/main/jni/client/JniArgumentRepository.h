#ifndef PROJECT_JNIARGUMENTREPOSITORY_H
#define PROJECT_JNIARGUMENTREPOSITORY_H

#include "JniValue.h"
#include <deque>

using std::deque;

/**
 * Stores arguments of JNI calls and creates the global reference for each reference argument.
 */
class JniArgumentRepository {
public:
    static JniArgumentRepository& getInstance();

    void pushArgument(JNIEnv * env, BasicJniValue& basicJniValue);
    const BasicJniValue popArgument(JNIEnv * env);
    const BasicJniValue popReturnResult();

    bool isEmpty();
    void clearArguments(JNIEnv * env);

private:
    static JniArgumentRepository argumentRepository;
    deque<BasicJniValue> globalRefArguments;
};

#endif //PROJECT_JNIARGUMENTREPOSITORY_H

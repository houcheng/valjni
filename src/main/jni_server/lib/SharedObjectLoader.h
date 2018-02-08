#ifndef SRC_LIB_SHAREDOBJECTLOADER_H_
#define SRC_LIB_SHAREDOBJECTLOADER_H_

#include <string>
#include <vector>
#include <unordered_map>
using std::vector;
using std::unordered_map;
using std::string;

#include "JniValue.h"

class JniCallMessage;

class Loader {

public:
    static Loader* getInstance();

public:
    Loader(JNIEnv *jni);
    unique_ptr<JniValue> callFunction(JniCallMessage* message, vector<unique_ptr<JniValue>>& values);

private:
    static void* mHandle;
    static Loader* singleton_;

private:
    unordered_map<string, void *> dl_handles_;
    JNIEnv* jniEnv;

private:
    void* loadSymbol(string path, string symbol);
    void* loadSharedObject(string path);

    JniRawValue call(void* func_ptr, bool is_real);
    JniRawValue call(void* func_ptr, bool is_real, JniRawValue arg0);
    JniRawValue call(void* func_ptr, bool is_real, JniRawValue arg0, JniRawValue arg1);
    JniRawValue call(void* func_ptr, bool is_real, JniRawValue arg0, JniRawValue arg1, JniRawValue arg2);
    JniRawValue call(void* func_ptr, bool is_real, JniRawValue arg0, JniRawValue arg1, JniRawValue arg2, JniRawValue arg3);
    JniRawValue call(void* func_ptr, bool is_real, JniRawValue arg0, JniRawValue arg1, JniRawValue arg2, JniRawValue arg3, JniRawValue arg4);
    JniRawValue call(void* func_ptr, bool is_real, JniRawValue arg0, JniRawValue arg1, JniRawValue arg2, JniRawValue arg3, JniRawValue arg4, JniRawValue arg5);
    JniRawValue call(void* func_ptr, bool is_real, JniRawValue arg0, JniRawValue arg1, JniRawValue arg2, JniRawValue arg3, JniRawValue arg4, JniRawValue arg5, JniRawValue arg6);
    JniRawValue call(void* func_ptr, bool is_real, JniRawValue arg0, JniRawValue arg1, JniRawValue arg2, JniRawValue arg3, JniRawValue arg4, JniRawValue arg5, JniRawValue arg6, JniRawValue arg7);

    vector<JniValue*> extractFloatArguments(const vector<unique_ptr<JniValue>>& values);
    vector<JniValue*> extracNonFloatArguments(const vector<unique_ptr<JniValue>>& values);
    JniRawValue collectReturnFloat();

private:
    static void setupFloatArguments(const vector<JniValue*>& values);
    static void putFloat(int position, float value);
    static void putDouble(int position, double value);

};

#endif /* SRC_LIB_SHAREDOBJECTLOADER_H_ */

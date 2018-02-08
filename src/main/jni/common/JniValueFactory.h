#ifndef JNI_VALUE_FACTORY
#define JNI_VALUE_FACTORY

#include <string>
#include "json.hpp"
#include "JniValue.h"
#include "JniRawValue.h"

using std::string;
using std::unique_ptr;
using JSON = nlohmann::json;

class JniValueFactory {
public:
    unique_ptr<JniValue> buildFromJson(const JSON& json);
    unique_ptr<JniValue> build(const string& signature, JniRawValue jniRawValue);

    unique_ptr<JniValue> build(int value);
    unique_ptr<JniValue> build(const SharedMemory& value);
    unique_ptr<JniValue> build(jobject value);
    unique_ptr<JniValue> build(jfloat value);
    unique_ptr<JniValue> build(jdouble value);
};

#endif /* JNI_VALUE_FACTORY */

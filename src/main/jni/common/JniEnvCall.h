#ifndef SRC_MAIN_JNI_CLIENT_JNIENVCALL_H_
#define SRC_MAIN_JNI_CLIENT_JNIENVCALL_H_

#include <string>
#include <vector>
using namespace std;

#include "json.hpp"
using JSON = nlohmann::json;

#include "JniCall.h"

/**
 * Processors lookahead JSON field and creates a JniEnvCall object.
 */
class JniEnvCall : public JniCall {
public:
    JniEnvCall();
    JniEnvCall(const JSON& json);

public:
    class Builder {
    public:
        Builder& setMethodName(const char *method_name);
        Builder& setReturnValue(JniValue* return_value);
        Builder& pushArgument(JniValue* argument);

        unique_ptr<JniEnvCall> build();

    private:
        string methodName = "";
        unique_ptr<JniValue> returnValue;
        vector<unique_ptr<JniValue>> arguments;
    };
    static Builder builder();
};


#endif /* SRC_MAIN_JNI_CLIENT_JNIENVCALL_H_ */

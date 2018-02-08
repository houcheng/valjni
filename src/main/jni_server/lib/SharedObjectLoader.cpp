#include "SharedObjectLoader.h"

#include <queue>
#include "JniValueFactory.h"
#include "JniMessage.h"


using JSON = nlohmann::json;
using std::string;

void *Loader::mHandle;

Loader * Loader::getInstance() {
    if (singleton_ == NULL) {
        singleton_ = new Loader(nullptr);
    }
    return singleton_;
}

Loader::Loader(JNIEnv* jniEnv1) : jniEnv(jniEnv1) {
}

void * Loader::loadSymbol(string path, string symbol) {
    mHandle = loadSharedObject(path);

    void *address = dlsym(mHandle, symbol.c_str());
    if (address == NULL) {
        throw JniException("Can not find symbol of:" + symbol);
    }

    return address;
}

void * Loader::loadSharedObject(string path) {
    if (dl_handles_[path] == NULL) {
        dl_handles_[path] = dlopen(path.c_str(), RTLD_LAZY);
    }
    if (dl_handles_[path] == NULL) {
        throw JniException("Load file failed:" + path);
    }

    return dl_handles_[path];
}

void setupFloatArguments(int count, BasicJniValue* argv[]) {
    float f;
    JniRawValue r = argv[0]->getAsRaw();
    memcpy(&f, &r, sizeof(f));
    asm( "movss %0,%%xmm0": :"m"(f):"%xmm0");

    r = argv[1]->getAsRaw();
    memcpy(&f, &r, sizeof(f));
    asm( "movss %0,%%xmm0": :"m"(f):"%xmm1");

}

void Loader::putDouble(int position, double value) {
    if (position == 0) {
        return;
    } else if (position == 1) {
        asm( "movsd %0,%%xmm1": :"m"(value):);
    } else if (position == 2) {
        asm( "movsd %0,%%xmm2": :"m"(value):);
    } else if (position == 3) {
        asm( "movsd %0,%%xmm3": :"m"(value):);
    } else if (position == 4) {
        asm( "movsd %0,%%xmm4": :"m"(value):);
    } else if (position == 5) {
        asm( "movsd %0,%%xmm5": :"m"(value):);
    } else {
        throw JniException("Unsupported six or more floating getArguments.");
    }
}

void Loader::putFloat(int position, float value) {
    if (position == 0) {
        return;
    } else if (position == 1) {
        asm( "movss %0,%%xmm1": :"m"(value):);
    } else if (position == 2) {
        asm( "movss %0,%%xmm2": :"m"(value):);
    } else if (position == 3) {
        asm( "movss %0,%%xmm3": :"m"(value):);
    } else if (position == 4) {
        asm( "movss %0,%%xmm4": :"m"(value):);
    } else if (position == 5) {
        asm( "movss %0,%%xmm5": :"m"(value):);
    } else {
        throw JniException("Unsupported six or more floating getArguments.");
    }
}

JniRawValue Loader::collectReturnFloat() {
    JniRawValue raw_value;
    float value;
    asm( "movss %%xmm0, %0": "=m"(value): :);
    memcpy(&raw_value, &value, sizeof(value));
    return raw_value;
}


void Loader::setupFloatArguments(const vector<JniValue*>& values) {
    for(int i = values.size()-1; i >= 0; i--) {
        BasicJniValue* value = (BasicJniValue*) values[i];
        if (value->getType() == JniValueType::JNI_VALUE_FLOAT) {
            putFloat(i, value->getAsFloat());
        } else if (value->getType() == JniValueType::JNI_VALUE_DOUBLE) {
            putDouble(i, value->getAsDouble());
        }
    }
}

unique_ptr<JniValue> Loader::callFunction(JniCallMessage * message, vector<unique_ptr<JniValue>>& values) {
    cerr << message->toJsonString() << endl;

    void* func_ptr = loadSymbol(message->getJniCall()->getSharedObjectPath(), message->getJniCall()->getMethodName());

    const vector<JniValue*> float_arguments = extractFloatArguments(values);
    const vector<JniValue*> int_arguments = extracNonFloatArguments(values);
    setupFloatArguments(float_arguments);

    // TODO: changed to dynamic cast only!
    bool returnValueIsReal = ((BasicJniValue*) message->getJniCall()->getReturnValue())->isRealNumber();
    JniRawValue raw_value;

    switch(int_arguments.size()) {
    case 0:
        raw_value = call(func_ptr, returnValueIsReal);
        break;
    case 1:
        raw_value = call(func_ptr, returnValueIsReal, int_arguments[0]->getAsRaw());
        break;
    case 2:
        raw_value = call(func_ptr, returnValueIsReal, int_arguments[0]->getAsRaw(),
                         int_arguments[1]->getAsRaw());
        break;
    case 3:
        raw_value = call(func_ptr, returnValueIsReal, int_arguments[0]->getAsRaw(),
                         int_arguments[1]->getAsRaw(),
                         int_arguments[2]->getAsRaw());
        break;
    case 4:
        raw_value = call(func_ptr, returnValueIsReal, int_arguments[0]->getAsRaw(),
                         int_arguments[1]->getAsRaw(),
                         int_arguments[2]->getAsRaw(),
                         int_arguments[3]->getAsRaw());
        break;
    case 5:
        raw_value = call(func_ptr, returnValueIsReal, int_arguments[0]->getAsRaw(),
                         int_arguments[1]->getAsRaw(),
                         int_arguments[2]->getAsRaw(),
                         int_arguments[3]->getAsRaw(),
                         int_arguments[4]->getAsRaw());
        break;
    case 6:
        raw_value = call(func_ptr, returnValueIsReal, int_arguments[0]->getAsRaw(),
                         int_arguments[1]->getAsRaw(),
                         int_arguments[2]->getAsRaw(),
                         int_arguments[3]->getAsRaw(),
                         int_arguments[4]->getAsRaw(),
                         int_arguments[5]->getAsRaw());
        break;
    case 7:
        raw_value = call(func_ptr, returnValueIsReal, int_arguments[0]->getAsRaw(),
                         int_arguments[1]->getAsRaw(),
                         int_arguments[2]->getAsRaw(),
                         int_arguments[3]->getAsRaw(),
                         int_arguments[4]->getAsRaw(),
                         int_arguments[5]->getAsRaw(),
                         int_arguments[6]->getAsRaw());
        break;
    case 8:
        raw_value = call(func_ptr, returnValueIsReal, int_arguments[0]->getAsRaw(),
                         int_arguments[1]->getAsRaw(),
                         int_arguments[2]->getAsRaw(),
                         int_arguments[3]->getAsRaw(),
                         int_arguments[4]->getAsRaw(),
                         int_arguments[5]->getAsRaw(),
                         int_arguments[6]->getAsRaw(),
                         int_arguments[7]->getAsRaw());
        break;
    default:
        throw JniException("No more getArguments are supported.");
        break;
    }

    float f1;
    memcpy(&f1, &raw_value, sizeof(f1));
    std::cerr << "[Server] float result is: " << f1 << endl;

    int i1;
    memcpy(&i1, &raw_value, sizeof(f1));
    std::cerr << "[Server] integer result is: " << i1 << endl;

    JniValueFactory factory;
    return factory.build(message->getJniCall()->getReturnValue()->getSignature(), raw_value);
}

typedef JniRawValue (*jni_function_call_no_arg)(void *env, void *jobject);
typedef JniRawValue (*jni_function_call_1_arg)(void *env, void *jobject, JniRawValue arg0);
typedef JniRawValue (*jni_function_call_2_arg)(void *env, void *jobject, JniRawValue arg0, JniRawValue arg1);
typedef JniRawValue (*jni_function_call_3_arg)(void *env, void *jobject, JniRawValue arg0, JniRawValue arg1, JniRawValue arg2);
typedef JniRawValue (*jni_function_call_4_arg)(void *env, void *jobject, JniRawValue arg0, JniRawValue arg1, JniRawValue arg2, JniRawValue arg3);
typedef JniRawValue (*jni_function_call_5_arg)(void *env, void *jobject, JniRawValue arg0, JniRawValue arg1, JniRawValue arg2, JniRawValue arg3, JniRawValue arg4);
typedef JniRawValue (*jni_function_call_6_arg)(void *env, void *jobject, JniRawValue arg0, JniRawValue arg1, JniRawValue arg2, JniRawValue arg3, JniRawValue arg4, JniRawValue arg5);
typedef JniRawValue (*jni_function_call_7_arg)(void *env, void *jobject, JniRawValue arg0, JniRawValue arg1, JniRawValue arg2, JniRawValue arg3, JniRawValue arg4, JniRawValue arg5, JniRawValue arg6);
typedef JniRawValue (*jni_function_call_8_arg)(void *env, void *jobject, JniRawValue arg0, JniRawValue arg1, JniRawValue arg2, JniRawValue arg3, JniRawValue arg4, JniRawValue arg5, JniRawValue arg6, JniRawValue arg7);

JniRawValue Loader::call(void* func_ptr, bool is_real) {
    jni_function_call_no_arg func = (jni_function_call_no_arg) func_ptr;
    JniRawValue return_value_ = (*func)(jniEnv, NULL);
    if (is_real) {
        return collectReturnFloat();
    }
    return return_value_;
}

JniRawValue Loader::call(void* func_ptr, bool is_real, JniRawValue arg0) {
    jni_function_call_1_arg func = (jni_function_call_1_arg) func_ptr;
    JniRawValue return_value_ = (*func)(jniEnv, NULL, arg0);
    if (is_real) {
        return collectReturnFloat();
    }
    return return_value_;
}

JniRawValue Loader::call(void* func_ptr, bool is_real, JniRawValue arg0, JniRawValue arg1) {
    jni_function_call_2_arg func = (jni_function_call_2_arg) func_ptr;
    JniRawValue return_value_ = (*func)(jniEnv, NULL, arg0, arg1);
    if (is_real) {
        return collectReturnFloat();
    }
    return return_value_;
}

JniRawValue Loader::call(void* func_ptr, bool is_real, JniRawValue arg0, JniRawValue arg1, JniRawValue arg2) {
    jni_function_call_3_arg func = (jni_function_call_3_arg) func_ptr;
    JniRawValue return_value_ = (*func)(jniEnv, NULL, arg0, arg1, arg2);
    if (is_real) {
        return collectReturnFloat();
    }
    return return_value_;
}

JniRawValue Loader::call(void* func_ptr, bool is_real, JniRawValue arg0, JniRawValue arg1, JniRawValue arg2, JniRawValue arg3) {
    jni_function_call_4_arg func = (jni_function_call_4_arg) func_ptr;
    JniRawValue return_value_ = (*func)(jniEnv, NULL, arg0, arg1, arg2, arg3);
    if (is_real) {
        return collectReturnFloat();
    }
    return return_value_;
}

JniRawValue Loader::call(void* func_ptr, bool is_real, JniRawValue arg0, JniRawValue arg1, JniRawValue arg2, JniRawValue arg3, JniRawValue arg4) {
    jni_function_call_5_arg func = (jni_function_call_5_arg) func_ptr;
    JniRawValue return_value_ = (*func)(jniEnv, NULL, arg0, arg1, arg2, arg3, arg4);
    if (is_real) {
        return collectReturnFloat();
    }
    return return_value_;
}

JniRawValue Loader::call(void* func_ptr, bool is_real, JniRawValue arg0, JniRawValue arg1, JniRawValue arg2, JniRawValue arg3, JniRawValue arg4, JniRawValue arg5) {
    jni_function_call_6_arg func = (jni_function_call_6_arg) func_ptr;
    JniRawValue return_value_ = (*func)(jniEnv, NULL, arg0, arg1, arg2, arg3, arg4, arg5);
    if (is_real) {
        return collectReturnFloat();
    }
    return return_value_;
}

JniRawValue Loader::call(void* func_ptr, bool is_real, JniRawValue arg0, JniRawValue arg1, JniRawValue arg2, JniRawValue arg3, JniRawValue arg4, JniRawValue arg5, JniRawValue arg6) {
    jni_function_call_7_arg func = (jni_function_call_7_arg) func_ptr;
    JniRawValue return_value_ = (*func)(jniEnv, NULL, arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    if (is_real) {
        return collectReturnFloat();
    }
    return return_value_;
}

JniRawValue Loader::call(void* func_ptr, bool is_real, JniRawValue arg0, JniRawValue arg1, JniRawValue arg2, JniRawValue arg3, JniRawValue arg4, JniRawValue arg5, JniRawValue arg6, JniRawValue arg7) {
    jni_function_call_8_arg func = (jni_function_call_8_arg) func_ptr;
    JniRawValue return_value_ = (*func)(jniEnv, NULL, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    if (is_real) {
        return collectReturnFloat();
    }
    return return_value_;
}

Loader * Loader::singleton_ = NULL;

vector<JniValue*> Loader::extractFloatArguments(const vector<unique_ptr<JniValue>>& values) {
    vector<JniValue*> extracted_values;
    for(auto&& value: values) {
        // TODO: changed to dynamic cast only!
        if (((BasicJniValue*)value.get())->isRealNumber()) {
            extracted_values.push_back(value.get());
        }
    }
    return extracted_values;
}

vector<JniValue*> Loader::extracNonFloatArguments(const vector<unique_ptr<JniValue>>& values) {
    vector<JniValue*> extracted_values;
    for(auto&& value: values) {
        // TODO: changed to dynamic cast only!
        if (! ((BasicJniValue*)value.get())->isRealNumber()) {
            extracted_values.push_back(value.get());
        }
    }
    return extracted_values;
}

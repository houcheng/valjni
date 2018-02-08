package com.valjni.caller;

import com.fasterxml.jackson.databind.ObjectMapper;

import java.io.IOException;
import java.lang.reflect.Array;

/**
 * Calls C based JniClientMain
 */
public class JniClientImplementation implements JniClientInterface {
    private ObjectMapper mapper = new ObjectMapper();
    private JniValue jniReturnValue = null;

    private native void invokeJniClientMain(String sharedObjectPath, String methodName);
    private native void pushJint(int argument);
    private native void pushJfloat(float argument);
    private native void pushJobject(Object argument);
    private native void pushJarray(Object argument);

    // TODO: pushJdouble, pushJbyte, etc.

    public JniClientImplementation() {
        mapper.enableDefaultTyping(ObjectMapper.DefaultTyping.NON_FINAL);
        System.load("/home/houcheng/workspace/valjni/src/main/build/libjni_client.so");
    }

    @Override
    public Object invokeJni(JniCall jniCall) throws IOException {
        jniReturnValue = null;
        for (Object argument : jniCall.getArguments()) {
            pushArgument(argument);
        }
        pushReturnValue(jniCall.getMethod().getReturnType());
        invokeJniClientMain(jniCall.getSharedObjectPath(), jniCall.getJniMethodName());
        return jniReturnValue.asPrimitiveObject();
    }


    private void pushReturnValue(Class<?> returnType) {
        if (isArrayClass(returnType)) {
            pushJarray(null);
        } else if (returnType == int.class) {
            pushJint(0);
        } else if (returnType == float.class) {
            pushJfloat(0);
        } else {
            pushJobject(null);
        }
    }

    private void pushArgument(Object argument) {
        if (isArrayInstance(argument)) {
            pushJarray(argument);
        } else if (argument instanceof Integer) {
            pushJint((int) argument);
        } else if (argument instanceof Float) {
            pushJfloat((float) argument);
        // TODO: more pushJdouble, boolean, byte, etc.
        } else {
            pushJobject(argument);
        }
    }

    private boolean isArrayInstance(Object argument) {
        return isArrayClass(argument.getClass());
    }

    private boolean isArrayClass(Class<?> returnType) {
        return returnType.getComponentType() != null;
    }

    // TODO: Change updateJniReturn(signature, value) to (jsonString)
    private void updateJniReturn(String signature, String value) {
        jniReturnValue = new JniValue(signature, value);
    }
}

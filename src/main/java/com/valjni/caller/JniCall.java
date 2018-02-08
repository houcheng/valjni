package com.valjni.caller;

import lombok.Builder;
import lombok.Getter;
import lombok.NoArgsConstructor;

import java.lang.reflect.Method;
import java.util.LinkedList;
import java.util.List;

/**
 * Stores prototype and argument returnValueString of one JNI call.
 */
@Builder
@Getter
public class JniCall {
    private String sharedObjectPath;
    private Object callObject;
    private Method method;
    private List<Object> arguments = new LinkedList<>();

    public String getJniMethodName() {
        String className = callObject.getClass().getName().replace('.', '_');
        return "Java_" + className + '_' + method.getName();
    }
}

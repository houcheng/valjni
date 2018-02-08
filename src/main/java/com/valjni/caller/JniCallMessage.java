package com.valjni.caller;

import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Getter;

import java.util.LinkedList;

@Getter
@Builder
@AllArgsConstructor
public class JniCallMessage extends JniMessage {
    private String sharedObjectPath;
    private String methodName;
    private JniValue returnValue;

    final private class JniValueList extends LinkedList <JniValue> { }
    private JniValueList argumentValues = new JniValueList();

    public JniCallMessage(JniCall jniCall) {
        sharedObjectPath = jniCall.getSharedObjectPath();

        // TODO: bad here
        String className = jniCall.getCallObject().getClass().getName().replace('.', '_');
        methodName = "Java_" + className + '_' + jniCall.getMethod().getName();

        returnValue = JniValue.builder()
                .type(jniCall.getMethod().getReturnType())
                .value(null)
                .build();

        int index = 0;
        for (Class parameterType : jniCall.getMethod().getParameterTypes()) {
            Object argumentValue = jniCall.getArguments().get(index);
            JniValue jniValue = JniValue.builder()
                    .type(parameterType)
                    .value(argumentValue)
                    .build();
            argumentValues.add(jniValue);
            index ++;
        }
    }
}

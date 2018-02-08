package com.valjni.example;

/**
 * Calls example jni functions in libsample.so.
 */
public class JniExample {

    public JniExample() {
        System.load("/home/houcheng/workspace/valjni/src/main/build/libexample.so");
    }

    public native int addJni(int a, int b);
    public native float addFloatJni(float a, float b);
    public native int sumJni(int[] intData);

    public int add(int a, int b) {
        return addJni(a, b);
    }

    public float addFloat(float a, float b) {
        return addFloatJni(a, b);
    }
}


package com.valjni.sample;

/**
 * Wrapper class for utility jni functions in libutility.so.
 */
public class JniUtilityWrapper {

    public JniUtilityWrapper() {
        System.load("./libutility.so");
    }

    private native int add(int a, int b);

}


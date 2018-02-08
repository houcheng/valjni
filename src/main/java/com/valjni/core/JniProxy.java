package com.valjni.core;

import net.bytebuddy.ByteBuddy;
import net.bytebuddy.NamingStrategy;
import net.bytebuddy.agent.builder.AgentBuilder;
import net.bytebuddy.dynamic.DynamicType;
import net.bytebuddy.dynamic.loading.ClassLoadingStrategy;
import net.bytebuddy.implementation.FixedValue;
import net.bytebuddy.implementation.MethodDelegation;
import net.bytebuddy.implementation.bind.annotation.AllArguments;

import java.lang.annotation.Annotation;
import java.lang.annotation.Native;
import java.lang.instrument.Instrumentation;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.LinkedList;
import java.util.List;

import static net.bytebuddy.matcher.ElementMatchers.isNative;
import static net.bytebuddy.matcher.ElementMatchers.named;

class NativeMethodDelegate {

}

/**
 * Currently support oneshot jni call only.
 */
public class JniProxy {

    private Object jniObject;
    private Class jniClass;
    private JniProcessClient jniProcessClient;

    private ByteBuddy byteBuddy;
    private DynamicType.Builder<?> builder;

    public JniProxy(Class jniClass, JniProcessClient client) {
        this.jniClass = jniClass;
        this.jniProcessClient = client;

        byteBuddy = new ByteBuddy()
                .with(new NamingStrategy.SuffixingRandom("JniProxy"));

        builder = byteBuddy
                .subclass(jniClass)
                .method(named("toString"))
                .intercept(FixedValue.value("HelloWorld"));
    }

    public JniProxy(Object jniObject, JniProcessClient client) {
        this(jniObject.getClass(), client);
    }

    public Object  createInstance() {

        builder = builder
                .method(named("addJni"))
                .intercept(MethodDelegation.to(NativeMethodDelegate.class));

        try {
            return builder
                    .make()
                    .load(jniClass.getClassLoader(), ClassLoadingStrategy.Default.INJECTION)
                    .getLoaded()
                    .newInstance();
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        }
        return null;
    }


}

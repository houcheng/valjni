package com.valjni.core;

import com.valjni.caller.*;
import com.valjni.caller.JniClientImplementation;
import com.valjni.caller.JniClientInterface;
import net.bytebuddy.implementation.bind.annotation.*;

import java.lang.reflect.Method;
import java.util.Arrays;
import java.util.concurrent.Callable;

/**
 * Intercepts native jni methods.
 */
public class JniInterceptor {
    static boolean enableIntercept = true;

    /**
     * Allows the intercepted code, if they aware, to decide whether to be alerted by
     * the interceptor.
     * @param enable
     */
    public static void setEnableIntercept(boolean enable) {
        enableIntercept = enable;
    }

    @RuntimeType
    public static Object intercept(@This Object callObject, @Origin Method method, @SuperCall Callable<?> callable, @AllArguments Object[] arguments) throws Exception {
        if (enableIntercept) {

            String sharedObjectPath = JniInterceptAgent.getSharedObjectPath(method.getDeclaringClass().getName());
            JniCall jniCall = JniCall
                    .builder()
                    .sharedObjectPath(sharedObjectPath)
                    .callObject(callObject)
                    .method(method)
                    .arguments(Arrays.asList(arguments))
                    .build();

            JniClientInterface jniClientInterface = new JniClientImplementation();
            return jniClientInterface.invokeJni(jniCall);
        }

        System.out.println("TODO: Disable the callable later.");
        return callable.call();
    }
}

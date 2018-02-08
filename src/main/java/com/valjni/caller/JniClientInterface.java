package com.valjni.caller;

import java.io.IOException;

public interface JniClientInterface {
    Object invokeJni(JniCall jniCall) throws IOException;
}


Valgrind for JNI C shared libraries
===================================

The valjni library helps detecting JNI code's memory leak with valgrind utility. The valgrind emulates the process'es execution model and finds memory leak in the emulated machine. However, it cannot be use with JNI code as the JVM's memory management activities and the byte-code execution model is not well emulated by the valgrind tool. The valjni library solves this problem by moving the JNI native code into another standalone process and translates the JNI call into RPC call. Each JNI call is executed in the newly created process monitored by valgrind and use RPC mechanism for send call arguments and return values.

The follow features are implemented:

- Automatic JNI call detection by byte-code instrumentation, byte buddy.
- Automatic create JNI process.
- Automatic JNI arguments translation into RPC arguments.
- Emulated JNI runtime environment that synchronizes array data between JVM and JNI process.


Installation and build
======================

    sudo apt install libboost-all-dev
    sudo apt install binutils-dev  # for C++ beautiful stacktrace library, backward.hpp.

Build steps:

    export JAVA_HOME=/usr/lib/jvm/java-1.8.0-openjdk-amd64
    export CXX=clang++
    export CC=clang
    cmake <dir>

Design
======

```
     +----+ jni call                =================
     |java|--------------->         |native jni code|
     |code|               ^         =================
     |    |               |
     +----+               | intercept
                     +--------------+            =================            =================           ===================
                     |instrumention | jni call   | valjni client | rpc call   | valjni server | jni call  | native jni code |
                     |created proxy |----------> | (in C)        |----------->| (in C)        |---------->| (in C)          |
                     +--------------+            =================            =================           ===================
```


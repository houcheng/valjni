# Design of JNI environment simulation

Two problems has to resolve:

- JNI C code calls Java method
- JNI C code calls primitive array request and release

One extra problem:

- Can JniValue store enough information for all the possible scenario and no redundant?
    - JNI call arguments: argument value and type is fine. For array primitive, should add a flag to represent array.
    - JNI call return: can be no value for representing return type and store value when return.
    - JNI call Java arguments: the same as above.
    - JNI call Java return: the same as above.
    - JNI array acquire: the same as above with array flag is true.
    - JNI array release: the same as above.

## Solutions

### JNI C code calls Java method

The arguments and return values could be passed by JniValue represented value in string. For non-primitive types, it can also represented by its ID. There should be no other kinds of type in Java world and no need the memory copy for calling Java method.

**done and how to prove or verify?**

**Stepwise verify**

- Mocked JNI environment in JNI server received an Java method call request. It forward request to JNI client and wait for response.
- Then JNI client receives an method call request:
    - Calls the JNI environment for this method. Primitive arguments are passed by value and non-primitive are passed by ID. The JniValue object should handle this part.
    - Send the Java method call reply back.
- Then the mocked JNI environment receives an method call reply:
    - Parse the result.
    - Return control to original C code.

### JNI C code calls primitive array request and release

When requesting, a memory block would be return by JNI environment. Create the share memory with the same size, copy the array content into share memory and send the shared memory ID back to the JNI server. When releasing, copy the shared memory back to array content and release it.

**done and how to prove or verify?**

**Stepwise verify**

- Mocked JNI environment in JNI server received an array request. It would forward it to JNI client and wait for response.
- Then the JNI client receives an array request:
    - Calls JNI environment to get the array memory block.
    - Create share memory and copy the array memory into shared memory.
    - Send the array reply with shared memory ID to JNI server.
- Then the mocked JNI environment, in JNI server side, receives an array reply:
    - Access the share memory by ID.
    - Return the control to the original code.

## Java handle to JNI id

The JNI method's array-id/ object-id argument is converted from Java handle by JVM call processing code. These JNI-did in C and handle in Java are not the same thing. We need a service to convert the Java handle for array or object from into C JNI-id. In C space, the JNI id looks like a data pointer and points to a object containing reference to Java handle.

If the array-id for C is persistent among JNI calls, we can have a service to Java code for converting Java handle to JNI-id. If not, our interceptor must rely on JVM for converting the handles.

Can we gaurantee the JNI-id is unchanged among many JNI calls? Use global reference.

http://hsienwei.blogspot.tw/2013/05/jnideletelocalref.html


Creating a service to lookup JNI-id before calling the JNI which creates a JNI global reference for each handle.
Then in our jni-client code, re-create the local references and delete the global reference.



If you want to keep jobject (like jbyteArray) between JNI calls you need to make it a GlobalRef:

jbyteArray temp_buffer = env->NewByteArray(buflen);
buffer = (jbyteArray)env->NewGlobalRef(temp_buffer);


**Study**

The handle in Java is actually a pointer reference to oop structure. So, the handle should never changes, and the oop may never change and only oop's content changes. We may assume the array-id in C-space never changes.

`In general, the garbage collector may move Java arrays. However, the Java Virtual Machine guarantees that the result of GetIntArrayElements points to a nonmovable array of integers`

The JNI would locks the oop before entering the JNI method. So, if we want to gaurantee it is unchanged, we must PIN it.


It looks like that the JNI's array-id arguments, it is called JNIHandles and is actually a pointer to oop structure. The question is what is relation from Java Handle to JNI handle ?


    inline oop JNIHandles::resolve(jobject handle) {
      oop result = (handle == NULL ? (oop)NULL : *(oop*)handle);
      assert(result != NULL || (handle == NULL || !CheckJNICalls || is_weak_global_handle(handle)), "Invalid value read from jni handle");
      assert(result != badJNIHandle, "Pointing to zapped jni handle area");
      return result;
    };
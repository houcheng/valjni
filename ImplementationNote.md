Implementation Note
===================


The jni-call request contains 1) shared object path, 2) function prototype and 3) function parameter value

- share object name
- function type
- function name
- list of parameter type
- list of parameter value

The jni-call reply contains:

- return type and value

The connection between proxy and jni server is handled by zmq, zero message queue protocol.

reference: http://docs.oracle.com/javase/7/docs/technotes/guides/jni/spec/types.html


JNI message format
------------------

The Java program uses JNI message to communicate with the spwaned child process. It has the folllowing types of message:

- JNI call message
- JNI reply message


Todo
----

- Support all functions defined in jni.h



## Reference

Here are some comments on comparing byte buddy, cglib and java asistant in reddit:

The two biggest competitors to Byte Buddy today are javassist and cglib. Both of them work quite differently:
javassist takes strings containing Java code and compiles these strings at runtime. This makes it simple to use as every Java developer knows Java. Unfortunately, the javassist compiler lags a lot behind the actual javac compiler. For example, it does not support generics or even auto-boxing, not to speak of lambda expressions. Also, Strings are not type-safe what makes javassist code quite error prone. (It is the equivalent of concenating SQL strings together, with all the risks like foreign code injection.)

cglib allows you to register callbacks for an overridden class, similar to the JDK proxies. However, this requires that all class loaders know about cglib if they want to know these cglib-proxies as cglib-specific types are hard-coded into any generated class. Also, cglib only allows subclassing but not the rewrital of existing classes. Furthermore, the callbacks require any arguments to be boxed and registeres these boxed values into arrays. It turns out that cglib is not quite Just-In-Time-compiler friendly.
Byte Buddy works with annotations or its type-safe DSL and realizes its instrumentation mostly based on delegating to user code. This way, you can implement interceptors in any JVM language and you avoid dependencies on Byte Buddy within the generated classes (annotations are fortunately ignored by JVM class loaders if they cannot be resolved at load-time). This makes for example for better exception stack traces than when using "string code". Also, Byte Buddy avoids boxing by creating smarter objects for the interceptors and only creates those interceptors that are requested by annotated arguments, using dependency injection.

ASM or BCEL are rather byte code parser than code generation libraries. You need to have a clear concept of how byte code works in order to use them. Also, those libraries allow you to create "unverified code", i.e. you have to do a lot manually. Byte Buddy is itself built on top of ASM, you can even plug ASM code into Byte Buddy if you need to.


**Example code to read string from constant pool**

[url is here](https://www.programcreek.com/java-api-examples/org.objectweb.asm.ClassReader)

    /**
     * Searches a class's constant pool for the specified list of strings.
     *
     * NOTE: Constant pool strings are trimmed of whitespace!  Take into
     * account when matching.
     *
     * @param className - The name of the class to search.
     * @param matchStrings - The list of strings to find.
     * @return True if all strings were found.
     */
    public static boolean searchConstantPoolForStrings(String className, String... matchStrings)
    {
      if (className == null) return false;
      className = className.replace(".", "/");
      InputStream stream = DynamicMappings.class.getClassLoader().getResourceAsStream(className + ".class");
      if (stream == null) return false;

      ClassReader reader = null;
      try {
        reader = new ClassReader(stream);
      } catch (IOException e) { return false; }

      int itemCount = reader.getItemCount();
      char[] buffer = new char[reader.getMaxStringLength()];

      int matches = 0;

      for (int n = 1; n < itemCount; n++) {
        int pos = reader.getItem(n);
        if (pos == 0 || reader.b[pos - 1] != 8) continue;

        Arrays.fill(buffer, (char)0);
        String string = reader.readUTF8(pos,  buffer).trim();
        //String string = (new String(buffer)).trim();

        for (int n2 = 0; n2 < matchStrings.length; n2++) {
          if (string.equals(matchStrings[n2].trim())) { matches++; break; }
        }
      }

      return (matches == matchStrings.length);


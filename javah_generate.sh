./gradlew build
cd build/classes/main
javah com.valjni.caller.JniClientImplementation
mv com_valjni_caller_JniClientImplementation.h ../../../src/main/jni/client

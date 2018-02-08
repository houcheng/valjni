package com.valjni.core;

import com.google.common.base.Splitter;
import net.bytebuddy.agent.builder.AgentBuilder;
import net.bytebuddy.implementation.MethodDelegation;
import net.bytebuddy.jar.asm.*;
import org.apache.commons.lang3.StringUtils;

import java.io.IOException;
import java.io.InputStream;
import java.lang.instrument.Instrumentation;
import java.util.HashMap;
import java.util.Map;

import static net.bytebuddy.matcher.ElementMatchers.isNative;
import static net.bytebuddy.matcher.ElementMatchers.named;

/**
 * Acts as a Java agent for intercepting native method.
 */
public class JniInterceptAgent {
    private static Map<String, String> classNameToSharedObjectPath = new HashMap<>();

    public static void premain(String arg, Instrumentation instrumentation) {
        Splitter argSplitter = Splitter.on(",").omitEmptyStrings().trimResults();
        Class interceptClass = JniInterceptor.class;

        for (String jniClassName: argSplitter.split(arg)) {
            installTransformForJniClass(interceptClass, jniClassName, instrumentation);
            updateSharedObjectPathFromByteCode(jniClassName);
        }
    }

    private static void updateSharedObjectPathFromByteCode(String jniClassName) {
        ClassReader classReader = createClassReader(jniClassName);

        ClassVisitor classVisitor = new ClassVisitor(Opcodes.ASM6) {
            @Override
            public MethodVisitor visitMethod(int access, String name, String desc, String signature, String[] exceptions) {
                return new MethodVisitor(Opcodes.ASM6) {
                    String lastLoadString;
                    @Override
                    public void visitLdcInsn(Object cst) {
                        lastLoadString = cst.toString();
                        super.visitLdcInsn(cst);
                    }
                    @Override
                    public void visitMethodInsn(int opcode, String owner, String name,
                                 String desc, boolean itf) {
                        if ((opcode == Opcodes.INVOKESTATIC) && StringUtils.equals(owner, "java/lang/System") &&
                                StringUtils.equals(name, "load")) {
                            classNameToSharedObjectPath.put(jniClassName, lastLoadString);
                        }
                    }
                };
            }
        };

        classReader.accept(classVisitor, ClassReader.SKIP_DEBUG);
    }

    public static String getSharedObjectPath(String className) {
        return classNameToSharedObjectPath.get(className);
    }

    private static ClassReader createClassReader(String jniClassName) {
        String resName = "/" + jniClassName.replace('.', '/') + ".class";
        try {
            InputStream byteCodeStream = JniInterceptAgent.class.getResourceAsStream(resName);
            return new ClassReader(byteCodeStream);
        } catch (IOException e) {
            e.printStackTrace();
            throw new RuntimeException("Class bytecode not found:" + resName);
        }
    }

    private static void installTransformForJniClass(Class interceptClass, String jniClassName, Instrumentation instrumentation) {
        new AgentBuilder.Default()
                .enableNativeMethodPrefix("foo")
                .type(named(jniClassName))
                .transform((builder, typeDescription, classLoader, m) -> builder
                        .method(isNative())
                        .intercept(MethodDelegation.to(interceptClass))
                ).installOn(instrumentation);
    }
}

package com.valjni.caller;

import com.fasterxml.jackson.databind.ObjectMapper;
import org.junit.Before;
import org.junit.Test;

import java.io.IOException;

import static org.assertj.core.api.Assertions.assertThat;

public class JniMessageTest {
    private static final String[] ARGUMENT_TYPE_SIGNATURES = { "I", "I" };
    private static final String[] ARGUMENT_VALUE_STRINGS = { "3", "4" };

    private JniCallMessage jniCallMessage;
    private JniReplyMessage jniReplyMessage;
    private ObjectMapper mapper;

    @Before
    public void setUp() {
        jniCallMessage = JniCallMessage.builder()
                .methodName("add")
                .sharedObjectPath("libadd.so")
                .build();

        jniReplyMessage = JniReplyMessage.builder()
                .returnTypeSignature("I")
                .returnValueString("3")
                .build();

        mapper = new ObjectMapper();
        mapper.enableDefaultTyping(ObjectMapper.DefaultTyping.NON_FINAL);
    }

    @Test
    public void testEncodingAndDecodingJniCallMessage() throws IOException {
        JniMessage jniMessage = (JniMessage) jniCallMessage;

        String jsonString = mapper.writeValueAsString(jniMessage);
        jniMessage = mapper.readValue(jsonString, JniMessage.class);
        System.out.println(jsonString);
        assertThat(jniMessage).isInstanceOf(JniCallMessage.class);

        JniCallMessage decodedJniCallMessage = (JniCallMessage) jniMessage;
//        assertThat(decodedJniCallMessage.getMethodName()).isEqualTo("add");
//        assertThat(decodedJniCallMessage.getReturnTypeSignature()).isEqualTo("I");
//        assertThat(decodedJniCallMessage.getSharedObjectPath()).isEqualTo("libadd.so");
//
//        assertThat(decodedJniCallMessage.getArgumentTypeSignatures()).containsExactly(ARGUMENT_TYPE_SIGNATURES);
//        assertThat(decodedJniCallMessage.getArgumentValueStrings()).containsExactly(ARGUMENT_VALUE_STRINGS);
    }

    @Test
    public void testEncodingAndDecodingJniReplyMessage() throws IOException {
        JniMessage jniMessage = (JniMessage) jniReplyMessage;

        String jsonString = mapper.writeValueAsString(jniMessage);
        System.out.println(jsonString);
        jniMessage = mapper.readValue(jsonString, JniMessage.class);
        assertThat(jniMessage).isInstanceOf(JniReplyMessage.class);

        JniReplyMessage decodedReplyMessage = (JniReplyMessage) jniMessage;
        assertThat(decodedReplyMessage.getReturnTypeSignature()).isEqualTo("I");
        assertThat(decodedReplyMessage.getReturnValueString()).isEqualTo("3");

    }
}
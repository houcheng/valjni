package com.valjni.core;


import com.valjni.example.JniExample;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.runners.MockitoJUnitRunner;

import static org.assertj.core.api.Assertions.assertThat;
import static org.mockito.Mockito.verify;


@RunWith(MockitoJUnitRunner.class)
public class JniProxyTest {

    @Mock
    private JniProcessClient jniProcessClient;
    private JniExample jniObject;
    private JniProxy target;

    @Before
    public void setup() {
        // jniObject = new JniWrapperForTest();
        target = new JniProxy(JniExample.class, jniProcessClient);
    }

    @Test
    public void testJniMethodInspect() {
        // JniExample proxy = (JniExample) target.createInstance();
        // assertThat(proxy.toString()).isEqualTo("HelloWorld");
        // assertThat(proxy.addJni(3, 4)).isEqualTo(99);
        // assertThat(proxy.add(3, 8)).isEqualTo(99);
        // verify(jniProcessClient).callJni();
        int[] data = new int[3];
        System.out.println(data.getClass());
        Class<?> type = data.getClass();
        System.out.println(type.isArray());

        Integer i = new Integer(3);
        System.out.println(i.getClass());

        String string = "hello";
        type = string.getClass();
        String signature = "L" + type.getName().replace('.', '/') + ';';
        System.out.println(signature);


    }

}
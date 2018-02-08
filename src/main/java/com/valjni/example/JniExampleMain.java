package com.valjni.example;

public class JniExampleMain {
    public static void main(String[] args) {
        JniExample jniSampleWrapper = new JniExample();

        int sum = jniSampleWrapper.addJni(3, 4);
        System.out.printf("addJni(3, 4) returned: %d\n", sum);

        sum = jniSampleWrapper.add(3, 4);
        System.out.printf("add(3, 4) returned: %d\n", sum);

        float floatSum = jniSampleWrapper.addFloatJni(9.2f, 8.7f);
        System.out.printf("addFloatJni(9.2, 8.7) returned: %f\n", floatSum);

        int[] data = {1, 2, 3};
        sum = jniSampleWrapper.sumJni(data);

        System.out.printf("sumJni(1, 2, 3) returned: %d\n", sum);

    }
}

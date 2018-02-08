package com.valjni.caller;

import com.google.common.base.Splitter;
import lombok.Getter;
import org.apache.commons.lang3.StringUtils;

@Getter
public final class JniValue {
    private String signature;
    private String value;

    public JniValue(String signature, String value) {
        this.signature = signature;
        this.value = value;
    }

    public static Builder builder() {
        return new Builder();
    }

    // TODO: Complete JniValue's asPrimitiveObject function.
    // TODO: Change JNI signature to type mapping into a table.
    public Object asPrimitiveObject() {
        if (StringUtils.equals(signature, "B")) {
            return Byte.valueOf(value);
        } else if (StringUtils.equals(signature, "C")) {
            return value.charAt(0);
        } else if (StringUtils.equals(signature, "S")) {
            return Short.valueOf(value);
        } else if (StringUtils.equals(signature, "I")) {
            return Integer.valueOf(value);
        } else if (StringUtils.equals(signature, "J")) {
            return Long.valueOf(value);

        } else if (StringUtils.equals(signature, "F")) {
            return Float.valueOf(value);
        } else if (StringUtils.equals(signature, "D")) {
            return Double.valueOf(value);
        } else if (StringUtils.equals(signature, "*")) {
            return null;
        } else if (StringUtils.equals(signature, "Z")) {
            return Boolean.valueOf(value);
        }
        throw new RuntimeException("Method asPrimitiveObject is not implemented!");
    }

    public static class Builder {
        private Class<?> type;
        private Object value;

        public Builder type(Class<?> type) {
            this.type = type;
            return this;
        }
        public Builder value(Object value) {
            this.value = value;
            return this;
        }
        public JniValue build() {
            return new JniValue(convertTypeToSignature(), convertValueObjectToString());
        }

        private String convertValueObjectToString() {
            if (type.isArray()) {
                // for int array, the value string is "[I@1ba9117e"
                String pointerString = "0x";
                pointerString += Splitter.on("@").omitEmptyStrings().trimResults()
                        .splitToList(String.valueOf(value)).get(1);
                return pointerString;
            } else {
                return String.valueOf(value);
            }
        }

        // TODO: Change to mapping (signature, type) by table.
        public String convertTypeToSignature() {
            if (type == Boolean.TYPE) {
                return "Z";
            } else if (type == Byte.TYPE) {
                return "B";
            } else if (type == Character.TYPE) {
                return "C";
            } else if (type == Short.TYPE) {
                return "S";
            } else if (type == Integer.TYPE) {
                return "I";
            } else if (type == Long.TYPE) {
                return "J";
            } else if (type == Float.TYPE) {
                return "F";
            } else if (type == Double.TYPE) {
                return "D";
            } else if (type.isArray()) {
                // for int array, the typeString is "class [I"
                // return Splitter.on(" ").omitEmptyStrings().trimResults()
                //        .splitToList(type.toString()).get(1);
                return "JARRAY";
            } else if (type instanceof Object) {
                String signature = "L" + type.getName().replace('.', '/') + ';';
                return signature;
            }
            throw new RuntimeException("Method convertTypeToSignature is not implemented!");
        }
    };
}

package com.valjni.caller;

import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Getter;

class JniMessage {
}

/**
 *
 */
@Builder
@AllArgsConstructor
@Getter
class JniReplyMessage extends JniMessage {
    String returnTypeSignature;
    String returnValueString;

    Object getReturnValue() {
        if (returnTypeSignature == "I") {
            return new Integer(Integer.parseInt(returnValueString));
        }

        String messageBuffer = String.format("The return type %s is not supported yet!", returnTypeSignature);
        throw new RuntimeException(messageBuffer);
    }
}

class JniCallbackMessage extends JniMessage {
    JniCallbackMessage() {
        throw new RuntimeException("Not implemented!");
    }
}

class JniCallbackReplyMessage extends JniMessage {
    JniCallbackReplyMessage() {
        throw new RuntimeException("Not implemented!");
    }
}

#include "JniClient.h"

#include <iostream>
#include <unistd.h>
#include <jni.h>
#include "JniMessageFactory.h"
#include "JniValueFactory.h"
#include "JniArgumentRepository.h"

JniClient::JniClient(JNIEnv* jniEnv1, jobject jniClientImplementation1, JniEnvWrapper* jniEnvService1) {
    jniClientImplementation = jniClientImplementation1;
    jniEnv = jniEnv1;
    jniEnvService = jniEnvService1;
}

#define READ_FD 0
#define WRITE_FD 1

void JniClient::jniClientMain(string sharedObjectPath, string methodName) {
    auto message = createJniCallMessge(sharedObjectPath, methodName);

    int to_server_fds[2];
    int from_server_fds[2];

    pipe(to_server_fds);
    pipe(from_server_fds);

    forkChildProcess(to_server_fds, from_server_fds);

    close(from_server_fds[WRITE_FD]);
    close(to_server_fds[READ_FD]);

    toServerFd = to_server_fds[WRITE_FD];
    fromServerFd = from_server_fds[READ_FD];

    sendInitialJniCallMessageToServer(message->toJsonString() + "\n");
    cerr << "[Client] writes:" << message->toJsonString() << endl;
    tryProcessMessageLoop(message.get());
}

void JniClient::forkChildProcess(int *to_server_fds, int *from_server_fds) {
    int child_pid = fork();
    if (child_pid < 0) {
        throw JniException("Failed to fork new process.");
    }

    if (child_pid == 0) {
        close(from_server_fds[READ_FD]);
        close(to_server_fds[WRITE_FD]);
        executeInChildProcess(to_server_fds[READ_FD], from_server_fds[WRITE_FD]);
        assert(0);
    }
}

void JniClient::executeInChildProcess(int private_stdin_fd, int private_stdout_fd) {
    dup2(private_stdin_fd, 0);
    dup2(private_stdout_fd, 1);

    execlp("valgrind", "--tool=memcheck", "--leak-check=full", "--log-file=jni-memchk.txt",
          "--track-origins=yes", "./src/main/build/jni-server", nullptr);
    throw JniException("Should not go here after execl().");
}

void JniClient::sendInitialJniCallMessageToServer(const std::string &message_json) {
    ssize_t len = write(toServerFd, message_json.c_str(), message_json.size());
    if (len != (ssize_t) message_json.size()) {
        throw JniException("Send initial JNI call message to server failed.");
    }
}

void JniClient::tryProcessMessageLoop(const JniCallMessage* jniCallMessage) {
    try {
        processMessageLoop(jniCallMessage);
    } catch (JniException& jniException) {
        std::cerr << "[Client] Catch an JNI exception: " << jniException.what() << endl;
        jniException.print();
    } catch (const std::exception& exception) {
        std::cerr << "[Client] Catch an exception: " << exception.what() << endl;
    }
    std::cerr << "[Client] leave loop" << endl;
}

void JniClient::processMessageLoop(const JniCallMessage* jniCallMessage) {
    JniMessageFactory jniMessageFactory;
    // TODO: fix the bad receivedBuffer[4096].
    char receiveBuffer[4096];
    ssize_t len;
    len = read(fromServerFd, receiveBuffer, sizeof(receiveBuffer));

    while (len > 0 && len < (int) sizeof(receiveBuffer)) {
        receiveBuffer[len] = 0;

        std::cerr << "[Client] Received:'" << receiveBuffer;
        const JSON json = JSON::parse(receiveBuffer);
        std::cerr << "[Client] Create message"<< endl;
        auto message = jniMessageFactory.createMessageFromJson(json);
        std::cerr << "[Client] Process message"<< endl;
        processMessage(message.get());
        std::cerr << "[Client] Process message done"<< endl;

        if(processDone) {
            break;
        }

        len = read(fromServerFd, receiveBuffer, sizeof(receiveBuffer) - 1);
    }
}

void JniClient::processMessage(JniMessage* message) {
    if (auto replyMessage = dynamic_cast<JniReplyMessage*>(message)) {
        processReplyMessage(replyMessage);
        processDone = true;
    } else if (auto jniEnvCallMessage = dynamic_cast<JniEnvCallMessage*>(message)) {
        cerr << "[Client] processJniEnvCallMessage"<< endl;
        processJniEnvCallMessage(jniEnvCallMessage);
    } else {
        throw JniException("JniClient doesn't know how to handle message of:" + message->toJsonString());
    }
}

void JniClient::processReplyMessage(JniReplyMessage* message) {
    JniValueFactory valueFactory;
    auto returnValue = valueFactory.buildFromJson(
            JSON::parse(message->getReturnValue()->getJson().dump()));

    jclass clazz = jniEnv->FindClass("com/valjni/caller/JniClientImplementation");
    jmethodID methodId = jniEnv->GetMethodID(clazz, "updateJniReturn", "(Ljava/lang/String;Ljava/lang/String;)V");

    jstring signature = jniEnv->NewStringUTF(returnValue->getSignature().c_str());
    jstring value = jniEnv->NewStringUTF(returnValue->getAsString().c_str());
    jniEnv->CallVoidMethod(jniClientImplementation, methodId, signature, value);
}

void JniClient::processJniEnvCallMessage(JniEnvCallMessage *message) {
    auto returnValue = jniEnvService->invoke(message->getJniEnvCall());
    cerr << "The return value signature:" << returnValue->getSignature() << endl;
    cerr << "The return value:" << returnValue->getAsString() << endl;

    JniEnvReplyMessage replyMessage(returnValue.get());
    cerr << "the reply message generated" << endl;

    const string replyMessageJsonString = replyMessage.toJsonString() + "\n";
    write(toServerFd, replyMessageJsonString.c_str(), replyMessageJsonString.size());
}

unique_ptr<JniCallMessage> JniClient::createJniCallMessge(string sharedObjectPath, string methodName) {
    JniCall* jniCall = new JniCall();
    JniArgumentRepository& repository = JniArgumentRepository::getInstance();

    BasicJniValue returnResult = repository.popReturnResult();
    jniCall->setReturnValue(&returnResult);

    while (!repository.isEmpty()) {
        BasicJniValue b = repository.popArgument(jniEnv);
        jniCall->pushArgument(&b);
    }
    jniCall->setSharedObjectPath(sharedObjectPath);
    jniCall->setMethodName(methodName);

    return make_unique<JniCallMessage>(jniCall);
}

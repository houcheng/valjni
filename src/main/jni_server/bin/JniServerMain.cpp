#include <JniProcessor.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include "backward.hpp"

using namespace std;

void callJniProcessor() {
    JniEnvProxy proxy;
    JniInterfaceWrapper wrapper(&proxy);
    JniProcessor processor(wrapper.getJNIEnv());
    proxy.setJniProccessor(&processor);
    std::cerr << "[Server started]" << endl;
    for (std::string line; std::getline(std::cin, line);) {
        std::cerr << "[Server] received:" << line << endl;
        processor.process(line);
        if (processor.getIsEnd()) {
            break;
        }
    }
}

void tryCallJniProcessor() {
    try {
        callJniProcessor();
    } catch (JniException& jniException) {
        std::cerr << "[Server] Catch an JNI exception: " << jniException.what() << endl;
        jniException.print();
    } catch (const std::exception& exception) {
        std::cerr << "[Server] Catch an exception: " << exception.what() << endl;
    }
}

int main(int argc, char **argv) {
    tryCallJniProcessor();
}


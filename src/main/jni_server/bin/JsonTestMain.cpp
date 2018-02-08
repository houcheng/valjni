#include <JniProcessor.h>
#include <string>
#include <iostream>
#include <stdlib.h>
using namespace std;

#include "JniEnvProxy.h"
#include "JniInterfaceWrapper.h"

int main(int argc, char** argv) {
    JniEnvProxy proxy;
    JniInterfaceWrapper wrapper(&proxy);
    JNIEnv* env = wrapper.getJNIEnv();
    env->FindClass("java/lang/String");
    return 0;
}

int main2(int argc, char **argv) {
    std::string line;
    std::getline(std::cin, line);
    JSON json = JSON::parse(line);
    printf("Got done!\n");
    return 0;
}

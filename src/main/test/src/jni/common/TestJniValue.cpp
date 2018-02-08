#define CATCH_CONFIG_MAIN

#include <JniValueFactory.h>
#include "catch.hpp"
#include "fakeit.hpp"
#include "JniValue.h"

TEST_CASE("BasicJniValue as an integer.") {
    JSON jsonValue = {
            {"signature", "I"},
            {"value", "65535"},
    };

    JniValueFactory factory;
    auto jniValue = factory.buildFromJson(jsonValue);

    SECTION("Test getJson and factory feature") {
        REQUIRE(jniValue->getJson() == jsonValue);

        auto basicJniValue = dynamic_cast<BasicJniValue*>(jniValue.get());
        CHECK(basicJniValue != nullptr);
    }

    SECTION("Test basic features") {
        REQUIRE((jniValue->getAsRaw() & 0xffffffff) == 65535);
        REQUIRE(jniValue->getAsString() == "65535");
        REQUIRE(jniValue->getType() == JniValueType::JNI_VALUE_INT);

        auto basicJniValue = dynamic_cast<BasicJniValue*>(jniValue.get());
        REQUIRE(basicJniValue->getType() == JniValueType::JNI_VALUE_INT);
        REQUIRE_FALSE(basicJniValue->isRealNumber());
        REQUIRE(basicJniValue->getJson() == jsonValue);

    }

    SECTION("Test changing value") {
        BasicJniValue* basicJniValue = dynamic_cast<BasicJniValue*>(jniValue.get());
        basicJniValue->setByString("123456");
        REQUIRE(basicJniValue->getAsString() == "123456");
        REQUIRE(basicJniValue->getJson() != jsonValue);
    }
}

TEST_CASE("BasicJniValue as an float.") {
    JSON jsonValue = {
            {"signature", "F"},
            {"value", "3.800000"},
    };

    JniValueFactory factory;
    auto jniValue = factory.buildFromJson(jsonValue);

    SECTION("Test getJson and factory feature") {
        REQUIRE(jniValue->getJson() == jsonValue);

        BasicJniValue* basicJniValue = dynamic_cast<BasicJniValue*>(jniValue.get());
        CHECK(basicJniValue != nullptr);
    }

    SECTION("Test basic features") {
        REQUIRE(jniValue->getAsString() == "3.800000");
        REQUIRE(jniValue->getType() == JniValueType::JNI_VALUE_FLOAT);

        BasicJniValue* basicJniValue = dynamic_cast<BasicJniValue*>(jniValue.get());
        REQUIRE(basicJniValue->isRealNumber());
        REQUIRE(basicJniValue->getAsFloat() == Approx(3.8f));
    }

    SECTION("Test changing value") {
        BasicJniValue* basicJniValue = dynamic_cast<BasicJniValue*>(jniValue.get());
        basicJniValue->setByString("9.2");
        REQUIRE(basicJniValue->getJson() != jsonValue);
        REQUIRE(basicJniValue->getAsFloat() == Approx(9.2f));
    }
}

TEST_CASE("BasicJniValue as an jarray") {
    JSON jsonValue = {
            {"signature", "JARRAY"},
            {"value", "0x7f0566bc3b98"},
    };
    JniValueFactory factory;
    auto jniValue = factory.buildFromJson(jsonValue);

    SECTION("Test getJson and factory feature") {
        REQUIRE(jniValue->getJson() == jsonValue);

        BasicJniValue* basicJniValue = dynamic_cast<BasicJniValue*>(jniValue.get());
        CHECK(basicJniValue != nullptr);
    }

    SECTION("Test basic features") {
        REQUIRE(jniValue->getAsRaw() == 0x7f0566bc3b98);
        REQUIRE(jniValue->getAsString() == "0x7f0566bc3b98");
        REQUIRE(jniValue->getType() == JniValueType::JNI_VALUE_JARRAY);

        BasicJniValue* basicJniValue = dynamic_cast<BasicJniValue*>(jniValue.get());
        REQUIRE_FALSE(basicJniValue->isRealNumber());
    }
}

TEST_CASE("BasicJniValue as an jobject") {
    JSON jsonValue = {
            {"signature", "JOBJECT"},
            {"value", "0xe0eabcd"},
    };
    JniValueFactory factory;
    auto jniValue = factory.buildFromJson(jsonValue);

    SECTION("Test getJson and factory feature") {
        REQUIRE(jniValue->getJson() == jsonValue);

        BasicJniValue* basicJniValue = dynamic_cast<BasicJniValue*>(jniValue.get());
        CHECK(basicJniValue != nullptr);
    }

    SECTION("Test basic features") {
        REQUIRE((jniValue->getAsRaw() & 0xffffffff) == 0x0e0eabcd);
        REQUIRE(jniValue->getAsString() == "0xe0eabcd");
        REQUIRE(jniValue->getType() == JniValueType::JNI_VALUE_JOBJECT);

        BasicJniValue* basicJniValue = dynamic_cast<BasicJniValue*>(jniValue.get());
        REQUIRE_FALSE(basicJniValue->isRealNumber());
    }
}


TEST_CASE("CStringJniEnvValue") {
    JSON jsonValue = {
            {"signature", "CSTRING"},
            {"value", "Hello world"},
    };
    SECTION("Test basic features") {
        CStringJniEnvValue jniEnvValue("Hello world");
        REQUIRE(jniEnvValue.getSignature() == "CSTRING");
        REQUIRE(jniEnvValue.getAsString() == "Hello world");
        REQUIRE(jniEnvValue.getJson() == jsonValue);

        jniEnvValue.setByString("Hello world 2");
        REQUIRE(jniEnvValue.getAsString() == "Hello world 2");
    }

    SECTION("Test getJson and factory feature") {
        JniValueFactory factory;
        auto jniValue = factory.buildFromJson(jsonValue);
        REQUIRE(jniValue->getJson() == jsonValue);

        CStringJniEnvValue* cstringJniEnvValue = dynamic_cast<CStringJniEnvValue*>(jniValue.get());
        CHECK(cstringJniEnvValue != nullptr);
    }
}

TEST_CASE("Test basic and factory feature for C pointer shared memory jni value.") {
    JSON sharedMemoryJson = {
            { "address", "0xabcd000" },
            { "shmId", "1123" },
            { "size", 1024 },
            { "remoteAddressForRef", "0x7788" },
    };
    JSON jsonValue = {
            {"signature", "*"},
            {"sharedMemory", sharedMemoryJson},
    };

    SharedMemory sharedMemory(sharedMemoryJson);
    CPointerSharedMemoryJniEnvValue jniEnvValue(sharedMemory);
    REQUIRE(jniEnvValue.getSignature() == "*");
    REQUIRE(jniEnvValue.getSharedMemory().getShmId() == "1123");
    REQUIRE(jniEnvValue.getSharedMemory().getSize() == 1024);
    REQUIRE(jniEnvValue.getJson() == jsonValue);
}

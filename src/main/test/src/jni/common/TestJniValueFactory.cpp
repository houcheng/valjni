#include "catch.hpp"
#include "fakeit.hpp"
#include "JniValue.h"
#include "JniValueFactory.h"


TEST_CASE("Test creating cstring jni value with raw value should raise an exception") {
    JniValueFactory factory;
    JniRawValue rawValue = 1234;
    REQUIRE_THROWS(factory.build("CSTRING", rawValue));
    REQUIRE_THROWS(factory.build("*", rawValue));
}

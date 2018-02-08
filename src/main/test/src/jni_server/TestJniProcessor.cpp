#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "fakeit.hpp"

TEST_CASE("Test1", "[unit]") {
    int a = 1;
    int b = 1;
    REQUIRE(a == b);
}


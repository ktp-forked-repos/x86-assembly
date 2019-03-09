#define CATCH_CONFIG_MAIN
#include "catch.hpp"

extern "C" const char *hello();

TEST_CASE("Say Hi!") {
    CHECK_THAT(hello(), Catch::Equals("Hello, World!"));
}

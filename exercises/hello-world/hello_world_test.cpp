#include "catch.hpp"

extern "C" const char *hello();

TEST_CASE("Say Hi!", "[integration]") {
    CHECK_THAT(hello(), Catch::Equals("Hello, World!"));
}

#include <catch2/catch_all.hpp>

TEST_CASE("BASE CASE 2")
{
    int a = 3;
    int b = 2;

    REQUIRE(a + b == 5);
    REQUIRE(a - b == 1);
    REQUIRE(a * b == 6);
    REQUIRE(a / b == 2); //! make it fail lol
}

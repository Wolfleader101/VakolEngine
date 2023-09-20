// #include "../include/Vakol/core.hpp"

// #include "Application/Application.hpp"
// #include <iostream>

// #include <catch2/catch_all.hpp>

// int main()
// {

//     return 0;
// }

#include <catch2/catch_all.hpp>

TEST_CASE("BASE CASE")
{
    int a = 3;
    int b = 2;

    REQUIRE(a + b == 5);
    REQUIRE(a - b == 1);
    REQUIRE(a * b == 6);
    REQUIRE(a / b == 1);
}

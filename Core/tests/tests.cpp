#include <Logger/Logger.hpp>
#include <catch2/catch_all.hpp>

TEST_CASE("SETUP", "[SETUP]")
{
    Vakol::Logger::Init(); // need this or dies
}

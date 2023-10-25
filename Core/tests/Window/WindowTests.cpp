#include "Window/Window.hpp"

#include <catch2/catch_all.hpp>

using namespace Vakol;

Window window("Test Window", 1, 1);
TEST_CASE("Window - Initialization", "[Window]")
{
    SECTION("Window creation with given parameters")
    {

        REQUIRE(window.GetWidth() == 1);
        REQUIRE(window.GetHeight() == 1);
        REQUIRE(window.GetWindow() != nullptr);
    }
}

TEST_CASE("Window - Event Callback", "[Window]")
{

    bool callbackInvoked = false;

    window.SetEventCallback([&](Event& e) { callbackInvoked = true; });

    // For this simplified test, we're just checking if the callback can be set.
    REQUIRE(callbackInvoked == false); // Ensure the callback hasn't been invoked yet.
}

TEST_CASE("Window - OnUpdate", "[Window]")
{
    REQUIRE_NOTHROW(window.OnUpdate());
}

TEST_CASE("Window - GetWindow", "[Window]")
{
    REQUIRE(window.GetWindow() != nullptr);
}
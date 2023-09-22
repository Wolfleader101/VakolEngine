#include <catch2/catch_all.hpp>

#include <GUI/GUIWindow.hpp>

TEST_CASE("GUI Window class tests", "[GUIWindow]")
{
    Vakol::GUIWindow guiWindow1;

    SECTION("DisplayWindowWidth returns correct width")
    {
        auto window1 = std::make_shared<Vakol::Window>("Test Window 1", 800, 600);
        guiWindow1.Init(window1);

        guiWindow1.CreateNewFrame(); // Create a new frame

        guiWindow1.StartWindowCreation("Test Window 1", true, true, 800, 600, 0, 0); // Create a window

        float width = guiWindow1.DisplayWindowWidth(); // Get the width of the window

        guiWindow1.EndWindowCreation(); // End the window creation

        REQUIRE(width == 800); // Check the width is correct
    }

    SECTION("DisplayWindowHeight returns correct height")
    {
        auto window1 = std::make_shared<Vakol::Window>("Test Window 1", 800, 600);
        guiWindow1.Init(window1);

        guiWindow1.CreateNewFrame(); // Create a new frame

        guiWindow1.StartWindowCreation("Test Window 2", true, true, 800, 600, 0, 0); // Create a window

        float height = guiWindow1.DisplayWindowHeight(); // Get the height of the window

        guiWindow1.EndWindowCreation(); // End the window creation

        REQUIRE(height == 600); // Check the height is correct
    }
}

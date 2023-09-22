#include <cmath> // for std::fabs

#include <catch2/catch_all.hpp>

#include <GUI/GUIWindow.hpp>

TEST_CASE("GUI Window class tests", "[GUIWindow]")
{
    Vakol::GUIWindow guiWindow1;

    SECTION("DisplayWindowWidth returns correct width")
    {
        std::shared_ptr<Vakol::Window> window1 =
            std::make_shared<Vakol::Window>("DisplayWindowWidth Test", 800,
                                            600); // Create a window for the GUI to be displayed in

        guiWindow1.Init(window1); // Initialise the window

        guiWindow1.CreateNewFrame(); // Create a new frame

        guiWindow1.StartWindowCreation("Test Window 1", true, true, 800, 600, 0, 0); // Create a window

        float width = guiWindow1.DisplayWindowWidth(); // Get the width of the window

        guiWindow1.EndWindowCreation(); // End the window creation

        REQUIRE(width == 800); // Check the width is correct
    }

    SECTION("DisplayWindowHeight returns correct height")
    {
        std::shared_ptr<Vakol::Window> window1 =
            std::make_shared<Vakol::Window>("DisplayWindowHeight Test", 800,
                                            600); // Create a window for the GUI to be displayed in

        guiWindow1.Init(window1); // Initialise the window

        guiWindow1.CreateNewFrame(); // Create a new frame

        guiWindow1.StartWindowCreation("Test Window 2", true, true, 800, 600, 0, 0); // Create a window

        float height = guiWindow1.DisplayWindowHeight(); // Get the height of the window

        guiWindow1.EndWindowCreation(); // End the window creation

        REQUIRE(height == 600); // Check the height is correct
    }

    SECTION("Check if most recent window is created")
    {
        std::shared_ptr<Vakol::Window> window1 =
            std::make_shared<Vakol::Window>("ImGui Window Creation Test", 800,
                                            600); // Create a window for the GUI to be displayed in

        guiWindow1.Init(window1); // Initialise the window

        guiWindow1.CreateNewFrame(); // Create a new frame

        guiWindow1.StartWindowCreation("Test Window 3", true, true, 800, 600, 0, 0); // Create a window

        guiWindow1.EndWindowCreation(); // End the window creation

        REQUIRE(guiWindow1.IsWindowCreated() == true); // Check the window is created
    }

    SECTION("Check if the font default has been changed")
    {
        std::shared_ptr<Vakol::Window> window1 =
            std::make_shared<Vakol::Window>("Default Font Success Test", 800,
                                            600); // Create a window for the GUI to be displayed in

        guiWindow1.Init(window1); // Initialise the window

        bool fontChanged =
            guiWindow1.ChangeFontDefault("coreAssets/fonts/GidoleFont/Gidole-Regular.ttf"); // Change the font

        guiWindow1.CreateNewFrame(); // Create a new frame

        guiWindow1.StartWindowCreation("Test Window 4", true, true, 800, 600, 0, 0); // Create a window

        guiWindow1.EndWindowCreation(); // End the window creation

        REQUIRE(fontChanged == true); // Check that the font default change was successful
    }

    SECTION("Check if the font default change has failed")
    {
        std::shared_ptr<Vakol::Window> window1 =
            std::make_shared<Vakol::Window>("Default Font Failed Test Window", 800,
                                            600); // Create a window for the GUI to be displayed in

        guiWindow1.Init(window1); // Initialise the window

        bool fontChanged = guiWindow1.ChangeFontDefault(
            "coreAssets/fonts/TestDirectory/FakeFont.ttf"); // Change the font (Intentionally fake directory)

        guiWindow1.CreateNewFrame(); // Create a new frame

        guiWindow1.StartWindowCreation("Test Window 5", true, true, 800, 600, 0, 0); // Create a window

        guiWindow1.EndWindowCreation(); // End the window creation

        REQUIRE(fontChanged == false); // Check that the font default change failed
    }

    SECTION("Check if window rounding changes")
    {
        std::shared_ptr<Vakol::Window> window1 = std::make_shared<Vakol::Window>(
            "Window Rounding Test", 800, 600); // Create a window for the GUI to be displayed in

        guiWindow1.Init(window1); // Initialise the window

        guiWindow1.WindowRoundingStyle(5.5f); // Change the window rounding style to 5.5f

        guiWindow1.CreateNewFrame();                                                 // Create a new frame
        guiWindow1.StartWindowCreation("Test Window 6", true, true, 800, 600, 0, 0); // Create a window

        guiWindow1.EndWindowCreation(); // End the window creation

        float newRounding = guiWindow1.GetStyle()->WindowRounding; // Get the new window rounding value

        REQUIRE(std::fabs(newRounding - 5.5f) < 0.01f); // Check that the window rounding was successfully changed
    }
}

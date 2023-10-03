#include <cmath> // for std::fabs

#include <catch2/catch_all.hpp>

#include <GUI/GUIWindow.hpp>

TEST_CASE("GUI Window class tests", "[GUIWindow]")
{
    Vakol::GUIWindow guiWindow1;

    SECTION("DisplayWindowWidth and DisplayWindowHeight returns correct width") // Check the display window width/height
    {
        float width = 0.0f, height = 0.0f;

        std::shared_ptr<Vakol::Window> window1 =
            std::make_shared<Vakol::Window>("DisplayWindowWidth and DisplayWindowHeight Test", 1.0f,
                                            1.0f); // Create a window for the GUI to be displayed in

        guiWindow1.Init(window1); // Initialise the window

        guiWindow1.CreateNewFrame(); // Create a new frame

        guiWindow1.StartWindowCreation("Test Window 1", true, true, 1.0f, 1.0f, 0.0f, 0.0f); // Create a window

        width = guiWindow1.DisplayWindowWidth();   // Get the width of the window
        height = guiWindow1.DisplayWindowHeight(); // Get the height of the window

        guiWindow1.EndWindowCreation(); // End the window creation

        REQUIRE(width == window1->GetWidth());   // Check the display width is correct
        REQUIRE(height == window1->GetHeight()); // Check the display height is correct
    }

    SECTION("Check if most recent window is created")
    {
        std::shared_ptr<Vakol::Window> window1 =
            std::make_shared<Vakol::Window>("ImGui Window Creation Test", 1.0f,
                                            1.0f); // Create a window for the GUI to be displayed in

        guiWindow1.Init(window1); // Initialise the window

        guiWindow1.CreateNewFrame(); // Create a new frame

        guiWindow1.StartWindowCreation("Test Window 2", true, true, 1.0f, 1.0f, 0.0f, 0.0f); // Create a window

        guiWindow1.EndWindowCreation(); // End the window creation

        REQUIRE(guiWindow1.IsWindowCreated() == true); // Check the window is created
    }

    SECTION("Check if the font default has been changed")
    {
        std::shared_ptr<Vakol::Window> window1 =
            std::make_shared<Vakol::Window>("Default Font Success Test", 1.0f,
                                            1.0f); // Create a window for the GUI to be displayed in

        guiWindow1.Init(window1); // Initialise the window

        bool fontChanged =
            guiWindow1.ChangeFontDefault("coreAssets/fonts/GidoleFont/Gidole-Regular.ttf"); // Change the font

        guiWindow1.CreateNewFrame(); // Create a new frame

        guiWindow1.StartWindowCreation("Test Window 3", true, true, 1.0f, 1.0f, 0.0f, 0.0f); // Create a window

        guiWindow1.EndWindowCreation(); // End the window creation

        REQUIRE(fontChanged == true); // Check that the font default change was successful
    }

    SECTION("Check if the font default change has failed")
    {
        std::shared_ptr<Vakol::Window> window1 =
            std::make_shared<Vakol::Window>("Default Font Failed Test Window", 1.0f,
                                            1.0f); // Create a window for the GUI to be displayed in

        guiWindow1.Init(window1); // Initialise the window

        bool fontChanged = guiWindow1.ChangeFontDefault(
            "coreAssets/fonts/TestDirectory/FakeFont.ttf"); // Change the font (Intentionally fake directory)

        guiWindow1.CreateNewFrame(); // Create a new frame

        guiWindow1.StartWindowCreation("Test Window 4", true, true, 1.0f, 1.0f, 0.0f, 0.0f); // Create a window

        guiWindow1.EndWindowCreation(); // End the window creation

        REQUIRE(fontChanged == false); // Check that the font default change failed
    }

    SECTION("Check if window style changes")
    {
        std::shared_ptr<Vakol::Window> window1 = std::make_shared<Vakol::Window>(
            "Window Style Test", 1.0f, 1.0f); // Create a window for the GUI to be displayed in

        guiWindow1.Init(window1); // Initialise the window

        guiWindow1.WindowRoundingStyle(5.5f);                     // Change the window rounding style to 5.5f
        guiWindow1.WindowBackgroundStyle(1.0f, 0.0f, 0.0f, 1.0f); // Change the window background style to red

        guiWindow1.CreateNewFrame();                                                         // Create a new frame
        guiWindow1.StartWindowCreation("Test Window 5", true, true, 1.0f, 1.0f, 0.0f, 0.0f); // Create a window

        guiWindow1.EndWindowCreation(); // End the window creation

        float newRounding = guiWindow1.GetStyle()->WindowRounding; // Get the new window rounding value

        ImVec4 col = guiWindow1.GetStyle()->Colors[ImGuiCol_WindowBg]; // Get the new window background colour

        REQUIRE(std::fabs(newRounding - 5.5f) < 0.01f); // Check that the window rounding was successfully changed

        // Check that the window background colour was successfully changed
        REQUIRE(col.x == 1.0f);
        REQUIRE(col.y == 0.0f);
        REQUIRE(col.z == 0.0f);
        REQUIRE(col.w == 1.0f);
    }

    SECTION("GetFramesPerSecond returns a valid value")
    {
        std::shared_ptr<Vakol::Window> window1 = std::make_shared<Vakol::Window>(
            "FPS Validity Test", 1.0f, 1.0f); // Create a window for the GUI to be displayed in

        guiWindow1.Init(window1); // Initialise the window

        guiWindow1.CreateNewFrame();                                                         // Create a new frame
        guiWindow1.StartWindowCreation("Test Window 6", true, true, 1.0f, 1.0f, 0.0f, 0.0f); // Create a window

        guiWindow1.EndWindowCreation(); // End the window creation

        float fps = guiWindow1.GetFramesPerSecond(); // Get the FPS

        REQUIRE(fps > 0.0f); // Check that the FPS is valid (i.e. greater than 0)
    }
}

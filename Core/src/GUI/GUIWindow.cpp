#include "GUI/GUIWindow.hpp"
#include "Window/Window.hpp"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <Logger/Logger.hpp>

namespace Vakol
{
    GUIWindow::GUIWindow()
    {
        IMGUI_CHECKVERSION(); // Checks the version of IMGUI
    };

    bool GUIWindow::ChangeFontDefault(std::string inputPath) const
    {
        std::ifstream infile(inputPath); // Check if the file exists

        if (!infile.good()) // If the file does not exist, use the default font
        {
            VK_WARN("Font file not found at '" + inputPath + "'. Using default font.");

            return false; // Returns false if the font was not successfully changed
        }

        ImFontConfig font_cfg; // Configures the font

        font_cfg.OversampleH = 8;
        font_cfg.OversampleV = 8;

        ImGui::GetIO().FontDefault =
            ImGui::GetIO().Fonts->AddFontFromFileTTF(inputPath.c_str(), 16.0f, &font_cfg); // Adds the font to the UI

        return true; // Returns true if the font was successfully changed
    }

    void GUIWindow::Init(const std::shared_ptr<Window>& window)
    {
        m_context = ImGui::CreateContext(); // Creates a new context (Window)

        if (!m_context)
        {
            VK_ERROR("Failed to create GUI context!");
            return;
        }

        SetAsContext(); // Sets the current context (Window)

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-viewport

        ImGui::StyleColorsDark(); // Chooses the Dark style

        ImGui_ImplGlfw_InitForOpenGL(window->GetWindow(), true); // Takes in the GLFW Window
        ImGui_ImplOpenGL3_Init("#version 460");                  // Sets the version of GLSL being used

        m_style = &ImGui::GetStyle(); // Gets the current style of the ImGui window

        // Sets the styling options for the ImGui window
        m_style->Colors[ImGuiCol_WindowBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
        m_style->Colors[ImGuiCol_Border] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
        m_style->Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
        m_style->Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
        m_style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
        m_style->Colors[ImGuiCol_FrameBgActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
        m_style->Colors[ImGuiCol_TitleBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
        m_style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
        m_style->Colors[ImGuiCol_TitleBgActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
        m_style->Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
        m_style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
        m_style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);

        ChangeFontDefault("coreAssets/fonts/GidoleFont/Gidole-Regular.ttf");

        m_window = window; // Sets the window to the given window
    }

    void GUIWindow::CreateNewFrame() const
    {
        SetAsContext();
        ImGui_ImplOpenGL3_NewFrame(); // Sets up the new frame to be used within OpenGL
        ImGui_ImplGlfw_NewFrame();    // Sets up the new frame to be used within GLFW
        ImGui::NewFrame();            // Creates a new frame
    }

    void GUIWindow::EndFrame() const
    {
        ImGui::Render();                                        // Renders the ImGui frame
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // Renders the ImGui draw data
        ImGui::EndFrame();
    }

    float GUIWindow::DisplayWindowWidth() const
    {
        return ((float)m_window->GetWidth());
    }

    float GUIWindow::DisplayWindowHeight() const
    {
        return ((float)m_window->GetHeight());
    }

    float GUIWindow::GUIWindowWidth() const
    {
        return (ImGui::GetWindowWidth());
    }

    float GUIWindow::GUIWindowHeight(bool addTitleBarHeight) const
    {
        if (addTitleBarHeight) // If the title bar height is to be added
        {
            return (ImGui::GetWindowHeight() + ImGui::GetFrameHeight()); // Add the title bar height
        }
        else
            return (ImGui::GetWindowHeight()); // Return the window height without the title bar height
    }

    void GUIWindow::StartWindowCreation(const std::string& windowName, bool centerX, bool centerY, float width,
                                        float height, const float xOffset, float yOffset) const
    {
        if (height < 32.0f)
        {
            VK_WARN("Height of GUI window '" + windowName + "' is less than 32.0f. Setting height to 32.0f.");

            height = 32.0f;
        }

        if (width < 32.0f)
        {
            VK_WARN("Width of GUI window '" + windowName + "' is less than 32.0f. Setting height to 32.0f.");

            width = 32.0f;
        }

        ImGuiViewport* main_viewport = ImGui::GetMainViewport();

        // We will calculate the final position first
        ImVec2 finalPosition = {main_viewport->Pos.x + xOffset, main_viewport->Pos.y + yOffset};

        // Adjust finalPosition based on centering flags
        if (centerX && centerY)
        {
            finalPosition.x = main_viewport->Pos.x + (DisplayWindowWidth() - width) / 2 + xOffset;
            finalPosition.y = main_viewport->Pos.y + (DisplayWindowHeight() - height) / 2 + yOffset;
        }
        else if (centerX)
        {
            finalPosition.x = main_viewport->Pos.x + (DisplayWindowWidth() - width) / 2 + xOffset;
        }
        else if (centerY)
        {
            finalPosition.y = main_viewport->Pos.y + (DisplayWindowHeight() - height) / 2 + yOffset;
        }

        // Now set the final position
        ImGui::SetNextWindowPos(finalPosition);
        ImGui::SetNextWindowSize({width, height}, ImGuiCond_Always);

        // Begin the window
        ImGui::Begin(windowName.c_str(), nullptr,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoDocking);
    }

    bool GUIWindow::IsWindowCreated() const
    {
        return ImGui::IsWindowAppearing();
    }

    float GUIWindow::GetFramesPerSecond() const
    {
        return ImGui::GetIO().Framerate;
    };

    ImGuiStyle* GUIWindow::GetStyle() const
    {
        return m_style;
    }

    void GUIWindow::Update() const
    {
        ImGui::Render(); // Renders the UI

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // Renders the UI to the screen

        // Handle viewports
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void GUIWindow::AddText(const std::string& inputText, const bool centerX, const bool centerY, const float fontSize,
                            const float inputRed, const float inputGreen, const float inputBlue,
                            const float inputAlpha) const
    {
        ImGui::SetWindowFontScale(fontSize); // Sets the font size of the text

        if (centerX && centerY)
        {
            const auto windowWidth = ImGui::GetWindowSize().x;
            const auto textWidth = ImGui::CalcTextSize(inputText.c_str()).x;

            const auto windowHeight = ImGui::GetWindowSize().y;
            const auto textHeight = ImGui::CalcTextSize(inputText.c_str()).y;

            ImGui::SetCursorPosX((windowWidth - textWidth) / 2);
            ImGui::SetCursorPosY((windowHeight - textHeight) / 2);
        }
        else if (centerX)
        {
            const auto windowWidth = ImGui::GetWindowSize().x;
            const auto textWidth = ImGui::CalcTextSize(inputText.c_str()).x;

            ImGui::SetCursorPosX((windowWidth - textWidth) / 2);
        }
        else if (centerY)
        {
            const auto windowHeight = ImGui::GetWindowSize().y;
            const auto textHeight = ImGui::CalcTextSize(inputText.c_str()).y;

            ImGui::SetCursorPosY((windowHeight - textHeight) / 2);
        }

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(inputRed, inputGreen, inputBlue, inputAlpha));

        ImGui::Text("%s", inputText.c_str());

        ImGui::PopStyleColor();
    }

    void GUIWindow::AddImage(const unsigned id, const ImVec2& imageSize, const bool centerX, const bool centerY) const
    {
        if (centerX && centerY)
        {
            const auto width = ImGui::GetWindowSize().x;
            const auto height = ImGui::GetWindowSize().y;

            ImGui::SetCursorPosX((width - imageSize.x) * 0.5f);
            ImGui::SetCursorPosY((height - imageSize.y) * 0.5f);
        }
        else if (centerX)
        {
            const auto width = ImGui::GetWindowSize().x;
            ImGui::SetCursorPosX((width - imageSize.x) * 0.5f);
        }
        else if (centerY)
        {
            const auto height = ImGui::GetWindowSize().y;
            ImGui::SetCursorPosY((height - imageSize.y) * 0.5f);
        }

        ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(id)), imageSize);
    }

    void GUIWindow::AddButton(const std::string& buttonName, const bool centerX, const bool centerY, float width,
                              float height, const std::function<void()>& inputFunction) const
    {
        float alignment = centerX ? 0.5f : 0.0f;
        float yAlignment = centerY ? 0.5f : 0.0f;
        float avail = ImGui::GetContentRegionAvail().x;
        float off = (avail - width) * alignment;

        if (off > 0.0f)
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
        if (yAlignment > 0.0f)
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (ImGui::GetContentRegionAvail().y - height) * yAlignment);

        if (ImGui::Button(buttonName.c_str(), {width, height}))
            inputFunction(); // Runs the given input function
    };

    void GUIWindow::AddCheckbox(const std::string& checkboxName, bool* checkBoxValue)
    {
        ImGui::Checkbox(checkboxName.c_str(), checkBoxValue);
    };

    void GUIWindow::AddIntSlider(const std::string& sliderName, int* sliderValue, const int minValue,
                                 const int maxValue) const
    {
        ImGui::SliderInt(sliderName.c_str(), sliderValue, minValue, maxValue);
    };

    void GUIWindow::AddVecIntSlider(const std::string& sliderName, int sliderValue[], const int size,
                                    const int minValue, const int maxValue) const
    {
        switch (size)
        {
        case 2:
            ImGui::SliderInt2(sliderName.c_str(), sliderValue, minValue, maxValue);

            break;
        case 3:
            ImGui::SliderInt3(sliderName.c_str(), sliderValue, minValue, maxValue);

            break;
        case 4:
            ImGui::SliderInt4(sliderName.c_str(), sliderValue, minValue, maxValue);

            break;
        default:
            VK_CRITICAL("Outside of the bounds of a Vector 2, 3 or 4. Please enter a correct array!");

            break;
        }
    };

    void GUIWindow::AddFloatSlider(const std::string& sliderName, float* sliderValue, const float minValue,
                                   const float maxValue) const
    {
        ImGui::SliderFloat(sliderName.c_str(), sliderValue, minValue, maxValue);
    };

    void GUIWindow::AddVecFloatSlider(const std::string& sliderName, float sliderValue[], const int size,
                                      const float minValue, const float maxValue) const
    {
        switch (size)
        {
        case 2:
            ImGui::SliderFloat2(sliderName.c_str(), sliderValue, minValue, maxValue);

            break;
        case 3:
            ImGui::SliderFloat3(sliderName.c_str(), sliderValue, minValue, maxValue);

            break;
        case 4:
            ImGui::SliderFloat4(sliderName.c_str(), sliderValue, minValue, maxValue);

            break;
        default:
            VK_CRITICAL("Outside of the bounds of a Vector 2, 3 or 4. Please enter a correct array!");

            break;
        }
    };

    void GUIWindow::SameLine() const
    {
        ImGui::SameLine(); // Adds content to the same line
    }

    void GUIWindow::WindowBackgroundStyle(const float inputRed, const float inputGreen, const float inputBlue,
                                          const float inputAlpha) const
    {
        m_style->Colors[ImGuiCol_WindowBg] =
            ImVec4(inputRed, inputGreen, inputBlue, inputAlpha); // Sets the background colour of the window

        ImGui::GetStyle() = *m_style; // Sets the ImGui Style to the current style
    }

    void GUIWindow::WindowRoundingStyle(const float inputValue) const
    {
        m_style->WindowRounding = inputValue; // Sets the rounding style of the window

        ImGui::GetStyle() = *m_style; // Sets the ImGui Style to the current style
    }

    void GUIWindow::EndWindowCreation() const
    {
        ImGui::End(); // Ends the creation of the window
    };

    GUIWindow::~GUIWindow()
    {
        if (!is_initialised)
            return;

        // Before shutting down the ImGui backends...
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::DestroyPlatformWindows();
        }

        ImGui_ImplOpenGL3_Shutdown();     // Shuts down OpenGL support
        ImGui_ImplGlfw_Shutdown();        // Shuts down GLFW support
        ImGui::DestroyContext(m_context); // Destroys the Window
    };

    void GUIWindow::SetAsContext() const
    {
        ImGui::SetCurrentContext(m_context); // Sets the current context (Window)
    }
} // namespace Vakol

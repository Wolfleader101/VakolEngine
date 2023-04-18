#include "GUIWindow.hpp"

namespace Vakol::View {
    GUIWindow::GUIWindow() {
        IMGUI_CHECKVERSION();  // Checks the version of IMGUI

        windowFlags = 0;
    };

    void GUIWindow::Init(View::Window *m_window) {
        ImGui::CreateContext();

        ImGui::StyleColorsDark();  // Chooses the Dark style

        ImGui_ImplGlfw_InitForOpenGL(m_window->GetWindow(), true);  // Takes in the GLFW Window
        ImGui_ImplOpenGL3_Init("#version 460");                     // Sets the version of GLSL being used

        CreateNewFrame();
    };

    void GUIWindow::CreateNewFrame() {
        ImGui_ImplOpenGL3_NewFrame();  // Sets up the new frame to be used within OpenGL
        ImGui_ImplGlfw_NewFrame();     // Sets up the new frame to be used within GLFW
        ImGui::NewFrame();             // Creates a new frame
    };

    void GUIWindow::StartWindowCreation(std::string& windowName, float width, float height, float xPosition,
                                        float yPosition) {
        ImGui::Begin(windowName.c_str(), 0, windowFlags);  // Begins the creation of the Window

        ImGui::SetWindowPos({xPosition, yPosition}, ImGuiCond_Once);  // Sets the position of the window

        ImGui::SetWindowSize({width, height}, ImGuiCond_Once);  // Sets the size of the window (Width, Height) in pixels
    };

    float GUIWindow::GetFramesPerSecond() { 
        return ImGui::GetIO().Framerate;
    };

    void GUIWindow::Update(std::string scriptName, Controller::LuaState& lua) {
        lua.RunFile("scripts/" + scriptName);

        sol::function update = lua.GetState()["update"];

        update(*this);
    }

    void GUIWindow::AddText(std::string& inputText) {
        ImGui::Text(inputText.c_str());
    };

    void GUIWindow::AddButton(std::string& buttonName, float width, float height, std::function<void()> inputFunction) {
        if (ImGui::Button(buttonName.c_str(), {width, height})) {
            inputFunction();  // Runs the given input function
        }
    };

    void GUIWindow::AddCheckbox(std::string& checkboxName, bool *checkBoxValue) {
        ImGui::Checkbox(checkboxName.c_str(), checkBoxValue);
    };

    void GUIWindow::AddIntSlider(std::string& sliderName, int *sliderValue, int minValue, int maxValue) {
        ImGui::SliderInt(sliderName.c_str(), sliderValue, minValue, maxValue);
    };

    void GUIWindow::AddVecIntSlider(std::string& sliderName, int sliderValue[], unsigned arraySize, int minValue,
                                    int maxValue) {
        switch (arraySize) {
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

    void GUIWindow::AddFloatSlider(std::string& sliderName, float *sliderValue, float minValue, float maxValue) {
        ImGui::SliderFloat(sliderName.c_str(), sliderValue, minValue, maxValue);
    };

    void GUIWindow::AddVecFloatSlider(std::string& sliderName, float sliderValue[], unsigned arraySize, float minValue,
                                      float maxValue) {
        switch (arraySize) {
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

    void GUIWindow::EndWindowCreation() {
        ImGui::End();  // Ends the creation of the window

        ImGui::Render();                                         // Renders the window
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());  // Retreives the data needed for OpenGL
    };

    GUIWindow::~GUIWindow() {
        if (!is_initialised) return;
        ImGui_ImplOpenGL3_Shutdown();  // Shuts down OpenGL support
        ImGui_ImplGlfw_Shutdown();     // Shuts down GLFW support
        ImGui::DestroyContext();       // Destroys the Window
    };
}  // namespace Vakol::View
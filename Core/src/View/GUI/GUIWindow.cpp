#include "GUIWindow.hpp"

#include <Controller/Logger.hpp>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Vakol::View {
    GUIWindow::GUIWindow()
	{
        IMGUI_CHECKVERSION();  // Checks the version of IMGUI

        windowFlags = 0;
    };

    void GUIWindow::Init(const std::shared_ptr<View::Window>& window) {
        ImGui::CreateContext();

        ImGui::StyleColorsDark();  // Chooses the Dark style

        ImGui_ImplGlfw_InitForOpenGL(window->GetWindow(), true);  // Takes in the GLFW Window
        ImGui_ImplOpenGL3_Init("#version 460");                   // Sets the version of GLSL being used
    };

    void GUIWindow::CreateNewFrame() {
        ImGui_ImplOpenGL3_NewFrame();  // Sets up the new frame to be used within OpenGL
        ImGui_ImplGlfw_NewFrame();     // Sets up the new frame to be used within GLFW
        ImGui::NewFrame();             // Creates a new frame
    };

    void GUIWindow::EndFrame()
    {
        ImGui::Render();                           // Renders the ImGui frame
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());  // Renders the ImGui draw data
        ImGui::EndFrame();
    }

    void GUIWindow::StartWindowCreation(const std::string& windowName, const float width, const float height, const float x, float y) const
    {
        ImGui::Begin(windowName.c_str(), nullptr, windowFlags);  // Begins the creation of the Window

        ImGui::SetWindowPos({x, y}, ImGuiCond_Once);  // Sets the position of the window

        ImGui::SetWindowSize({width, height}, ImGuiCond_Once);  // Sets the size of the window (Width, Height) in pixels
    };

    float GUIWindow::GetFramesPerSecond() { return ImGui::GetIO().Framerate; };

    void GUIWindow::Update() {
        ImGui::Render();  // Renders the UI

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());  // Renders the UI to the screen
    }

    void GUIWindow::AddText(const std::string& inputText) { ImGui::Text(inputText.c_str()); };

    void GUIWindow::AddButton(const std::string& buttonName, float width, float height, const std::function<void()>& inputFunction) {
        if (ImGui::Button(buttonName.c_str(), {width, height})) {
            inputFunction();  // Runs the given input function
        }
    };

    void GUIWindow::AddCheckbox(const std::string& checkboxName, bool* checkBoxValue) {
        ImGui::Checkbox(checkboxName.c_str(), checkBoxValue);
    };

    void GUIWindow::AddIntSlider(const std::string& sliderName, int* sliderValue, const int minValue, const int maxValue) {
        ImGui::SliderInt(sliderName.c_str(), sliderValue, minValue, maxValue);
    };

    void GUIWindow::AddVecIntSlider(const std::string& sliderName, int sliderValue[], const int size, const int minValue, const int maxValue)
	{
        switch (size) {
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

    void GUIWindow::AddFloatSlider(const std::string& sliderName, float* sliderValue, const float minValue, const float maxValue) {
        ImGui::SliderFloat(sliderName.c_str(), sliderValue, minValue, maxValue);
    };

    void GUIWindow::AddVecFloatSlider(const std::string& sliderName, float sliderValue[], const int size, const float minValue, const float maxValue) {
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

    void GUIWindow::EndWindowCreation()
	{
        ImGui::End();  // Ends the creation of the window

        ImGui::Render();                                         // Renders the window
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());  // Retreives the data needed for OpenGL
    };

    GUIWindow::~GUIWindow()
	{
        if (!is_initialised) return;

        ImGui_ImplOpenGL3_Shutdown();  // Shuts down OpenGL support
        ImGui_ImplGlfw_Shutdown();     // Shuts down GLFW support
        ImGui::DestroyContext();       // Destroys the Window
    };
}  // namespace Vakol::View
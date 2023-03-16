#include "UI.hpp"

namespace Vakol::View 
{
    UI::UI() 
    {
    };
    
    void UI::InitUI(GLFWwindow* window) 
    { 
        IMGUI_CHECKVERSION(); //Checks the version of IMGUI

        ImGui::CreateContext();

        ImGuiIO& inputOutput = ImGui::GetIO(); (void)inputOutput;         // Sets up the variable which handles inputs and outputs for the ImGui Window
        ImGui::StyleColorsDark(); //Chooses the Dark style

        ImGui_ImplGlfw_InitForOpenGL(window, true); //Takes in the GLFW Window
        ImGui_ImplOpenGL3_Init("#version 460"); //Sets the version of GLSL being used
    };

    void UI::CreateNewFrame() 
    { 
        ImGui_ImplOpenGL3_NewFrame(); //Sets up the new frame to be used within OpenGL
        ImGui_ImplGlfw_NewFrame(); //Sets up the new frame to be used within GLFW
        ImGui::NewFrame(); //Creates a new frame
    };

    void UI::CreateWindow(const char* windowName) 
    { 
        ImGui::Begin(windowName);

        ImGui::SetWindowSize({300.0f, 300.0f});

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    };

    UI::~UI() 
    { 
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    };
}	// namespace Vakol::View
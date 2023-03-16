#include "GUIWindow.hpp"

namespace Vakol::View 
{
    GUIWindow::GUIWindow() 
    {
        IMGUI_CHECKVERSION();  // Checks the version of IMGUI
    };
    
    void GUIWindow::InitUI(GLFWwindow* window) 
    { 
        ImGui::CreateContext();

        ImGui::StyleColorsDark(); //Chooses the Dark style

        ImGui_ImplGlfw_InitForOpenGL(window, true); //Takes in the GLFW Window
        ImGui_ImplOpenGL3_Init("#version 460"); //Sets the version of GLSL being used
    };

    void GUIWindow::CreateNewFrame() 
    { 
        ImGui_ImplOpenGL3_NewFrame(); //Sets up the new frame to be used within OpenGL
        ImGui_ImplGlfw_NewFrame(); //Sets up the new frame to be used within GLFW
        ImGui::NewFrame(); //Creates a new frame
    };

    void GUIWindow::StartWindowCreation(const char* windowName, float width, float height) 
    { 
        ImGui::Begin(windowName); //Begins the creation of the Window

        ImGui::SetWindowSize({width, height});  // Sets the size of the window (Width, Height) in pixels
    };

    void GUIWindow::AddButton(const char* buttonName, float width, float height) 
    { 
        if (ImGui::Button(buttonName, {width, height})) 
        {
            
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
        ImGui_ImplOpenGL3_Shutdown(); //Shuts down OpenGL support
        ImGui_ImplGlfw_Shutdown(); //Shuts down GLFW support
        ImGui::DestroyContext(); //Destroys the Window
    };
}	// namespace Vakol::View
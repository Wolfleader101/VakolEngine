#include "MyGUILayer.hpp"
#include <iostream>

#include <imgui.h>

void MyGUILayer::OnAttach(Vakol::SceneManager* SM)
{
    m_SceneManager = SM;
}

void MyGUILayer::OnDetach()
{
}

void MyGUILayer::OnUpdate()
{
    std::cout << "MyGUILayer::OnUpdate()" << std::endl;

    // Start a new ImGui frame
    ImGui::NewFrame();

    // Begin a new window called "Test Window"
    if (ImGui::Begin("Test Window"))
    {
        // Display some static text
        ImGui::Text("Hello, world!");

        // Create a button
        if (ImGui::Button("Click Me"))
        {
            std::cout << "Button was clicked!" << std::endl;
        }
    }
    ImGui::EndFrame(); // End of "Test Window"
}

void MyGUILayer::OnEvent(Vakol::Event& event)
{
}

void MyGUILayer::OnGUI()
{
}

void MyGUILayer::OnTick()
{
}

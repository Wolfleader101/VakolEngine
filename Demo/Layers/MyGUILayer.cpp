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
}

void MyGUILayer::OnEvent(Vakol::Event& event)
{
}

void MyGUILayer::OnGUI()
{

    // Begin a new window called "Test Window"
    if (ImGui::Begin("Test Window"))
    {
        std::cout << "MyGUILayer::OnGUI()" << std::endl;
        // Display some static text
        ImGui::Text("Hello, world!");

        ImGui::End(); // End of "Test Window"
    }
}

void MyGUILayer::OnTick()
{
}

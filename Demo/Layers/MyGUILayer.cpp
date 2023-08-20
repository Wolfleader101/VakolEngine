#include "MyGUILayer.hpp"
#include <iostream>

#include <imgui.h>

#include <SceneManager/SceneManager.hpp>

void MyGUILayer::OnAttach(Vakol::SceneManager* SM)
{
    m_SceneManager = SM;
}

void MyGUILayer::OnDetach()
{
}

void MyGUILayer::OnUpdate()
{
    const auto& pos = m_SceneManager->GetActiveScene().GetCamera().GetPos();

    std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;

    if (ImGui::Begin("Test Window"))
    {
        const auto& EL = m_SceneManager->GetActiveScene().GetEntityList();
    }
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
        ImGui::Text("Death to Alvaro!");

        ImGui::End(); // End of "Test Window"
    }
}
}

void MyGUILayer::OnTick()
{
}

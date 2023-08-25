#include "MyGUILayer.hpp"
#include <iostream>

#include <ECS/Components.hpp>
#include <SceneManager/SceneManager.hpp>

#include <Window/Events/Event.hpp>
#include <Window/Events/KeyEvent.hpp>

#include <Input/Input.hpp>

#include <Application/Application.hpp>
#include <Utils/Singleton.hpp>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void MyGUILayer::OnAttach(Vakol::SceneManager* SM)
{
    m_SceneManager = SM;
}

// auto window = Vakol::Singleton<Vakol::Application>::GetInstance().GetWindow();
// ImGui_ImplGlfw_InitForOpenGL(window->GetWindow(), true); // Takes in the GLFW Window
// ImGui_ImplOpenGL3_Init("#version 460");
void MyGUILayer::OnDetach()
{
}

void MyGUILayer::OnUpdate()
{
    if (m_Show)
    {

        ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Entities", &m_Show, ImGuiWindowFlags_NoDocking))
        {

            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f)); // Dark background color
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.9f, 0.9f, 1.0f));

            auto& EL = m_SceneManager->GetActiveScene().GetEntityList();

            EL.Iterate<Vakol::Components::Tag, Vakol::Components::Transform>(
                [&](Vakol::Components::Tag& Tag, Vakol::Components::Transform trans) {
                    if (ImGui::CollapsingHeader(Tag.tag.c_str()))
                    {
                        ImGui::DragFloat3("Position", &trans.pos.x, 0.1f);
                        ImGui::DragFloat3("Rotation", &trans.rot.x, 0.1f);
                        ImGui::DragFloat3("Scale", &trans.scale.x, 0.1f);
                    }
                });

            ImGui::PopStyleColor(2);
            ImGui::End();
        }
    }
}

void MyGUILayer::OnEvent(Vakol::Event& event) // toggle editor view
{

    if (event.GetEventType() == Vakol::EventType::KeyPressed)
    {
        auto& keyEvent = dynamic_cast<Vakol::KeyPressedEvent&>(event);
        if (keyEvent.GetKeyCode() == (int)Vakol::Input::KEY::KEY_SEMICOLON)
        {
            m_Show = !m_Show;

            Vakol::Singleton<Vakol::Application>::GetInstance().SetActiveMouse(m_Show);
            event.Handled = true;
            return;
        }

        if (m_Show) // basically disabling any keyboard presses getting to the game when the editor is open
        {
            if (keyEvent.GetKeyCode() == (int)Vakol::Input::KEY::KEY_ESCAPE)
            {
                m_Show = false;
                Vakol::Singleton<Vakol::Application>::GetInstance().SetActiveMouse(m_Show);
            }
            event.Handled = true;
        }
    }

    if (event.GetEventType() == Vakol::EventType::MouseMoved)
    {
        if (m_Show)
        {
            event.Handled = true;
        }
    }
}

void MyGUILayer::OnTick()
{
}

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
    m_Context = ImGui::CreateContext();
    ImGui::SetCurrentContext(m_Context);
    ImGui::StyleColorsDark();

    unsigned width = Vakol::Singleton<Vakol::Application>::GetInstance().GetWidth();
    unsigned height = Vakol::Singleton<Vakol::Application>::GetInstance().GetHeight();
    auto window = Vakol::Singleton<Vakol::Application>::GetInstance().GetWindow();
    ImGui_ImplGlfw_InitForOpenGL(window->GetWindow(), true); // Takes in the GLFW Window
    ImGui_ImplOpenGL3_Init("#version 460");

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(width, height);
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
        ImGui::SetCurrentContext(m_Context);
        ImGui_ImplOpenGL3_NewFrame(); // Sets up the new frame to be used within OpenGL
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        if (ImGui::Begin("Entities"))
        {
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
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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

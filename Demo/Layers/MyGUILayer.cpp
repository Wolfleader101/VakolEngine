#include "MyGUILayer.hpp"
#include <iostream>

#include <imgui.h>

#include <ECS/Components.hpp>
#include <SceneManager/SceneManager.hpp>

#include <Window/Events/Event.hpp>
#include <Window/Events/KeyEvent.hpp>

#include <Input/Input.hpp>

#include <Application/Application.hpp>
#include <Utils/Singleton.hpp>

void MyGUILayer::OnAttach(Vakol::SceneManager* SM)
{
    m_SceneManager = SM;
}

void MyGUILayer::OnDetach()
{
}

void MyGUILayer::OnUpdate()
{
    if (m_Show)
    {
        if (ImGui::Begin("Entities"))
        {
            auto& EL = m_SceneManager->GetActiveScene().GetEntityList();

            EL.Iterate<Vakol::Components::Tag, Vakol::Components::Transform>(
                [&](Vakol::Components::Tag& Tag, Vakol::Components::Transform& trans) {
                    if (ImGui::CollapsingHeader(Tag.tag.c_str()))
                    {
                        ImGui::DragFloat3("Position", &trans.pos.x, 0.1f);
                        ImGui::DragFloat3("Rotation", &trans.rot.x, 0.1f);
                        ImGui::DragFloat3("Scale", &trans.scale.x, 0.1f);
                    }
                });
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

            Vakol::Singleton<Vakol::Application>::GetInstance().SetGameState(m_Show ? Vakol::GameState::Paused
                                                                                    : Vakol::GameState::Running);

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

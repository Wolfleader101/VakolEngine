#include "MyGUILayer.hpp"

#include "AssetLoader/AssetLoader.hpp"

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
        ImGui::SetNextWindowSize(ImVec2(500, 750), ImGuiCond_FirstUseEver);

        ImVec4 dark(0.15f, 0.15f, 0.15f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, dark);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.9f, 0.9f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_TitleBg, dark);
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, dark);

        bool open = ImGui::Begin("Editor", &m_Show);

        if (open) // Check if window is open
        {
            if (ImGui::CollapsingHeader("Systems Control"))
            {

                if (ImGui::Button("Run Scripts"))
                {
                    if (Vakol::Singleton<Vakol::Application>::GetInstance().IsSystemActive(
                            Vakol::SystemFlag::Scripting) == false)
                        Vakol::Singleton<Vakol::Application>::GetInstance().ToggleSystem(Vakol::SystemFlag::Scripting);
                }

                if (ImGui::Button("Pause Scripts"))
                {
                    if (Vakol::Singleton<Vakol::Application>::GetInstance().IsSystemActive(
                            Vakol::SystemFlag::Scripting))
                        Vakol::Singleton<Vakol::Application>::GetInstance().ToggleSystem(Vakol::SystemFlag::Scripting);
                }

                if (ImGui::Button("Run Physics"))
                {
                    if (Vakol::Singleton<Vakol::Application>::GetInstance().IsSystemActive(
                            Vakol::SystemFlag::Physics) == false)
                        Vakol::Singleton<Vakol::Application>::GetInstance().ToggleSystem(Vakol::SystemFlag::Physics);
                }

                if (ImGui::Button("Pause Physics"))
                {
                    if (Vakol::Singleton<Vakol::Application>::GetInstance().IsSystemActive(Vakol::SystemFlag::Physics))
                        Vakol::Singleton<Vakol::Application>::GetInstance().ToggleSystem(Vakol::SystemFlag::Physics);
                }
            }

            ImGui::Separator();

            if (ImGui::CollapsingHeader("Entity List"))
            {

                ImGui::BeginChild("Entity List Child", ImVec2(0, 0), true);

                auto& EL = m_SceneManager->GetActiveScene().GetEntityList();

                EL.Iterate<Vakol::Components::Tag, Vakol::Components::Transform, Vakol::Rendering::Drawable>(
                    [&](const Vakol::Components::Tag& tag, Vakol::Components::Transform& trans,
                        const Vakol::Rendering::Drawable& drawable) {
                        ImGui::PushID((tag.tag + drawable.ID.ConvertToString()).c_str());

                        if (ImGui::CollapsingHeader(tag.tag.c_str()))
                        {
                            ImGui::Text("Drawable ID: %s", drawable.ID.ConvertToString().c_str());

                            ImGui::SeparatorText("Transform");

                            ImGui::DragFloat3("Position", &trans.pos.x, 0.1f);
                            ImGui::DragFloat3("Rotation", &trans.eulerAngles.x, 0.1f);
                            ImGui::DragFloat3("Scale", &trans.scale.x, 0.1f);

                            ImGui::Spacing();

                            ImGui::SeparatorText("Material");

                            Vakol::Rendering::Assets::Model& model = Vakol::AssetLoader::FindModel(drawable.ID);

                            for (Vakol::Rendering::Assets::Mesh& mesh : model.meshes)
                            {
                                ImGui::Text("Mesh ID: %s", mesh.ID.c_str());

                                Vakol::Rendering::Assets::Material& material = mesh.material;

                                ImGui::Text("Material ID: %s", material.ID.c_str());

                                ImGui::Checkbox("Use Lighting?", &material.properties.use_lighting);
                                ImGui::Checkbox("Use Textures?", &material.properties.use_textures);

                                /*ImGui::ColorEdit3(("Diffuse Color##" + material.ID).c_str(),
                                                  &material.properties.diffuse_color.x);*/
                            }
                        }

                        ImGui::PopID();
                    });

                ImGui::EndChild(); // End of child frame
            }
        }
        else
        {
            Vakol::Singleton<Vakol::Application>::GetInstance().SetActiveMouse(m_Show);
        }

        ImGui::End();            // Moved this outside the if-check.
        ImGui::PopStyleColor(4); // Moved this outside the if-check.
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
            if (keyEvent.GetKeyCode() == (int)Vakol::Input::KEY::KEY_ESCAPE)
            {
                m_Show = false;
                Vakol::Singleton<Vakol::Application>::GetInstance().SetActiveMouse(m_Show);
                Vakol::Singleton<Vakol::Application>::GetInstance().SetGameState(Vakol::GameState::Running);
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

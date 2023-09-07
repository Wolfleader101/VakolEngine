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

                EL.IterateEntities([&](auto handle) {
                    Vakol::Entity entity = EL.GetEntity(static_cast<uint32_t>(handle));
                    Vakol::Components::Tag& tag = entity.GetComponent<Vakol::Components::Tag>();
                    Vakol::Components::Transform& trans = entity.GetComponent<Vakol::Components::Transform>();

                    if (ImGui::CollapsingHeader(tag.tag.c_str()))
                    {
                        ImGui::Indent(20.0f);
                        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.9f, 0.4f, 0.f, 1.0f));

                        if (ImGui::CollapsingHeader("Transform"))
                        {

                            ImGui::SeparatorText("Transform");

                            ImGui::DragFloat3("Position", &trans.pos.x, 0.1f);
                            ImGui::DragFloat3("Rotation", &trans.eulerAngles.x, 0.1f);
                            ImGui::DragFloat3("Scale", &trans.scale.x, 0.1f);
                        }

                        if (entity.HasComponent<Vakol::Rendering::Drawable>() && ImGui::CollapsingHeader("Drawable"))
                        {
                            Vakol::Rendering::Drawable& drawable = entity.GetComponent<Vakol::Rendering::Drawable>();

                            ImGui::Text("Drawable ID: %s", drawable.ID.ToString().c_str());
                            ImGui::Spacing();

                            ImGui::SeparatorText("Material");

                            Vakol::Rendering::Assets::Model& model = Vakol::AssetLoader::FindModel(drawable.ID);

                            for (Vakol::Rendering::Assets::Mesh& mesh : model.meshes)
                            {
                                ImGui::Text("Mesh ID: %s", mesh.ID.c_str());

                                Vakol::Rendering::Assets::Material& material = mesh.material;

                                ImGui::Text("Material ID: %s", material.ID.c_str());

                                ImGui::Checkbox("Use Lighting", &material.properties.use_lighting);
                                ImGui::Checkbox("Use Textures", &material.properties.use_textures);

                                ImGui::ColorEdit3(("Diffuse Color" + material.ID).c_str(),
                                                  &material.properties.diffuse_color.x);
                            }
                        }

                        if (ImGui::CollapsingHeader("RigidBody"))
                        {
                            Vakol::RigidBody& rb = entity.GetComponent<Vakol::RigidBody>();
                            const char* preview_label;

                            switch (rb.type)
                            {
                            case Vakol::BodyType::Static:
                                preview_label = "Static";
                                break;
                            case Vakol::BodyType::Kinematic:
                                preview_label = "Kinematic";
                                break;
                            case Vakol::BodyType::Dynamic:
                                preview_label = "Dynamic";
                                break;
                            }

                            if (ImGui::BeginCombo("BodyType", preview_label))
                            {
                                bool is_selected = (rb.type == Vakol::BodyType::Static);
                                if (ImGui::Selectable("Static", is_selected))
                                {
                                    rb.type = Vakol::BodyType::Static;
                                }
                                if (is_selected)
                                {
                                    ImGui::SetItemDefaultFocus();
                                }

                                is_selected = (rb.type == Vakol::BodyType::Kinematic);
                                if (ImGui::Selectable("Kinematic", is_selected))
                                {
                                    rb.type = Vakol::BodyType::Kinematic;
                                }
                                if (is_selected)
                                {
                                    ImGui::SetItemDefaultFocus();
                                }

                                is_selected = (rb.type == Vakol::BodyType::Dynamic);
                                if (ImGui::Selectable("Dynamic", is_selected))
                                {
                                    rb.type = Vakol::BodyType::Dynamic;
                                }
                                if (is_selected)
                                {
                                    ImGui::SetItemDefaultFocus();
                                }

                                ImGui::EndCombo();
                            }

                            ImGui::Checkbox("Has Gravity", &rb.hasGravity);
                            ImGui::DragScalar("Mass", ImGuiDataType_Double, &rb.mass, 0.1f);
                            ImGui::DragScalar("Bounciness", ImGuiDataType_Double, &rb.bounciness, 0.1f);
                            ImGui::DragFloat3("Center of Mass", &rb.centerOfMass.x, 0.1f);

                            ImGui::DragFloat3("Force", &rb.force.x, 0.1f);
                            ImGui::DragFloat3("Torque", &rb.torque.x, 0.1f);
                            ImGui::DragFloat3("Linear Velocity", &rb.linearVelocity.x, 0.1f);
                            ImGui::DragFloat3("Angular Velocity", &rb.angularVelocity.x, 0.1f);

                            if (rb.collisionData)
                            {
                                ImGui::DragFloat3("World Normal", &rb.collisionData->worldNormal.x, 0.1f);
                                ImGui::DragFloat3("World Point", &rb.collisionData->worldPoint.x, 0.1f);
                                ImGui::DragFloat3("Local Point", &rb.collisionData->localPoint.x, 0.1f);
                                ImGui::DragScalar("Penetration Depth", ImGuiDataType_Double,
                                                  &rb.collisionData->penetrationDepth, 0.1f);
                                ImGui::Checkbox("Is Colliding", &rb.collisionData->isColliding);
                            }
                        }

                        ImGui::PopStyleColor();
                        ImGui::Unindent(20.0f);
                    }
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

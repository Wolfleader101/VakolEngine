#include "MyGUILayer.hpp"

#include "AssetLoader/AssetLoader.hpp"
#include "Rendering/RenderEngine.hpp"

#include <ECS/Components.hpp>
#include <SceneManager/SceneManager.hpp>

#include <Window/Events/Event.hpp>
#include <Window/Events/KeyEvent.hpp>

#include <Input/Input.hpp>

#include <Application/Application.hpp>

#include <imgui_impl_glfw.h>
#include <imgui_internal.h>

void MyGUILayer::OnAttach()
{
}

void MyGUILayer::OnDetach()
{
}

void MyGUILayer::OnUpdate()
{
    if (m_Show)
    {
        constexpr int menu_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowSize(ImVec2(500, 750), ImGuiCond_FirstUseEver);

        constexpr ImVec4 dark(0.15f, 0.15f, 0.15f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, dark);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.9f, 0.9f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_TitleBg, dark);
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, dark);

        bool open = ImGui::Begin("Editor", &m_Show);

        if (open) // Check if window is open
        {

            if (ImGui::CollapsingHeader("File"))
            {
                if (ImGui::Button("Quick Save Scene"))
                {
                    m_app.GetSceneManager().GetActiveScene().Serialize(
                        "assets/scenes/" + m_app.GetSceneManager().GetActiveScene().getName());
                }

                if (ImGui::Button("Quick Load Scene"))
                {
                    m_app.GetSceneManager().GetActiveScene().Deserialize(
                        "assets/scenes/" + m_app.GetSceneManager().GetActiveScene().getName());
                }
            }

            if (ImGui::CollapsingHeader("Systems Control"))
            {

                if (ImGui::Button("Run Scripts"))
                {
                    if (m_app.IsSystemActive(Vakol::SystemFlag::Scripting) == false)
                        m_app.ToggleSystem(Vakol::SystemFlag::Scripting);
                }

                if (ImGui::Button("Pause Scripts"))
                {
                    if (m_app.IsSystemActive(Vakol::SystemFlag::Scripting))
                        m_app.ToggleSystem(Vakol::SystemFlag::Scripting);
                }

                if (ImGui::Button("Run Physics"))
                {
                    if (m_app.IsSystemActive(Vakol::SystemFlag::Physics) == false)
                        m_app.ToggleSystem(Vakol::SystemFlag::Physics);
                }

                if (ImGui::Button("Pause Physics"))
                {
                    if (m_app.IsSystemActive(Vakol::SystemFlag::Physics))
                        m_app.ToggleSystem(Vakol::SystemFlag::Physics);
                }

                if (ImGui::Button("Toggle Physics Wireframe"))
                {
                    m_app.GetSceneManager().GetActiveScene().SetDebug(
                        !m_app.GetSceneManager().GetActiveScene().IsDebugEnabled());
                }
            }

            ImGui::Separator();

            if (ImGui::CollapsingHeader("Entity List"))
            {

                ImGui::SeparatorText("Add Entities");

                if (ImGui::Button("Add Empty Entity"))
                {
                    m_app.GetSceneManager().GetActiveScene().CreateEntity("New Entity");
                }

                if (ImGui::Button("Add Entity with Sphere"))
                {
                    Vakol::Entity entity = m_app.GetSceneManager().GetActiveScene().CreateEntity("Default Sphere");

                    entity.AddComponent<Vakol::Rendering::Drawable>();

                    Vakol::Rendering::RenderEngine::GenerateSphere(1.0f,
                                                                   entity.GetComponent<Vakol::Rendering::Drawable>());
                }

                if (ImGui::Button("Add Entity with Cube"))
                {

                    Vakol::Entity entity = m_app.GetSceneManager().GetActiveScene().CreateEntity("Default Cube");

                    entity.AddComponent<Vakol::Rendering::Drawable>();

                    Vakol::Rendering::RenderEngine::GenerateCube(1.0f,
                                                                 entity.GetComponent<Vakol::Rendering::Drawable>());
                }

                ImGui::SeparatorText("Entities");

                ImGui::BeginChild("Entity List Child", ImVec2(0, 0), true);

                auto& EL = m_app.GetSceneManager().GetActiveScene().GetEntityList();

                EL.IterateEntities([&](auto handle) {
                    Vakol::Entity entity = EL.GetEntity(static_cast<uint32_t>(handle));
                    Vakol::Components::Tag& tag = entity.GetComponent<Vakol::Components::Tag>();
                    Vakol::Components::Transform& trans = entity.GetComponent<Vakol::Components::Transform>();

                    // ## is needed to differentiate entities by their handle
                    if (ImGui::CollapsingHeader((tag.tag + "##" + std::to_string(entity.GetHandle())).c_str()))
                    {
                        ImGui::Indent(20.0f);
                        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.9f, 0.4f, 0.f, 1.0f));

                        if (ImGui::Button("Delete Entity"))
                        {
                            m_app.GetSceneManager().GetActiveScene().DestroyEntity(entity);
                        }

                        if (ImGui::CollapsingHeader("Transform"))
                        {
                            ImGui::SeparatorText("Transform");

                            ImGui::DragFloat3("Position", &trans.pos.x, 0.1f);
                            ImGui::DragFloat3("Rotation", &trans.eulerAngles.x, 0.1f);
                            ImGui::DragFloat3("Scale", &trans.scale.x, 0.1f);

                            trans.rot = Vakol::Math::Quat(Vakol::Math::DegToRad(trans.eulerAngles));
                        }

                        if (entity.HasComponent<Vakol::Rendering::Drawable>() && ImGui::CollapsingHeader("Drawable"))
                        {
                            Vakol::Rendering::Drawable& drawable = entity.GetComponent<Vakol::Rendering::Drawable>();

                            ImGui::Text("Drawable ID: %s", drawable.ID.ConvertToString().c_str());
                            ImGui::Spacing();

                            ImGui::SeparatorText("Material");

                            Vakol::Rendering::Assets::Model& model = Vakol::AssetLoader::FindModel(drawable.ID);

                            for (Vakol::Rendering::Assets::Mesh& mesh : model.meshes)
                            {
                                ImGui::Text("Mesh ID: %s", mesh.ID.c_str());

                                Vakol::Rendering::Assets::Material& material = mesh.material;

                                ImGui::Text("Material ID: %s", material.ID.c_str());

                                ImGui::Checkbox(("Use Lighting##" + material.ID).c_str(),
                                                &material.properties.use_lighting);
                                ImGui::Checkbox(("Use Textures##" + material.ID).c_str(),
                                                &material.properties.use_textures);

                                ImGui::ColorEdit4(("Ambient Color##" + material.ID).c_str(),
                                                  &material.properties.ambient_color.x);
                                ImGui::ColorEdit4(("Diffuse Color##" + material.ID).c_str(),
                                                  &material.properties.diffuse_color.x);
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
            m_app.SetActiveMouse(m_Show);
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

        if (keyEvent.GetKeyCode() == static_cast<int>(Vakol::Input::KEY::KEY_SEMICOLON))
        {
            m_Show = !m_Show;

            m_app.SetActiveMouse(m_Show);
            event.Handled = true;

            m_app.SetGameState(m_Show ? Vakol::GameState::Paused : Vakol::GameState::Running);

            return;
        }

        if (m_Show) // basically disabling any keyboard presses getting to the game when the editor is open
        {
            if (keyEvent.GetKeyCode() == static_cast<int>(Vakol::Input::KEY::KEY_ESCAPE))
            {
                m_Show = false;
                m_app.SetActiveMouse(m_Show);
                m_app.SetGameState(Vakol::GameState::Running);
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

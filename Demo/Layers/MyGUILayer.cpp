#include "MyGUILayer.hpp"

#include <cstring>

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

static Vakol::Math::Vec3 Vec3(const rp3d::Vector3& v)
{
    return {v.x, v.y, v.z};
}

static rp3d::Vector3 Vec3(const Vakol::Math::Vec3& v)
{
    return {v.x, v.y, v.z};
}

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
                ImGui::DragFloat("Velocity Damping", &m_app.GetPhysicsEngine().velocityDamping, 0.001f);
                ImGui::DragFloat3("Gravity", &m_app.GetPhysicsEngine().gravity.x, 0.1f);
            }

            ImGui::Separator();

            // if (ImGui::CollapsingHeader("Camera Debug"))
            //{
            //     if (const Vakol::Camera* camera = &m_app.GetSceneManager().GetActiveScene().GetCamera())
            //     {
            //         const Vakol::Math::Vec3 pos = camera->GetPos();
            //         const Vakol::Math::Vec3 fwd = camera->GetForward();
            //         const Vakol::Math::Vec3 rgt = camera->GetRight();

            //        ImGui::Text("Camera Position");
            //        ImGui::Text("x: %f, y: %f, z: %f", pos.x, pos.y, pos.z);

            //        ImGui::Spacing();

            //        ImGui::Text("Camera Forward");
            //        ImGui::Text("x: %f, y: %f, z: %f", fwd.x, fwd.y, fwd.z);

            //        ImGui::Spacing();

            //        ImGui::Text("Camera Right");
            //        ImGui::Text("x: %f, y: %f, z: %f", rgt.x, fwd.y, fwd.z);

            //        ImGui::Spacing();

            //        ImGui::Text("Camera Pitch");
            //        ImGui::Text("%f", camera->GetPitch());

            //        ImGui::Spacing();

            //        ImGui::Text("Camera Yaw");
            //        ImGui::Text("%f", camera->GetYaw());

            //        const Vakol::Math::Mat4& proj = camera->GetProjectionMatrix();
            //        const Vakol::Math::Mat4& view = camera->GetViewMatrix();

            //        ImGui::Spacing();
            //        ImGui::SeparatorText("Projection Matrix");

            //        ImGui::Text("x: %f, %f, %f, %f", proj[0][0], proj[0][1], proj[0][2], proj[0][3]);
            //        ImGui::Text("y: %f, %f, %f, %f", proj[1][0], proj[1][1], proj[1][2], proj[1][3]);
            //        ImGui::Text("z: %f, %f, %f, %f", proj[2][0], proj[2][1], proj[2][2], proj[2][3]);
            //        ImGui::Text("w: %f, %f, %f, %f", proj[3][0], proj[3][1], proj[3][2], proj[3][3]);

            //        ImGui::Spacing();
            //        ImGui::SeparatorText("View Matrix");

            //        ImGui::Text("x: %f, %f, %f, %f", view[0][0], view[0][1], view[0][2], view[0][3]);
            //        ImGui::Text("y: %f, %f, %f, %f", view[1][0], view[1][1], view[1][2], view[1][3]);
            //        ImGui::Text("z: %f, %f, %f, %f", view[2][0], view[2][1], view[2][2], view[2][3]);
            //        ImGui::Text("w: %f, %f, %f, %f", view[3][0], view[3][1], view[3][2], view[3][3]);
            //    }
            //}

            // ImGui::Separator();

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

                            ImGui::Spacing();

                            ImGui::Text("Quaternion Rotation");
                            ImGui::Text("x: %f, y: %f, z: %f, w: %f", trans.rot.x, trans.rot.y, trans.rot.z,
                                        trans.rot.w);
                        }

                        if (entity.HasComponent<Vakol::Rendering::Drawable>() && ImGui::CollapsingHeader("Drawable"))
                        {
                            const Vakol::Rendering::Drawable& drawable =
                                entity.GetComponent<Vakol::Rendering::Drawable>();

                            Vakol::Rendering::Assets::Model& model = Vakol::AssetLoader::FindModel(drawable.ID);

                            for (Vakol::Rendering::Assets::Mesh& mesh : model.meshes)
                            {
                                Vakol::Rendering::Assets::Material& material = mesh.material;

                                if (!mesh.name.empty())
                                    ImGui::Text("Mesh Name: %s", mesh.name.c_str());

                                ImGui::SeparatorText("AABB Bounds");

                                ImGui::Text("Min: x: %f, y: %f, z: %f", mesh.bounds.min.x, mesh.bounds.min.y,
                                            mesh.bounds.min.z);
                                ImGui::Text("Max: x: %f, y: %f, z: %f", mesh.bounds.max.x, mesh.bounds.max.y,
                                            mesh.bounds.max.z);
                                ImGui::Text("Half Extents: x: %f, y: %f, z: %f", mesh.bounds.halfExtents.x,
                                            mesh.bounds.halfExtents.y, mesh.bounds.halfExtents.z);

                                ImGui::Separator();
                                ImGui::Spacing();

                                ImGui::Checkbox(("Use Lighting##" + material.ID).c_str(),
                                                &material.properties.use_lighting);

                                if (material.properties.use_lighting)
                                {
                                    ImGui::SeparatorText("Lighting");

                                    ImGui::DragFloat3(("Light Position##" + material.ID).c_str(),
                                                      &material.properties.light_position.x, 0.1f);
                                    ImGui::DragFloat3(("Light Direction##" + material.ID).c_str(),
                                                      &material.properties.light_direction.x, 0.1f);
                                }

                                ImGui::Spacing();
                                ImGui::Spacing();

                                ImGui::SeparatorText("Material");

                                ImGui::Checkbox(("Use Textures##" + material.ID).c_str(),
                                                &material.properties.use_textures);

                                ImGui::Separator();
                                ImGui::Spacing();

                                if (material.properties.use_textures)
                                    ImGui::DragFloat2(("UV Offset##" + material.ID).c_str(),
                                                      &material.properties.uv_offset.x, 0.05f);
                                else
                                {
                                    ImGui::ColorEdit4(("Diffuse Color##" + material.ID).c_str(),
                                                      &material.properties.diffuse_color.x);
                                    ImGui::ColorEdit4(("Ambient Color##" + material.ID).c_str(),
                                                      &material.properties.ambient_color.x);
                                    ImGui::ColorEdit4(("Specular Color##" + material.ID).c_str(),
                                                      &material.properties.specular_color.x);
                                    ImGui::ColorEdit4(("Emissive Color##" + material.ID).c_str(),
                                                      &material.properties.emissive_color.x);
                                }

                                ImGui::DragFloat(("Shininess##" + material.ID).c_str(), &material.properties.shininess,
                                                 0.1f, 32.0f, FLT_MAX);

                                ImGui::DragFloat(("Opacity##" + material.ID).c_str(), &material.properties.opacity,
                                                 0.1f, 0.0f, 1.0f);

                                ImGui::Separator();
                                ImGui::Spacing();
                            }
                        }

                        if (entity.HasComponent<Vakol::RigidBody>() && ImGui::CollapsingHeader("RigidBody"))
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
                            ImGui::Checkbox("Is Sleeping", &rb.isSleeping);
                            ImGui::DragFloat("Mass", &rb.mass, 0.1f);
                            ImGui::DragFloat("Bounciness", &rb.bounciness, 0.1f);
                            ImGui::DragFloat3("Center of Mass", &rb.centerOfMass.x, 0.1f);

                            ImGui::DragFloat3("Force", &rb.force.x, 0.1f);
                            ImGui::DragFloat3("Torque", &rb.torque.x, 0.1f);
                            ImGui::DragFloat3("Linear Velocity", &rb.linearVelocity.x, 0.1f);
                            ImGui::DragFloat3("Angular Velocity", &rb.angularVelocity.x, 0.1f);

                            ImGui::Spacing();
                            ImGui::SeparatorText("Inverse Interia Tensor");

                            ImGui::DragFloat3("Inverse Interia Tensor", &rb.invInertiaTensor.x, 0.0f);

                            if (rb.collisionData)
                            {
                                // ImGui::DragFloat3("World Normal", &rb.collisionData->worldNormal.x, 0.1f);
                                // ImGui::DragFloat3("World Point", &rb.collisionData->worldPoint.x, 0.1f);
                                // ImGui::DragFloat3("Local Point", &rb.collisionData->localPoint.x, 0.1f);
                                // ImGui::DragScalar("Penetration Depth", ImGuiDataType_Double,
                                //                   &rb.collisionData->penetrationDepth, 0.1f);
                                // ImGui::Checkbox("Is Colliding", &rb.collisionData->isColliding);
                            }
                        }

                        // TODO allow it to handle new collider data
                        // if (entity.HasComponent<Vakol::BoxCollider>() && ImGui::CollapsingHeader("Box Collider"))
                        // {
                        //     const Vakol::BoxCollider& collider = entity.GetComponent<Vakol::BoxCollider>();

                        //     Vakol::Math::Vec3 halfExtents = Vec3(collider.shape->getHalfExtents());

                        //     ImGui::SeparatorText("Size");
                        //     ImGui::DragFloat3("Half Extents", &halfExtents.x, 0.1f);

                        //     collider.shape->setHalfExtents(Vec3(halfExtents));
                        // }

                        // if (entity.HasComponent<Vakol::SphereCollider>() && ImGui::CollapsingHeader("Sphere
                        // Collider"))
                        // {
                        //     const Vakol::SphereCollider& collider = entity.GetComponent<Vakol::SphereCollider>();

                        //     constexpr float min = 0.01f;
                        //     constexpr float max = 100.0f;

                        //     float radius = collider.shape->getRadius();

                        //     ImGui::SeparatorText("Size");
                        //     ImGui::DragScalar("Radius", ImGuiDataType_Double, &radius, 0.1f);

                        //     if (radius < min)
                        //         radius = min;
                        //     if (radius > max)
                        //         radius = max;

                        //     collider.shape->setRadius(radius);
                        // }

                        // if (entity.HasComponent<Vakol::CapsuleCollider>() &&
                        //     ImGui::CollapsingHeader("Capsule Collider"))
                        // {
                        //     const Vakol::CapsuleCollider& collider = entity.GetComponent<Vakol::CapsuleCollider>();

                        //     constexpr float min = 0.01;
                        //     constexpr float max = 100.0;

                        //     float radius = collider.shape->getRadius();
                        //     float height = collider.shape->getHeight();

                        //     ImGui::SeparatorText("Size");
                        //     ImGui::DragScalar("Radius", ImGuiDataType_Double, &radius, 0.1f);
                        //     ImGui::DragScalar("Height", ImGuiDataType_Double, &height, 0.1f);

                        //     if (radius < min)
                        //         radius = min;
                        //     if (radius > max)
                        //         radius = max;

                        //     if (height < min)
                        //         height = min;
                        //     if (height > max)
                        //         height = max;

                        //     collider.shape->setRadius(radius);
                        //     collider.shape->setHeight(height);
                        // }

                        ImGui::Spacing();

                        if (!entity.HasComponent<Vakol::RigidBody>() && ImGui::Button("Add Rigidbody"))
                        {
                            Vakol::Components::Transform& transform =
                                entity.GetComponent<Vakol::Components::Transform>();

                            Vakol::RigidBody rb =
                                m_app.GetSceneManager().GetActiveScene().GetPhysicsScene().CreateRigidBody(
                                    transform.pos, transform.rot);

                            entity.AddComponent<Vakol::RigidBody>(rb);
                        }

                        ImGui::Spacing();

                        // TODO might wanna fix this eventually for compound bodies
                        // if (entity.HasComponent<Vakol::RigidBody>())
                        // {
                        //     if (!entity.HasComponent<Vakol::BoxCollider>() && ImGui::Button("Add Box Collider"))
                        //     {
                        //         Vakol::RigidBody& rb = entity.GetComponent<Vakol::RigidBody>();

                        //         Vakol::Math::Vec3 extents = Vakol::Math::Vec3(1.0f);

                        //         Vakol::BoxCollider collider = m_app.GetPhysicsEngine().CreateBoxCollider(extents);
                        //         m_app.GetPhysicsEngine().AttachCollider(rb, collider);

                        //         entity.AddComponent<Vakol::BoxCollider>(collider);
                        //     }

                        //     if (!entity.HasComponent<Vakol::SphereCollider>() && ImGui::Button("Add Sphere
                        //     Collider"))
                        //     {
                        //         Vakol::RigidBody& rb = entity.GetComponent<Vakol::RigidBody>();

                        //         constexpr float radius = 1.0;

                        //         Vakol::SphereCollider collider =
                        //         m_app.GetPhysicsEngine().CreateSphereCollider(radius);
                        //         m_app.GetPhysicsEngine().AttachCollider(rb, collider);

                        //         entity.AddComponent<Vakol::SphereCollider>(collider);
                        //     }

                        //     if (!entity.HasComponent<Vakol::CapsuleCollider>() && ImGui::Button("Add Capsule
                        //     Collider"))
                        //     {
                        //         Vakol::RigidBody& rb = entity.GetComponent<Vakol::RigidBody>();

                        //         constexpr float radius = 0.5;
                        //         constexpr float height = 1.0;

                        //         Vakol::CapsuleCollider collider =
                        //             m_app.GetPhysicsEngine().CreateCapsuleCollider(radius, height);

                        //         m_app.GetPhysicsEngine().AttachCollider(rb, collider);

                        //         entity.AddComponent<Vakol::CapsuleCollider>(collider);
                        //     }
                        // }

                        if (entity.HasComponent<Vakol::ScriptComp>() && ImGui::CollapsingHeader("Scripts"))
                        {
                            Vakol::ScriptComp& scriptComp = entity.GetComponent<Vakol::ScriptComp>();

                            for (auto& script : scriptComp.scripts)
                            {
                                ImGui::SeparatorText(script.name.c_str());

                                for (const auto& kv : script.env)
                                {
                                    if (!kv.first.valid() || !kv.second.valid())
                                        continue;

                                    if (kv.first.is<std::string>())
                                    {
                                        if (kv.second.is<std::string>())
                                        {
                                            ImGui::TextColored(ImVec4(0.37f, 0.66f, 0.69f, 1.0f), "%s",
                                                               kv.first.as<std::string>().c_str());
                                            ImGui::Text("%s", kv.second.as<std::string>().c_str());
                                            ImGui::Dummy(ImVec2(0.0f, 10.0f));
                                        }
                                        else if (kv.second.is<float>())
                                        {
                                            ImGui::TextColored(ImVec4(1.f, 0.33f, 0.33f, 1.0f), "%s",
                                                               kv.first.as<std::string>().c_str());
                                            ImGui::Text("%.2f", kv.second.as<float>());
                                            ImGui::Dummy(ImVec2(0.0f, 10.0f));
                                        }
                                        else if (kv.second.is<int>())
                                        {
                                            ImGui::TextColored(ImVec4(0.83f, 0.88f, 0.18f, 1.0f), "%s",
                                                               kv.first.as<std::string>().c_str());
                                            ImGui::Text("%d", kv.second.as<int>());
                                            ImGui::Dummy(ImVec2(0.0f, 10.0f));
                                        }
                                        else if (kv.second.is<bool>())
                                        {
                                            ImGui::TextColored(ImVec4(0.28f, 0.88f, 0.35f, 1.0f), "%s",
                                                               kv.first.as<std::string>().c_str());
                                            ImGui::Text("%s", kv.second.as<bool>() ? "true" : "false");
                                            ImGui::Dummy(ImVec2(0.0f, 10.0f));
                                        }
                                    }
                                }
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

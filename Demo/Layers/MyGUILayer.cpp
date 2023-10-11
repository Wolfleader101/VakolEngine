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

                            ImGui::Text("Drawable ID: %s", drawable.ID.ToString().c_str());
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
                            ImGui::SeparatorText("Rotiation Matrix");

                            // ImGui::Text("Rotation Matrix");
                            ImGui::Text("x: %f, %f, %f", rb.rotationMatrix[0][0], rb.rotationMatrix[0][1],
                                        rb.rotationMatrix[0][2]);
                            ImGui::Text("y: %f, %f, %f", rb.rotationMatrix[1][0], rb.rotationMatrix[1][1],
                                        rb.rotationMatrix[1][2]);
                            ImGui::Text("z: %f, %f, %f", rb.rotationMatrix[2][0], rb.rotationMatrix[2][1],
                                        rb.rotationMatrix[2][2]);

                            ImGui::Spacing();
                            ImGui::SeparatorText("Interia Tensor");

                            ImGui::Text("x: %f, %f, %f", rb.inertiaTensor[0][0], rb.inertiaTensor[0][1],
                                        rb.inertiaTensor[0][2]);
                            ImGui::Text("y: %f, %f, %f", rb.inertiaTensor[1][0], rb.inertiaTensor[1][1],
                                        rb.inertiaTensor[1][2]);
                            ImGui::Text("z: %f, %f, %f", rb.inertiaTensor[2][0], rb.inertiaTensor[2][1],
                                        rb.inertiaTensor[2][2]);

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

                        if (entity.HasComponent<Vakol::BoxCollider>() && ImGui::CollapsingHeader("Box Collider"))
                        {
                            Vakol::BoxCollider& collider = entity.GetComponent<Vakol::BoxCollider>();

                            Vakol::Math::Vec3 halfExtents = Vec3(collider.shape->getHalfExtents());

                            ImGui::SeparatorText("Size");
                            ImGui::DragFloat3("Half Extents", &halfExtents.x, 0.1f);

                            collider.shape->setHalfExtents(Vec3(halfExtents));
                        }

                        if (entity.HasComponent<Vakol::SphereCollider>() && ImGui::CollapsingHeader("Sphere Collider"))
                        {
                            Vakol::SphereCollider& collider = entity.GetComponent<Vakol::SphereCollider>();

                            double min = 0.01;
                            double max = 100.0;

                            double radius = collider.shape->getRadius();

                            ImGui::SeparatorText("Size");
                            ImGui::DragScalar("Radius", ImGuiDataType_Double, &radius, 0.1f);

                            if (radius < min)
                                radius = min;
                            if (radius > max)
                                radius = max;

                            collider.shape->setRadius(radius);
                        }

                        if (entity.HasComponent<Vakol::CapsuleCollider>() &&
                            ImGui::CollapsingHeader("Capsule Collider"))
                        {
                            Vakol::CapsuleCollider& collider = entity.GetComponent<Vakol::CapsuleCollider>();

                            double min = 0.01;
                            double max = 100.0;

                            double radius = collider.shape->getRadius();
                            double height = collider.shape->getHeight();

                            ImGui::SeparatorText("Size");
                            ImGui::DragScalar("Radius", ImGuiDataType_Double, &radius, 0.1f);
                            ImGui::DragScalar("Height", ImGuiDataType_Double, &height, 0.1f);

                            if (radius < min)
                                radius = min;
                            if (radius > max)
                                radius = max;

                            if (height < min)
                                height = min;
                            if (height > max)
                                height = max;

                            collider.shape->setRadius(radius);
                            collider.shape->setHeight(height);
                        }

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

                        if (entity.HasComponent<Vakol::RigidBody>())
                        {
                            if (!entity.HasComponent<Vakol::BoxCollider>() && ImGui::Button("Add Box Collider"))
                            {
                                Vakol::RigidBody& rb = entity.GetComponent<Vakol::RigidBody>();

                                Vakol::Math::Vec3 extents = Vakol::Math::Vec3(1.0f);

                                Vakol::BoxCollider collider = m_app.GetPhysicsEngine().CreateBoxCollider(extents);
                                m_app.GetPhysicsEngine().AttachCollider(rb, collider);

                                entity.AddComponent<Vakol::BoxCollider>(collider);
                            }

                            if (!entity.HasComponent<Vakol::SphereCollider>() && ImGui::Button("Add Sphere Collider"))
                            {
                                Vakol::RigidBody& rb = entity.GetComponent<Vakol::RigidBody>();

                                double radius = 1.0;

                                Vakol::SphereCollider collider = m_app.GetPhysicsEngine().CreateSphereCollider(radius);
                                m_app.GetPhysicsEngine().AttachCollider(rb, collider);

                                entity.AddComponent<Vakol::SphereCollider>(collider);
                            }

                            if (!entity.HasComponent<Vakol::CapsuleCollider>() && ImGui::Button("Add Capsule Collider"))
                            {
                                Vakol::RigidBody& rb = entity.GetComponent<Vakol::RigidBody>();

                                double radius = 0.5;
                                double height = 1.0;

                                Vakol::CapsuleCollider collider =
                                    m_app.GetPhysicsEngine().CreateCapsuleCollider(radius, height);
                                m_app.GetPhysicsEngine().AttachCollider(rb, collider);

                                entity.AddComponent<Vakol::CapsuleCollider>(collider);
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

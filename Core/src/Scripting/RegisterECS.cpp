#include "AssetLoader/AssetLoader.hpp"
#include "ECS/Components.hpp"
#include "ECS/Entity.hpp"
#include "LuaAccess.hpp"
#include "SceneManager/Scene.hpp"

#include "Physics/PhysicsEngine.hpp"
#include "Physics/PhysicsTypes.hpp"
#include "Rendering/RenderEngine.hpp"

namespace Vakol
{

    void RegisterEntity(sol::state& lua)
    {
        auto entity_type = lua.new_usertype<Entity>("Entity");

        entity_type.set_function("get_tag", [](Entity* ent) { return ent->GetComponent<Components::Tag>().tag; });
        entity_type.set_function("get_transform", &Entity::GetComponent<Components::Transform>);
        entity_type.set_function("get_fsm", &Entity::GetComponent<Components::FSM>);

        entity_type.set_function("add_model", [](Entity* ent, const std::string& path, const float scale = 1.0f) {
            if (!ent->HasComponent<Rendering::Drawable>())
                ent->AddComponent<Rendering::Drawable>();

            auto& drawable = ent->GetComponent<Rendering::Drawable>();

            auto& model = AssetLoader::GetModel(drawable.ID, path, scale);
            Rendering::RenderEngine::GenerateModel(model, drawable, "coreAssets/shaders/default.shader");

            return model;
        });

        entity_type.set_function("add_shaded_model", [](Entity* ent, const std::string& path, const float scale,
                                                        const std::string& shaderPath) {
            if (!ent->HasComponent<Rendering::Drawable>())
                ent->AddComponent<Rendering::Drawable>();

            auto& drawable = ent->GetComponent<Rendering::Drawable>();

            auto& model = AssetLoader::GetModel(drawable.ID, path, scale);
            Rendering::RenderEngine::GenerateModel(model, drawable, shaderPath);

            return model;
        });

        entity_type.set_function("replace_texture",
                                 [](const Entity* ent, const std::string& srcPath, const std::string& srcType,
                                    const std::string& dstPath, const std::string& dstType) {
                                     if (!ent->HasComponent<Rendering::Drawable>())
                                     {
                                         VK_ERROR("No Drawable component found on entity!");

                                         return;
                                     }

                                     AssetLoader::ReplaceTexture(ent->GetComponent<Rendering::Drawable>().ID, srcPath,
                                                                 Rendering::Assets::ToTextureType(srcType), dstPath,
                                                                 Rendering::Assets::ToTextureType(dstType));
                                 });

        entity_type.set_function("active_model", [](const Entity* ent, const bool active) {
            if (ent->HasComponent<Rendering::Drawable>())
                ent->GetComponent<Rendering::Drawable>().active = active;
        });

        entity_type.set_function("add_rigid", [&](Entity* ent) -> RigidBody& {
            Scene& scene = lua["scene"];

            RigidBody rb = scene.GetPhysicsScene().CreateRigidBody(ent->GetComponent<Components::Transform>().pos,
                                                                   ent->GetComponent<Components::Transform>().rot);
            ent->AddComponent<RigidBody>(rb);

            // TODO implement this
            auto& newRb = ent->GetComponent<RigidBody>();
            newRb.collisionData->parentBody = &newRb;

            return newRb;
        });

        entity_type.set_function("get_rigid", [](const Entity* ent) {
            if (ent->HasComponent<RigidBody>())
                return &ent->GetComponent<RigidBody>();

            VK_CRITICAL("No rigid body component found on entity");

            return static_cast<RigidBody*>(nullptr);
        });

        entity_type.set_function("add_sphere_collider", [&](Entity* ent, const double radius) {
            PhysicsEngine& physEngine = lua["PhysicsEngine"];

            if (!ent->HasComponent<RigidBody>())
            {
                VK_CRITICAL("No rigid body component found on entity");

                return static_cast<SphereCollider*>(nullptr);
            }

            SphereCollider collider = physEngine.CreateSphereCollider(radius);

            ent->AddComponent<SphereCollider>(collider);
            physEngine.AttachCollider(ent->GetComponent<RigidBody>(), collider);

            return &ent->GetComponent<SphereCollider>();
        });

        entity_type.set_function("add_box_collider", [&](Entity* ent, Math::Vec3& halfExtents) {
            PhysicsEngine& physEngine = lua["PhysicsEngine"];

            if (!ent->HasComponent<RigidBody>())
            {
                VK_CRITICAL("No rigid body component found on entity");

                return static_cast<BoxCollider*>(nullptr);
            }

            BoxCollider collider = physEngine.CreateBoxCollider(halfExtents);

            ent->AddComponent<BoxCollider>(collider);
            physEngine.AttachCollider(ent->GetComponent<RigidBody>(), collider);

            return &ent->GetComponent<BoxCollider>();
        });

        // TODO remove FSM component
        entity_type.set_function("add_fsm", [&](Entity* ent) -> Components::FSM& {
            if (!ent->HasComponent<Components::FSM>())
                ent->AddComponent<Components::FSM>(lua.create_table());
            return ent->GetComponent<Components::FSM>();
        });
    }
} // namespace Vakol
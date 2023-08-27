#include "AssetLoader/AssetLoader.hpp"
#include "ECS/Components.hpp"
#include "ECS/Entity.hpp"
#include "LuaAccess.hpp"
#include "SceneManager/Scene.hpp"

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

            auto model = AssetLoader::GetModel(path, scale);
            auto& drawable = ent->GetComponent<Rendering::Drawable>();

            Rendering::RenderEngine::GenerateModel(model, drawable);

            return std::make_shared<Rendering::Assets::Model>(std::move(model));
        });

        entity_type.set_function("add_model_direct", [](Entity* ent, Rendering::Assets::Model& inputModel) {
            if (!ent->HasComponent<Rendering::Drawable>())
                ent->AddComponent<Rendering::Drawable>();

            auto& drawable = ent->GetComponent<Rendering::Drawable>();

            AssetLoader::AddModelDirect(inputModel);

            Rendering::RenderEngine::GenerateModel(inputModel, drawable);
        });

        entity_type.set_function("replace_texture", [](const Entity* ent, const std::string& srcPath,
                                                       const std::string& srcType, const std::string& dstPath,
                                                       const std::string& dstType) {
            if (!ent->HasComponent<Rendering::Drawable>())
            {
                VK_ERROR("No Drawable component found on entity!");

                return;
            }

            const auto& model = AssetLoader::FindModel(ent->GetComponent<Rendering::Drawable>().modelID);

            AssetLoader::ReplaceTexture(model.path, srcPath, Rendering::Assets::ToTextureType(srcType), dstPath,
                                        Rendering::Assets::ToTextureType(dstType));
        });

        entity_type.set_function("active_model", [](const Entity* ent, const bool active) {
            if (ent->HasComponent<Rendering::Drawable>())
                ent->GetComponent<Rendering::Drawable>().active = active;
        });

        // entity_type.set_function("add_rigid", [](Entity* ent) -> Components::RigidBody& {
        //     if (!ent->HasComponent<Components::RigidBody>())
        //         ent->AddComponent<Components::RigidBody>();

        //     return ent->GetComponent<Components::RigidBody>();
        // });

        // entity_type.set_function("get_rigid", [](const Entity* ent) -> Components::RigidBody& {
        //     if (ent->HasComponent<Components::RigidBody>())
        //         return ent->GetComponent<Components::RigidBody>();

        //     VK_CRITICAL("No rigid body component found on entity");
        //     assert(0);
        // });

        // entity_type.set_function("add_collider", [](Entity* ent) -> Components::Collider& {
        //     if (!ent->HasComponent<Components::Collider>())
        //         ent->AddComponent<Components::Collider>();
        //     return ent->GetComponent<Components::Collider>();
        // });

        // entity_type.set_function("get_collider", [](const Entity* ent) -> Components::Collider& {
        //     if (ent->HasComponent<Components::Collider>())
        //         return ent->GetComponent<Components::Collider>();

        //     VK_CRITICAL("No collider component found on entity");
        //     assert(0);
        // });

        /* Changed by Caleb */
        // entity_type.set_function("get_bounds_from_model", [](const Entity* ent) -> void {
        //     if (ent->HasComponent<Rendering::Drawable, Components::Collider>())
        //     {
        //         const auto& model = ent->GetComponent<Rendering::Drawable>();

        //        auto& collider = ent->GetComponent<Components::Collider>();

        //        collider.bounds = GetBounds(model, ent->GetComponent<Components::Transform>());
        //    }
        //    else
        //    {
        //        VK_CRITICAL("drawable and collider must be present to get bounds from");
        //        assert(0);
        //    }
        //});

        // TODO remove FSM component
        entity_type.set_function("add_fsm", [&](Entity* ent) -> Components::FSM& {
            if (!ent->HasComponent<Components::FSM>())
                ent->AddComponent<Components::FSM>(lua.create_table());
            return ent->GetComponent<Components::FSM>();
        });
    }
} // namespace Vakol

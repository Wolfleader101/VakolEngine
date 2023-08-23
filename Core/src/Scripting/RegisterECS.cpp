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

        entity_type.set_function("set_bool", [](const Entity* ent, const char* name, const bool value) {
            if (!ent->HasComponent<Rendering::Drawable>())
            {
                VK_ERROR("No Drawable component found on entity!");

                return;
            }

            AssetLoader::SetBool(AssetLoader::GetShader(ent->GetComponent<Rendering::Drawable>().shaderID), name,
                                 value);
        });

        entity_type.set_function("set_int", [](const Entity* ent, const char* name, const int value) {
            if (!ent->HasComponent<Rendering::Drawable>())
            {
                VK_ERROR("No Drawable component found on entity!");

                return;
            }

            AssetLoader::SetInt(AssetLoader::GetShader(ent->GetComponent<Rendering::Drawable>().shaderID), name, value);
        });

        entity_type.set_function("set_float", [](const Entity* ent, const char* name, const float value) {
            if (!ent->HasComponent<Rendering::Drawable>())
            {
                VK_ERROR("No Drawable component found on entity!");

                return;
            }

            AssetLoader::SetFloat(AssetLoader::GetShader(ent->GetComponent<Rendering::Drawable>().shaderID), name,
                                  value);
        });

        entity_type.set_function("set_vec2v", [](const Entity* ent, const char* name, const Math::Vec2& value) {
            if (!ent->HasComponent<Rendering::Drawable>())
            {
                VK_ERROR("No Drawable component found on entity!");

                return;
            }

            AssetLoader::SetVec2(AssetLoader::GetShader(ent->GetComponent<Rendering::Drawable>().shaderID), name,
                                 value);
        });

        entity_type.set_function("set_vec3v", [](const Entity* ent, const char* name, const Math::Vec3& value) {
            if (!ent->HasComponent<Rendering::Drawable>())
            {
                VK_ERROR("No Drawable component found on entity!");

                return;
            }

            AssetLoader::SetVec3(AssetLoader::GetShader(ent->GetComponent<Rendering::Drawable>().shaderID), name,
                                 value);
        });

        entity_type.set_function("set_vec4v", [](const Entity* ent, const char* name, const Math::Vec4& value) {
            if (!ent->HasComponent<Rendering::Drawable>())
            {
                VK_ERROR("No Drawable component found on entity!");

                return;
            }

            AssetLoader::SetVec4(AssetLoader::GetShader(ent->GetComponent<Rendering::Drawable>().shaderID), name,
                                 value);
        });

        entity_type.set_function(
            "set_mat3", [](const Entity* ent, const char* name, const bool transpose, const Math::Mat3& value) {
                if (!ent->HasComponent<Rendering::Drawable>())
                {
                    VK_ERROR("No Drawable component found on entity!");

                    return;
                }

                AssetLoader::SetMat3(AssetLoader::GetShader(ent->GetComponent<Rendering::Drawable>().shaderID), name,
                                     transpose, value);
            });

        entity_type.set_function(
            "set_mat4", [](const Entity* ent, const char* name, const bool transpose, const Math::Mat4& value) {
                if (!ent->HasComponent<Rendering::Drawable>())
                {
                    VK_ERROR("No Drawable component found on entity!");

                    return;
                }

                AssetLoader::SetMat4(AssetLoader::GetShader(ent->GetComponent<Rendering::Drawable>().shaderID), name,
                                     transpose, value);
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
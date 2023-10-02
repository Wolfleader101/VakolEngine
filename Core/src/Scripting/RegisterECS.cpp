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

        entity_type.set_function(
            "add_model",
            [](Entity* ent, const std::string& path, const float scale = 1.0f) -> Rendering::Assets::Model& {
                if (!ent->HasComponent<Rendering::Drawable>())
                    ent->AddComponent<Rendering::Drawable>();

                auto& drawable = ent->GetComponent<Rendering::Drawable>();

                auto& model = AssetLoader::GetModel(drawable.ID, path, scale);
                Rendering::RenderEngine::GenerateModel(model, drawable, "coreAssets/shaders/default.shader");

                return model;
            });

        entity_type.set_function("add_shaded_model",
                                 [](Entity* ent, const std::string& path, const float scale,
                                    const std::string& shaderPath) -> Rendering::Assets::Model& {
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

        // TODO remove FSM component
        entity_type.set_function("add_fsm", [&](Entity* ent) -> Components::FSM& {
            if (!ent->HasComponent<Components::FSM>())
                ent->AddComponent<Components::FSM>(lua.create_table());
            return ent->GetComponent<Components::FSM>();
        });
    }
} // namespace Vakol
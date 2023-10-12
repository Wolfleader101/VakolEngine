#include "AssetLoader/AssetLoader.hpp"
#include "ECS/Components.hpp"
#include "ECS/Entity.hpp"
#include "LuaAccess.hpp"
#include "SceneManager/Scene.hpp"

#include "Logger/Logger.hpp"
#include "Rendering/RenderEngine.hpp"
#include "Scripting/ScriptEngine.hpp"

namespace Vakol
{

    void RegisterEntity(sol::state& lua, ScriptEngine& scriptEngine)
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

        // ADD PHSYCIS HERE

        entity_type.set_function("add_script", [&](Entity* ent, const std::string& name, const std::string& path) {
            if (!ent->HasComponent<ScriptComp>())
                ent->AddComponent<ScriptComp>();

            auto& scriptComp = ent->GetComponent<ScriptComp>();

            // if script with name already exists retunr
            for (auto& script : scriptComp.scripts)
            {
                if (script.name == name)
                {
                    VK_ERROR("Script with name {} already exists on entity {}", name,
                             ent->GetComponent<Components::Tag>().tag);

                    return;
                }
            }

            LuaScript tScript = scriptEngine.CreateScript(name, "scripts/" + path);

            scriptComp.scripts.push_back(std::move(tScript));
            LuaScript& script = scriptComp.scripts.back();

            scriptEngine.SetScriptVariable(script, "entity", ent);
            scriptEngine.InitScript(script);
        });

        entity_type.set_function("get_script", [&](Entity* ent, const std::string& name) -> LuaTable {
            if (!ent->HasComponent<ScriptComp>())
            {
                VK_ERROR("No script component found on entity {}", ent->GetComponent<Components::Tag>().tag);

                return LuaTable();
            }

            auto& scriptComp = ent->GetComponent<ScriptComp>();

            for (auto& script : scriptComp.scripts)
            {
                if (script.name == name)
                {
                    // Retrieve the script's environment using "state"
                    LuaType luaObj = scriptEngine.GetScriptVariable(script, "state");
                    if (luaObj.is<LuaTable>()) // Ensure that the retrieved object is a table
                    {
                        return luaObj.as<LuaTable>();
                    }
                    else
                    {
                        VK_ERROR("State variable in script {} is not a table", name);
                        return LuaTable();
                    }
                }
            }

            VK_ERROR("No script with name {} found on entity {}", name, ent->GetComponent<Components::Tag>().tag);

            return LuaTable(); // Return an empty sol::table or sol::nil
        });
    }
} // namespace Vakol
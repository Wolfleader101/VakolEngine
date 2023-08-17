#include "LuaAccess.hpp"

#include "ECS/Components.hpp"
#include "SceneManager/Scene.hpp"
#include "Scripting/ScriptEngine.hpp"

namespace Vakol
{
    void RegisterScene(sol::state& lua, ScriptEngine& scriptEngine)
    {
        auto scene_type = lua.new_usertype<Scene>("Scene");

        scene_type.set("globals", sol::property([](Scene& self) { return self.GetScript().env; }));

        // scene_type.set_function("create_entity", &Scene::CreateEntity);

        scene_type.set_function("create_entity", [&](Scene* scene, const std::string tag, const std::string& sname) {
            auto ent = scene->CreateEntity(tag);

            if (!sname.empty())
            {
                LuaScript script = scriptEngine.CreateScript("scripts/" + sname);

                scriptEngine.SetScriptVariable(script, "entity", ent);
                scriptEngine.InitScript(script);
                ent.AddComponent<LuaScript>(script);
            }
            return ent;
        });

        // scene_type.set_function("set_active", [](Scene* scene, const bool active) { scene->active = active; });

        scene_type.set_function("get_camera", &Scene::GetCamera);
        scene_type.set_function("get_entity", &Scene::GetEntity);

        // scene_type.set_function("add_terrain_physics", [](Scene* scene, const Entity ent) {
        //     if (!ent.HasComponent<Components::TerrainComp>())
        //     {
        //         VK_WARN("Entity does not have a terrain component. Can't add physics");
        //         return;
        //     }

        //    const auto& terrain = ent.GetComponent<Components::TerrainComp>();

        //    // TODO remove this
        //    //  System::BindScene(*scene);

        //    // System::Physics_AddTerrain(*terrain.terrain_ptr);
        //});

        scene_type.set_function("enable_debug",
                                [](Scene* scene, const bool enable) { scene->scenePhysics->EnableDebug(enable); });

        scene_type.set_function("serialize",
                                &Scene::Serialize); // Give it folder assets/scenes. will create subfolder for scene
        scene_type.set_function(
            "deserialize",
            &Scene::Deserialize); // needs to be given folder assets/scenes/scene_name .ie assets/scenes/Test Scene
        scene_type.set_function("get_name", &Scene::getName);
    }

} // namespace Vakol
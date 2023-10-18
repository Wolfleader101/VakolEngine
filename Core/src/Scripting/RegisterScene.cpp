#include "LuaAccess.hpp"

#include "ECS/Components.hpp"
#include "Rendering/RenderEngine.hpp"
#include "SceneManager/Scene.hpp"
#include "Scripting/ScriptEngine.hpp"

namespace Vakol
{
    void RegisterScene(sol::state& lua, ScriptEngine& scriptEngine)
    {
        auto scene_type = lua.new_usertype<Scene>("Scene");

        scene_type.set("globals", sol::property([](Scene& self) { return self.GetScript().env; }));

        scene_type.set_function("create_entity", [&](Scene* scene, const std::string tag, const std::string& path) {
            auto ent = scene->CreateEntity(tag);

            if (!path.empty())
            {
                ent.AddComponent<ScriptComp>();

                auto& scriptComp = ent.GetComponent<ScriptComp>();

                LuaScript tScript = scriptEngine.CreateScript("root", "scripts/" + path);
                scriptComp.scripts.push_back(std::move(tScript));

                LuaScript& script = scriptComp.scripts.back();

                scriptEngine.SetScriptVariable(script, "entity", ent);
                scriptEngine.InitScript(script);
            }
            return ent;
        });

        scene_type.set_function("generate_skybox", [](Scene* scene, sol::as_table_t<std::vector<std::string>>&& faces) {
            Rendering::Skybox skybox;

            Rendering::RenderEngine::GenerateSkybox(std::move(faces.value()), skybox);

            skybox.active = true;

            scene->SetSkybox(skybox);
        });

        scene_type.set_function("enable_debug", [](Scene* scene) { scene->SetDebug(true); });
        scene_type.set_function("disable_debug", [](Scene* scene) { scene->SetDebug(false); });

        scene_type.set_function("get_camera", &Scene::GetCamera);
        scene_type.set_function("get_entity", &Scene::GetEntity);

        scene_type.set_function("raycast", [](Scene& scene, Math::Vec3& origin, Math::Vec3& direction,
                                              double maxDistance, RayCastHitInfo& info) {
            PhysicsScene& physicsScene = scene.GetPhysicsScene();

            return physicsScene.RayCast(origin, direction, maxDistance, info);
        });

        scene_type.set_function("serialize",
                                &Scene::Serialize); // Give it folder assets/scenes. will create subfolder for scene
        scene_type.set_function(
            "deserialize",
            &Scene::Deserialize); // needs to be given folder assets/scenes/scene_name .ie assets/scenes/Test Scene

        scene_type.set_function("get_name", &Scene::getName);
    }

} // namespace Vakol
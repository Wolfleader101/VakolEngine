#include "Scene.hpp"

#include <Model/Components.hpp>
#include <Model/Entity.hpp>

#include "System.hpp"

namespace Vakol::Controller {
    Scene::Scene(const std::string& name, const std::string& scriptName, LuaState& lua)
        : name(name), scriptName(scriptName), lua(lua), entityList() {
        lua.RunFile("scripts/" + scriptName);

        sol::function init = lua.GetState()["init"];

        init();
    }

    void Scene::LoadScript(Entity& entity, std::string scriptName) {
        entity.AddComponent<Model::Components::Script>(scriptName);

        lua.RunFile("scripts/" + scriptName);

        sol::function init = lua.GetState()["init"];

        init();
    }

    void Scene::Update(const Controller::Time& time) {
        lua.RunFile("scripts/" + scriptName);

        sol::function update = lua.GetState()["update"];

        update();

        System::ScriptUpdate(entityList.GetRegistry(), lua);
    }
}  // namespace Vakol::Controller
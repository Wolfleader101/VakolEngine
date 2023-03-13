#include "Scene.hpp"

#include <Model/Components.hpp>
#include <Model/Entity.hpp>

#include "LuaAccess.hpp"
#include "System.hpp"
namespace Vakol::Controller {
    Scene::Scene(const std::string& name, const std::string& scriptName, LuaState& lua)
        : name(name), scriptName(scriptName), lua(lua), entityList() {
        lua.RunFile("scripts/" + scriptName);

        sol::function init = lua.GetState()["init"];

        init(*this);
    }

    void Scene::CreateEntity(const std::string scriptName) {
        auto ent = entityList.CreateEntity();
        if (scriptName.length() != 0) ent.AddComponent<Model::Components::Script>(scriptName, lua);
    }

    void Scene::Update(const Time& time) {
        lua.RunFile("scripts/" + scriptName);

        sol::function update = lua.GetState()["update"];

        update();

        System::ScriptUpdate(entityList.GetRegistry(), lua);
    }
}  // namespace Vakol::Controller
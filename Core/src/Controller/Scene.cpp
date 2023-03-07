#include "Scene.hpp"

#include <Model/Components.hpp>
#include <Model/Entity.hpp>

namespace Vakol::Controller {
    Scene::Scene(LuaState& lua) : lua(lua), entityList() {}

    void Scene::LoadScript(Entity& entity, std::string scriptName) {
        entity.AddComponent<Model::Components::Script>(scriptName);

        lua.RunFile(scriptName);

        sol::function init = lua.GetState()['init'];

        init();
    }
}  // namespace Vakol::Controller
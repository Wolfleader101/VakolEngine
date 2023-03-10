#include "System.hpp"

#include <Model/Components.hpp>

namespace Vakol::Controller::System {
    void Model_Draw(reg& registry) {
        // registry.view<Components::Transform, Components::ModelType>().each(
        //	[&](auto& trans, auto& model) {
        //		/* draw model */
        //
        //	}
        //);
    }

    void ScriptUpdate(reg& registry, LuaState& lua) {
        registry.view<Model::Components::Script>().each([&](auto& script) {
            lua.RunFile("scripts/" + script.script_name);

            sol::function update = lua.GetState()["update"];

            update();
        });
    }

}  // namespace Vakol::Controller::System

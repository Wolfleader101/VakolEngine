#include "System.hpp"

#include <Model/Components.hpp>

using namespace Vakol::Model::Components;

namespace Vakol::Controller {

    entt::registry* System::registry = nullptr;

    void System::SetEntityList(EntityList& EL) { registry = &EL.m_Registry; }

    void System::Drawable_Update() {
        registry->view<Components::Transform, Components::Drawable>().each(
            [&](auto& trans, Components::Drawable& drawable) {
                /* draw model */

                // drawable.model_ptr->.meshes;
            });
    }

    void System::Script_Update(LuaState& lua) {
        registry->view<Model::Entity, Components::Script>().each([&](auto& ent, auto& script) {
            lua.RunFile("scripts/" + script.script_name);

            sol::function update = lua.GetState()["update"];

            update(ent);
        });
    }

}  // namespace Vakol::Controller

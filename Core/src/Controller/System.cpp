#include "System.hpp"

#include <Model/Components.hpp>

using namespace Vakol::Model::Components;

namespace Vakol::Controller {

    entt::registry* System::registry = nullptr;

    void System::SetEntityList(EntityList& EL) { registry = &EL.m_Registry; }

    void System::Drawable_Update(const Time& time, const std::shared_ptr<Renderer> renderer) 
    {
        registry->view<Components::Transform, Components::Drawable>().each(
            [&](auto& trans, Components::Drawable& drawable) 
            {
                renderer->Draw(time, drawable);
            });
    }

    void System::Script_Update(LuaState& lua, EntityList& list, Scene* scene) {
        registry->view<Components::Script>().each([&](auto entity_id, auto& script) {
            lua.RunFile("scripts/" + script.script_name);

            sol::function update = lua.GetState()["update"];

            auto ent = list.GetEntity(static_cast<unsigned int>(entity_id));

            update(*scene, ent);
        });
    }

}  // namespace Vakol::Controller

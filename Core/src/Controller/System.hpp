#pragma once

#include <entt/entt.hpp>

#include "Controller/EntityList.hpp"
#include "LuaState.hpp"
#include "Scene.hpp"
#include "View/Renderer/Renderer.hpp"

namespace Vakol::Controller {

    class System {
       public:
        System() = delete;

        static void SetEntityList(EntityList& EL);

        // -- READ --
        // if you add a function use the convention Type_Action()
        // will make it more intuitive to find names of funcs

        static void Drawable_Update(const Time& time, const Controller::Camera& camera,
                                    const std::shared_ptr<View::Renderer> renderer);

        static void Script_Update(LuaState& lua, EntityList& list, Scene* scene);

       private:
        static entt::registry* registry;
    };

}  // namespace Vakol::Controller
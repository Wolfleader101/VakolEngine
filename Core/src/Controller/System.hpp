#pragma once

#include <entt/entt.hpp>

#include "Controller/EntityList.hpp"
#include "LuaState.hpp"

namespace Vakol::Controller {

    class System {
       public:
        System() = delete;

        static void SetEntityList(EntityList& EL);

        // -- READ --
        // if you add a function use the convention Type_Action()
        // will make it more intuitive to find names of funcs

        static void Drawable_Update();

        static void Script_Update(LuaState& lua);

       private:
        static entt::registry* registry;
    };

}  // namespace Vakol::Controller
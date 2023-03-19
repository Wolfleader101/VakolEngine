#pragma once

#include <Controller/EntityList.hpp>
#include <reactphysics3d/reactphysics3d.h>

#include "LuaState.hpp"

namespace Vakol::Controller {
    
    class System {

       public:

        System() = delete;

        static void SetEntityList(EntityList& EL);

        // -- READ --
        // if you add a function use the convention Type_Action()
        // will make it more intuitive to find names of funcs

        static void Model_Draw();

        static void ScriptUpdate(LuaState& lua);

        static void Physics_UpdateTransforms(float factor);

       private:
        static entt::registry* registry; 

    }; 
}  // namespace Vakol::Controller
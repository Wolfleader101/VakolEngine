#pragma once

#include <Controller/EntityList.hpp>
#include <Controller/Physics/ScenePhysics.hpp>


#include "LuaState.hpp"

namespace Vakol::Controller {
    
    class System {

       public:

        System() = delete;

        static void BindEntityList(EntityList& EL);
        


        // -- READ --
        // if you add a function use the convention Type_Action()
        // will make it more intuitive to find names of funcs

        static void Model_Draw();


        static void Script_Update(LuaState& lua);


        static void Physics_Init(ScenePhysics& SP);
        static void Physics_UpdateTransforms(float factor);

       private:
        static entt::registry* registry; 
    }; 
}  // namespace Vakol::Controller
#pragma once

#include <Controller/EntityList.hpp>
#include <Controller/Physics/ScenePhysics.hpp>
#include <Model/Components.hpp>


#include "LuaState.hpp"

namespace Vakol::Controller {
    
    using namespace Vakol::Controller::Physics;
    using namespace Vakol::Model::Components;

    class System {

       public:

        System() = delete;

        static void BindEntityList(EntityList& EL);
        


        // -- READ --
        // if you add a function use the convention Type_Action()
        // will make it more intuitive to find names of funcs

        static void Model_Draw();




        static void Script_Update(LuaState& lua);


        static void Physics_InitObject(std::shared_ptr<ScenePhysics> SP, PhysicsObject& PhyObj, const Drawable& model, Transform trans);
        static void Physics_Init(std::shared_ptr<ScenePhysics> SP);
        static void Physics_UpdateTransforms(float factor);
        static void Physics_SerializationPrep();

       private:
        static entt::registry* registry; 
    }; 
}  // namespace Vakol::Controller
#include "System.hpp"

#include <Model/Components.hpp>

#include <Model/Math/Quaternion.hpp>
#include <Model/Math/Math.hpp>

using namespace Vakol::Model::Components;

namespace Vakol::Controller 
{
    entt::registry* System::registry = nullptr;

    void System::SetEntityList(EntityList& EL) 
    { 
        registry = &EL.m_Registry;
    }

    void System::Model_Draw() {
        // registry.view<Components::Transform, Components::ModelType>().each(
        //	[&](auto& trans, auto& model) {
        //		/* draw model */
        //
        //	}
        //);
    }

    void System::ScriptUpdate(LuaState& lua) {
        registry->view<Script>().each([&](auto& script) {
            lua.RunFile("scripts/" + script.script_name);

            sol::function update = lua.GetState()["update"];

            update();
        });
    }

    void System::Physics_UpdateTransforms(float factor) 
    { 
        registry->view<Transform, PhysicsObject>().each(
        [&](auto& trans, auto& PhyObj) {


            rp3d::Transform currTransform = PhyObj.RigidBody->getTransform();
            
            // Compute the interpolated transform of the rigid body
            rp3d::Transform interpolatedTransform =
                rp3d::Transform::interpolateTransforms(PhyObj.prevTransform, currTransform, factor);
 
            PhyObj.prevTransform = currTransform;

            auto& interPos = interpolatedTransform.getPosition();
            trans.pos = glm::vec3(interPos.x, interPos.y, interPos.z);

            auto& interOrient = interpolatedTransform.getOrientation();
            trans.rot = glm::quat(interOrient.w, interOrient.x, interOrient.y, interOrient.z);
        }
        );
    }

}  // namespace Vakol::Controller

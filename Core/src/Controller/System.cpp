#include "System.hpp"

#include <Model/Components.hpp>

#include <Model/Math/Quaternion.hpp>
#include <Model/Math/Math.hpp>

using namespace Vakol::Model::Components;
using namespace Vakol::Model::Math;

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

    void PhysicsTransformUpdate(reg& registry, float factor) 
    { registry.view<Transform, PhysicsObject>().each(
        [&](Transform& trans, PhysicsObject& PhyObj) {


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

}  // namespace Vakol::Controller::System

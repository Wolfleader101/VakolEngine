#include "System.hpp"

#include <reactphysics3d/reactphysics3d.h>

#include <Model/Components.hpp>

struct Bounds
{
    rp3d::Vector3 min; /**< minimum vertice*/
    rp3d::Vector3 max; /**< Maximum vertice*/
    rp3d::Vector3 center; /**< Average of all vertices*/
    rp3d::Vector3 extents; /**< Extent of vertices*/
    rp3d::Vector3 size; /**< Size of vertices*/

    float radius; /**< Radius*/
};



using namespace Vakol::Model::Components;

Bounds getBounds(const Drawable& model);
void AddCollider(PhysicsObject& PhyObj, const Drawable& model);

namespace Vakol::Controller 
{
    entt::registry* System::registry = nullptr;

    void System::BindEntityList(EntityList& EL) 
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

    void System::Script_Update(LuaState& lua) {
        registry->view<Script>().each([&](auto& script) {
            lua.RunFile("scripts/" + script.script_name);

            sol::function update = lua.GetState()["update"];

            update();
        });
    }

    void System::Physics_Init(ScenePhysics& SP) 
    { 
        registry->view<Transform, Drawable, PhysicsObject>().each(
            [&](Transform& trans, Drawable& draw, PhysicsObject& PhyObj)
            {

                rp3d::Vector3 pos(trans.pos.x, trans.pos.y, trans.pos.z);
                rp3d::Quaternion quat = rp3d::Quaternion(trans.rot.x, trans.rot.y, trans.rot.z, trans.rot.w);
                
                PhyObj.RigidBody = SP.m_World->createRigidBody(rp3d::Transform(pos, quat));

                PhyObj.RigidBody->setMass(PhyObj.Data.mass);
                PhyObj.RigidBody->setType(PhyObj.Type);
                PhyObj.RigidBody->enableGravity(PhyObj.Data.grav);
                PhyObj.RigidBody->setAngularDamping(PhyObj.Data.ADamp);
                PhyObj.RigidBody->setLinearDamping(PhyObj.Data.LDamp);
                PhyObj.RigidBody->setAngularLockAxisFactor(PhyObj.Data.AngularLock);

                
                Bounds bounds = getBounds(draw);

                
                
                


                    
            }
        );
    }

    void System::Physics_UpdateTransforms(float factor) 
    { 
        registry->view<Transform, Components::PhysicsObject>().each(
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




Bounds getBounds(const Drawable& model)
{

    Bounds bounds;

    rp3d::Vector3& centre = bounds.center;

    rp3d::Vector3& max = bounds.max;
    rp3d::Vector3& min = bounds.min;

    auto& firstVert = model.ModelPtr->meshes.at(0).vertices.at(0);

    max = min = rp3d::Vector3(firstVert.position.x, firstVert.position.y, firstVert.position.z);

    rp3d::Vector3 tempVert;
    
    for (auto& msh : model.ModelPtr->meshes)
    {
        for (auto& vertice : msh.vertices)
        {
            tempVert.x = vertice.position.x;
            tempVert.y = vertice.position.y;
            tempVert.z = vertice.position.z;

            max = rp3d::Vector3::max(max, tempVert);
            min = rp3d::Vector3::min(min, tempVert);

        }
    }

    bounds.center = (bounds.max + bounds.min) / 2.0f;
    bounds.extents = (bounds.max - bounds.min) / 2.0f;
    bounds.size = bounds.extents * 2;
    bounds.radius = bounds.extents.length();
    
}

void AddCollider(PhysicsObject& PhyObj, const Drawable& model)
{
    Bounds bounds = getBounds(model);

    auto& shape = PhyObj.ShapeName;

    if (shape == rp3d::CollisionShapeName::BOX)
    {
        
    }

    //add collider depending on what type.
    //Then add something that is called before serialization so the data persists
}
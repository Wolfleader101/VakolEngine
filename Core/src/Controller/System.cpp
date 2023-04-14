#include "System.hpp"

#include <reactphysics3d/reactphysics3d.h>

#include <Controller/Physics/PhysicsPool.hpp>
#include <Controller/AssetLoader/AssetLoader.hpp>
#include <Controller/Scene.hpp>

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
using Scene = Vakol::Controller::Scene;

Bounds getBounds(const Drawable& model);



namespace Vakol::Controller 
{
    entt::registry* System::m_registry = nullptr;
    std::shared_ptr<ScenePhysics> System::m_SP = nullptr;


    void System::BindScene(Scene& scene)
    { 
        m_registry = &scene.entityList.m_Registry;
        m_SP = scene.scenePhysics;
    }

    void System::Model_Init()
    {
        m_registry->view<Drawable>().each(
            [&](Drawable& model)
            {
                model.ModelPtr = AssetLoader::GetModel(model.name);
            }

        );
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
        m_registry->view<Script>().each([&](auto& script) {
            lua.RunFile("scripts/" + script.script_name);

            sol::function update = lua.GetState()["update"];

            update();
        });
    }

    void System::Physics_Init() 
    { 
        m_registry->view<Transform, Drawable, PhysicsObject>().each(
            [&](Transform& trans, Drawable& draw, PhysicsObject& PhyObj)
            {
                System::Physics_InitObject(PhyObj, draw, trans);
            }
        );
    }

    void System::Physics_UpdateTransforms(float factor) 
    { 
        m_registry->view<Transform, Components::PhysicsObject>().each(
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

    void System::Physics_SerializationPrep()
    {
        auto view = m_registry->view<PhysicsObject, Transform>();
        for (auto entity : view)
        {
            auto& trans = view.get<Transform>(entity);
            auto& PhyObj = view.get<PhysicsObject>(entity);

            PhyObj.Data.mass = PhyObj.RigidBody->getMass();
            PhyObj.Data.grav = PhyObj.RigidBody->isGravityEnabled();
            PhyObj.Data.ADamp = PhyObj.RigidBody->getAngularDamping();
            PhyObj.Data.LDamp = PhyObj.RigidBody->getLinearDamping();
            PhyObj.Data.AngularLock = PhyObj.RigidBody->getAngularLockAxisFactor();
            PhyObj.Data.Orientation = PhyObj.RigidBody->getTransform().getOrientation().getVectorV();

            PhyObj.Type = PhyObj.RigidBody->getType();

            rp3d::Vector3 pos(trans.pos.x, trans.pos.y, trans.pos.z);
            rp3d::Quaternion quat = rp3d::Quaternion(trans.rot.x, trans.rot.y, trans.rot.z, trans.rot.w);

            PhyObj.prevTransform = rp3d::Transform(pos, quat);
        }
        
    }


    void System::Physics_InitObject(PhysicsObject& PhyObj, const Drawable& draw, Transform trans)
    {
        rp3d::Vector3 pos(trans.pos.x, trans.pos.y, trans.pos.z);
        rp3d::Quaternion quat = rp3d::Quaternion(trans.rot.x, trans.rot.y, trans.rot.z, trans.rot.w);

        //rp3d::Transform rpTrans = rp3d::Transform(pos, quat);

        rp3d::Transform rpTrans = rp3d::Transform::identity();

        PhyObj.owningWorld = m_SP;

        PhyObj.RigidBody = m_SP->m_World->createRigidBody(rpTrans);

        PhyObj.RigidBody->setMass(PhyObj.Data.mass);
        PhyObj.RigidBody->setType(PhyObj.Type);
        PhyObj.RigidBody->enableGravity(PhyObj.Data.grav);
        PhyObj.RigidBody->setAngularDamping(PhyObj.Data.ADamp);
        PhyObj.RigidBody->setLinearDamping(PhyObj.Data.LDamp);
        PhyObj.RigidBody->setAngularLockAxisFactor(PhyObj.Data.AngularLock);


        Bounds bounds = getBounds(draw);

        if (PhyObj.ShapeName == rp3d::CollisionShapeName::BOX)
        {
            PhyObj.Shape = PhysicsPool::m_Common.createBoxShape((bounds.extents) * rp3d::Vector3(trans.scale.x, trans.scale.y, trans.scale.z));
        }
        else if (PhyObj.ShapeName == rp3d::CollisionShapeName::SPHERE)
        {
            PhyObj.Shape = PhysicsPool::m_Common.createSphereShape(bounds.radius * trans.scale.x);
        }
        else if (PhyObj.ShapeName == rp3d::CollisionShapeName::CAPSULE)
        {
            PhyObj.Shape = PhysicsPool::m_Common.createCapsuleShape(bounds.extents.x * trans.scale.x, bounds.extents.y * trans.scale.y);
        }
        else if (PhyObj.ShapeName == rp3d::CollisionShapeName::TRIANGLE_MESH)
        {
            auto MeshPtr = PhysicsPool::m_Common.createTriangleMesh();

            for (auto mesh : draw.ModelPtr->meshes)
            {
                rp3d::TriangleVertexArray* triArray = nullptr;

                triArray = new rp3d::TriangleVertexArray(

                    mesh.vertices.size(), mesh.vertices.data(), sizeof(float) * 3,
                    mesh.indices.size() / 3, mesh.indices.data(), sizeof(unsigned int) * 3,
                    rp3d::TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
                    rp3d::TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE
                );

                MeshPtr->addSubpart(triArray);

            };

            PhyObj.Shape = PhysicsPool::m_Common.createConcaveMeshShape(MeshPtr, rp3d::Vector3(trans.scale.x, trans.scale.y, trans.scale.z));

        }

        PhyObj.CollisionBody = PhyObj.RigidBody->addCollider(PhyObj.Shape, rpTrans);
    };

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

    return bounds;
    
}


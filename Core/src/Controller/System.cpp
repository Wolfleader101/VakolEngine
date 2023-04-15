#include "System.hpp"

#include <functional>

#include <reactphysics3d/reactphysics3d.h>

#include <Controller/Physics/PhysicsPool.hpp>
#include <Controller/AssetLoader/AssetLoader.hpp>
#include <Controller/Scene.hpp>

#include <Model/Components.hpp>


using namespace Vakol::Model::Components;
using Scene = Vakol::Controller::Scene;


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

    void System::Physics_Init() //not pretty be we ball
    {
        m_registry->view<RigidBody, Collider, Drawable, Transform>().each( //for entities that have a model, use model for bounds
            [&](RigidBody& rigid, Collider& collider, Drawable& draw, Transform& trans)
            {
                collider.bounds = Components::getBounds(draw);
                System::Physics_InitObject(rigid, collider, draw, trans);
            }
        );

        m_registry->view<RigidBody, Collider, Transform>().each( //will use manual bounds set. 
            [&](RigidBody& rigid, Collider& collider, Transform& trans)
            {
                System::Physics_InitObject(rigid, collider, std::nullopt, trans);
            }
        );
    }

    void System::Physics_UpdateTransforms(float factor) 
    { 
        m_registry->view<Transform, RigidBody>().each(
        [&](auto& trans, auto& rigid) {


            rp3d::Transform currTransform = rigid.RigidBodyPtr->getTransform();
            
            // Compute the interpolated transform of the rigid body
            rp3d::Transform interpolatedTransform =
                rp3d::Transform::interpolateTransforms(rigid.prevTransform, currTransform, factor);
 
            rigid.prevTransform = currTransform;

            auto& interPos = interpolatedTransform.getPosition();
            trans.pos = glm::vec3(interPos.x, interPos.y, interPos.z);

            auto& interOrient = interpolatedTransform.getOrientation();
            trans.rot = glm::quat(interOrient.w, interOrient.x, interOrient.y, interOrient.z);
        }
        );
    }

    void System::Physics_SerializationPrep()
    {
        m_registry->view<RigidBody, Transform>().each( //can deduce that a collider can't exist without a rigidbody
            [&](RigidBody& rigid, Transform& trans)
            {
                
                if (rigid.RigidBodyPtr) {
                    rigid.Data.mass = rigid.RigidBodyPtr->getMass();
                    rigid.Data.grav = rigid.RigidBodyPtr->isGravityEnabled();
                    rigid.Data.ADamp = rigid.RigidBodyPtr->getAngularDamping();
                    rigid.Data.LDamp = rigid.RigidBodyPtr->getLinearDamping();
                    rigid.Data.AngularLock = rigid.RigidBodyPtr->getAngularLockAxisFactor();
                    rigid.Data.Orientation = rigid.RigidBodyPtr->getTransform().getOrientation().getVectorV();

                    rigid.Type = rigid.RigidBodyPtr->getType();


                    rp3d::Vector3 pos(trans.pos.x, trans.pos.y, trans.pos.z);
                    rp3d::Quaternion quat = rp3d::Quaternion(trans.rot.x, trans.rot.y, trans.rot.z, trans.rot.w);

                    rigid.prevTransform = rp3d::Transform(pos, quat);
                }
            }
        );
    }


    void System::Physics_InitObject(RigidBody& rigid, 
                                    std::optional<std::reference_wrapper<Collider>> collider,
                                    std::optional<std::reference_wrapper<Drawable>> model, //only used if you want to put triangle mesh
                                    const Transform& trans)
    {
        rp3d::Vector3 pos(trans.pos.x, trans.pos.y, trans.pos.z);
        

        rp3d::Transform rpTrans = rp3d::Transform(pos, rp3d::Quaternion::fromEulerAngles({trans.rot.x, trans.rot.y, trans.rot.z}));


        rigid.owningWorld = m_SP;

        rigid.RigidBodyPtr = m_SP->m_World->createRigidBody(rpTrans);

        rigid.RigidBodyPtr->setMass(rigid.Data.mass);
        rigid.RigidBodyPtr->setType(rigid.Type);
        rigid.RigidBodyPtr->enableGravity(rigid.Data.grav);
        rigid.RigidBodyPtr->setAngularDamping(rigid.Data.ADamp);
        rigid.RigidBodyPtr->setLinearDamping(rigid.Data.LDamp);
        rigid.RigidBodyPtr->setAngularLockAxisFactor(rigid.Data.AngularLock);

        if (collider.has_value())
        {
            Collider& col = collider.value();
            col.OwningBody = &rigid;

            Collider::Bounds& bounds = collider.value().get().bounds;

            if (col.ShapeName == rp3d::CollisionShapeName::BOX)
            {
                col.Shape = PhysicsPool::m_Common.createBoxShape((bounds.extents) * rp3d::Vector3(trans.scale.x, trans.scale.y, trans.scale.z));
            }
            else if (col.ShapeName == rp3d::CollisionShapeName::SPHERE)
            {
                col.Shape = PhysicsPool::m_Common.createSphereShape(bounds.radius * trans.scale.x);
            }
            else if (col.ShapeName == rp3d::CollisionShapeName::CAPSULE)
            {
                col.Shape = PhysicsPool::m_Common.createCapsuleShape(bounds.extents.x * trans.scale.x, bounds.extents.y * trans.scale.y);
            }
            else if (col.ShapeName == rp3d::CollisionShapeName::TRIANGLE_MESH)
            {
                if (!model.has_value())
                {
                    VK_CRITICAL("Trying to add triangle mesh collider without providing model!");
                    assert(0);
                }
                Drawable& draw = model.value();

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

                col.Shape = PhysicsPool::m_Common.createConcaveMeshShape(MeshPtr, rp3d::Vector3(trans.scale.x, trans.scale.y, trans.scale.z));

            }
            else
            {
                VK_CRITICAL("Failed Collider Initialization! No collider shape given.");
                assert(0);
            }

            col.ColliderPtr = rigid.RigidBodyPtr->addCollider(col.Shape, rpTrans);
            col.OwningBody = &rigid;
        }

        rigid.initialized = true;
    };

}  // namespace Vakol::Controller




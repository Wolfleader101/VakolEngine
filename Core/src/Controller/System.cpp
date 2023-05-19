#include "System.hpp"

#include <reactphysics3d/reactphysics3d.h>

#include <Controller/AssetLoader/AssetLoader.hpp>
#include <Controller/Physics/PhysicsPool.hpp>
#include <Controller/Scene.hpp>
#include <Model/Components.hpp>

#pragma warning(push)
#pragma warning(disable:4201)
#include <glm/gtc/quaternion.hpp>
#pragma warning(pop)

using namespace Components;

namespace Vakol::Controller
{
    entt::registry* System::m_registry = nullptr;
    std::shared_ptr<ScenePhysics> System::m_SP = nullptr;
    Controller::EntityList* System::Entlist = nullptr;

    void System::BindScene(Scene& scene) {
        m_registry = &scene.entityList.m_Registry;
        m_SP = scene.scenePhysics;
        Entlist = &scene.entityList;
    }

    void System::Drawable_Update(const Time& time, const Controller::Camera& camera,
                                 const std::shared_ptr<View::Renderer> renderer) {
        m_registry->view<Components::Transform, Components::Drawable>().each(
            [&](auto& trans, Components::Drawable& drawable) { renderer->Draw(time, camera, trans, drawable); });
    }

    void System::Script_Update(LuaState& lua, EntityList& list, Scene* scene) {
        m_registry->view<Components::Script>().each([&](auto entity_id, auto& script) {
            lua.RunFile("scripts/" + script.script_name);

            sol::function update = lua.GetState()["update"];

            auto ent = list.GetEntity(static_cast<unsigned int>(entity_id));

            update(*scene, ent);
        });
    }

    void System::Physics_Init()  
    {
        auto view = m_registry->view<Components::RigidBody>();

        for (auto entity : view) {
            Physics_InitEntity(Entlist->GetEntity((uint32_t) entity));
        }
    }

    void System::Physics_UpdateTransforms(float factor) {
        m_registry->view<Transform, RigidBody>().each([&](auto& trans, auto& rigid) {
            rp3d::Transform curr_transform = rigid.RigidBodyPtr->getTransform();

            // Compute the interpolated transform of the rigid body
            const rp3d::Transform interpolatedTransform =
                rp3d::Transform::interpolateTransforms(rigid.prevTransform, curr_transform, factor);

            rigid.prevTransform = curr_transform;

            auto& interPos = interpolatedTransform.getPosition();
            trans.pos = glm::vec3(interPos.x, interPos.y, interPos.z);

            auto& rp3dQuat = interpolatedTransform.getOrientation();

            glm::quat glmQuat(rp3dQuat.w, rp3dQuat.x, rp3dQuat.y, rp3dQuat.z);

            trans.rot = glm::eulerAngles(glmQuat);
        });
    }

    void System::Physics_SerializationPrep() {
        m_registry->view<RigidBody, Transform>().each(  // can deduce that a collider can't exist without a rigidbody
            [&](RigidBody& rigid, const Transform& trans) {
                if (rigid.RigidBodyPtr) {
                    rigid.Data.mass = rigid.RigidBodyPtr->getMass();
                    rigid.Data.grav = rigid.RigidBodyPtr->isGravityEnabled();
                    rigid.Data.ADamp = rigid.RigidBodyPtr->getAngularDamping();
                    rigid.Data.LDamp = rigid.RigidBodyPtr->getLinearDamping();
                    rigid.Data.AngularLock = rigid.RigidBodyPtr->getAngularLockAxisFactor();
                    rigid.Data.Orientation = rigid.RigidBodyPtr->getTransform().getOrientation().getVectorV();
                    
                    
                    rigid.Type = static_cast<RigidBody::BODY_TYPE>(rigid.RigidBodyPtr->getType());

                    const rp3d::Vector3 pos(trans.pos.x, trans.pos.y, trans.pos.z);
                    const rp3d::Quaternion quat = rp3d::Quaternion::fromEulerAngles(trans.rot.x, trans.rot.y, trans.rot.z);

                    rigid.prevTransform = rp3d::Transform(pos, quat);
                }
            });
    }

    void System::Physics_InitEntity(Entity& ent) 
    {
        auto& trans = ent.GetComponent<Transform>();
        auto& rigid = ent.GetComponent<RigidBody>();

        if (rigid.initialized) return;

        if(!ent.HasComponent<RigidBody>())
        {
            VK_CRITICAL("No rigid body component found on entity: {0}", ent.GetHandle());
            assert(0);
            return;
        }

        const rp3d::Vector3 pos(trans.pos.x, trans.pos.y, trans.pos.z);

        const auto rpTrans = rp3d::Transform(pos, rp3d::Quaternion::fromEulerAngles({trans.rot.x, trans.rot.y, trans.rot.z}));

        rigid.owningWorld = m_SP;

        if (rigid.RigidBodyPtr) {
            m_SP->m_World->destroyCollisionBody(rigid.RigidBodyPtr);
        }

        rigid.RigidBodyPtr = m_SP->m_World->createRigidBody(rpTrans);

        rigid.SetRigidData(rigid.Data);

        rigid.prevTransform = rpTrans;

        if(ent.HasComponent<Collider>())
        {
            auto& col = ent.GetComponent<Collider>();

            col.OwningBody = &rigid;

            const Collider::Bounds& bounds = col.bounds;

            if (col.ShapeName == Collider::ShapeName::BOX) 
            {
                col.Shape = PhysicsPool::m_Common.createBoxShape(
                    (bounds.extents) * rp3d::Vector3(trans.scale.x, trans.scale.y, trans.scale.z));
            }
            else if (col.ShapeName == Collider::ShapeName::SPHERE) 
            {
                col.Shape = PhysicsPool::m_Common.createSphereShape(bounds.radius * trans.scale.x);
            } 
            else if (col.ShapeName == Collider::ShapeName::CAPSULE) 
            {
                col.Shape = PhysicsPool::m_Common.createCapsuleShape(bounds.extents.x * trans.scale.x,
                                                                    bounds.extents.y * trans.scale.y);
            } 
            else if (col.ShapeName == Collider::ShapeName::TRIANGLE_MESH) 
            {

                if (!ent.HasComponent<Drawable>()) {
                    VK_CRITICAL("Trying to add triangle mesh collider without providing model!");
                    assert(0);
                }

                const auto& draw = ent.GetComponent<Drawable>();

                const auto mesh_ptr = PhysicsPool::m_Common.createTriangleMesh();

                for (auto& mesh : draw.model_ptr->meshes()) 
                {
                    const auto tri_array = new rp3d::TriangleVertexArray(
	                    mesh.nVertices(), mesh.vertices().data(),
	                    sizeof(float) * 3, mesh.nIndices() / 3,
	                    mesh.indices().data(), sizeof(unsigned int) * 3,
	                    rp3d::TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
	                    rp3d::TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE);

                    mesh_ptr->addSubpart(tri_array);
                };

                col.Shape = PhysicsPool::m_Common.createConcaveMeshShape(
                    mesh_ptr, rp3d::Vector3(trans.scale.x, trans.scale.y, trans.scale.z));

            } else {
                VK_CRITICAL("Failed Collider Initialization! No collider shape given.");
                assert(0);
            }

            col.ColliderPtr = rigid.RigidBodyPtr->addCollider(col.Shape, rp3d::Transform::identity());
            col.OwningBody = &rigid;
	        
	    }

        rigid.initialized = true;
    };

    void System::Physics_AddTerrain(const Terrain& ter) { m_SP->AddTerrain(ter); }

}  // namespace Vakol::Controller

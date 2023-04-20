#include "System.hpp"

#include <reactphysics3d/reactphysics3d.h>

#include <Controller/AssetLoader/AssetLoader.hpp>
#include <Controller/Physics/PhysicsPool.hpp>
#include <Controller/Scene.hpp>
#include <Model/Components.hpp>
#include <functional>

using namespace Vakol::Model::Components;

namespace Vakol::Controller {

    entt::registry* System::m_registry = nullptr;
    std::shared_ptr<ScenePhysics> System::m_SP = nullptr;

    void System::BindScene(Scene& scene) {
        m_registry = &scene.entityList.m_Registry;
        m_SP = scene.scenePhysics;
    }

    void System::Drawable_Init() {
        m_registry->view<Drawable>().each(
            [&](Drawable& drawable) { drawable.model_ptr = AssetLoader::GetModel(drawable.name); }

        );
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

    void System::Physics_Init()  // not pretty be we ball
    {
        m_registry->view<RigidBody, Collider, Drawable, Transform>()
            .each(  // for entities that have a model, use model for bounds
                [&](RigidBody& rigid, Collider& collider, Drawable& draw, Transform& trans) {
                    collider.bounds = Components::getBounds(draw);
                    System::Physics_InitObject(rigid, collider, draw, trans);
                });

        m_registry->view<RigidBody, Collider, Transform>().each(  // will use manual bounds set.
            [&](RigidBody& rigid, Collider& collider, Transform& trans) {
                System::Physics_InitObject(rigid, collider, std::nullopt, trans);
            });
    }

    void System::Physics_UpdateTransforms(float factor) {
        m_registry->view<Transform, RigidBody>().each([&](auto& trans, auto& rigid) {
            rp3d::Transform currTransform = rigid.RigidBodyPtr->getTransform();

            // Compute the interpolated transform of the rigid body
            rp3d::Transform interpolatedTransform =
                rp3d::Transform::interpolateTransforms(rigid.prevTransform, currTransform, factor);

            rigid.prevTransform = currTransform;

            auto& interPos = interpolatedTransform.getPosition();
            trans.pos = glm::vec3(interPos.x, interPos.y, interPos.z);

            auto& rp3dQuat = interpolatedTransform.getOrientation();

            glm::quat glmQuat(rp3dQuat.w, rp3dQuat.x, rp3dQuat.y, rp3dQuat.z);

            trans.rot = glm::vec3(glm::eulerAngles(glmQuat));
        });
    }

    void System::Physics_SerializationPrep() {
        m_registry->view<RigidBody, Transform>().each(  // can deduce that a collider can't exist without a rigidbody
            [&](RigidBody& rigid, Transform& trans) {
                if (rigid.RigidBodyPtr) {
                    rigid.Data.mass = rigid.RigidBodyPtr->getMass();
                    rigid.Data.grav = rigid.RigidBodyPtr->isGravityEnabled();
                    rigid.Data.ADamp = rigid.RigidBodyPtr->getAngularDamping();
                    rigid.Data.LDamp = rigid.RigidBodyPtr->getLinearDamping();
                    rigid.Data.AngularLock = rigid.RigidBodyPtr->getAngularLockAxisFactor();
                    rigid.Data.Orientation = rigid.RigidBodyPtr->getTransform().getOrientation().getVectorV();

                    rigid.Type = rigid.RigidBodyPtr->getType();

                    rp3d::Vector3 pos(trans.pos.x, trans.pos.y, trans.pos.z);
                    rp3d::Quaternion quat = rp3d::Quaternion::fromEulerAngles(trans.rot.x, trans.rot.y, trans.rot.z);

                    rigid.prevTransform = rp3d::Transform(pos, quat);
                }
            });
    }

    void System::Physics_InitObject(
        RigidBody& rigid, std::optional<std::reference_wrapper<Collider>> collider,
        std::optional<std::reference_wrapper<Drawable>> model,  // only used if you want to put triangle mesh
        const Transform& trans) {
        rp3d::Vector3 pos(trans.pos.x, trans.pos.y, trans.pos.z);

        rp3d::Transform rpTrans =
            rp3d::Transform(pos, rp3d::Quaternion::fromEulerAngles({trans.rot.x, trans.rot.y, trans.rot.z}));

        rigid.owningWorld = m_SP;

        rigid.RigidBodyPtr = m_SP->m_World->createRigidBody(rpTrans);

        rigid.RigidBodyPtr->setMass(rigid.Data.mass);
        rigid.RigidBodyPtr->setType(rigid.Type);
        rigid.RigidBodyPtr->enableGravity(rigid.Data.grav);
        rigid.RigidBodyPtr->setAngularDamping(rigid.Data.ADamp);
        rigid.RigidBodyPtr->setLinearDamping(rigid.Data.LDamp);
        rigid.RigidBodyPtr->setAngularLockAxisFactor(rigid.Data.AngularLock);

        rigid.prevTransform = rpTrans;

        if (collider.has_value()) {
            Collider& col = collider.value();
            col.OwningBody = &rigid;

            Collider::Bounds& bounds = collider.value().get().bounds;

            if (col.ShapeName == rp3d::CollisionShapeName::BOX) {
                col.Shape = PhysicsPool::m_Common.createBoxShape(
                    (bounds.extents) * rp3d::Vector3(trans.scale.x, trans.scale.y, trans.scale.z));
            } else if (col.ShapeName == rp3d::CollisionShapeName::SPHERE) {
                col.Shape = PhysicsPool::m_Common.createSphereShape(bounds.radius * trans.scale.x);
            } else if (col.ShapeName == rp3d::CollisionShapeName::CAPSULE) {
                col.Shape = PhysicsPool::m_Common.createCapsuleShape(bounds.extents.x * trans.scale.x,
                                                                     bounds.extents.y * trans.scale.y);
            } else if (col.ShapeName == rp3d::CollisionShapeName::TRIANGLE_MESH) {
                if (!model.has_value()) {
                    VK_CRITICAL("Trying to add triangle mesh collider without providing model!");
                    assert(0);
                }
                Drawable& draw = model.value();

                auto MeshPtr = PhysicsPool::m_Common.createTriangleMesh();

                for (auto& mesh : draw.model_ptr->GetMeshes()) {
                    rp3d::TriangleVertexArray* triArray = nullptr;

                    triArray = new rp3d::TriangleVertexArray(
                        mesh.GetVertexArray()->GetVertexCount(), mesh.GetVertexArray()->GetVertices().data(), sizeof(float) * 3,
                        mesh.GetVertexArray()->GetIndexCount() / 3, mesh.GetVertexArray()->GetIndices().data(), sizeof(unsigned int) * 3,
                        rp3d::TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
                        rp3d::TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE);

                    MeshPtr->addSubpart(triArray);
                };

                col.Shape = PhysicsPool::m_Common.createConcaveMeshShape(
                    MeshPtr, rp3d::Vector3(trans.scale.x, trans.scale.y, trans.scale.z));

            } else {
                VK_CRITICAL("Failed Collider Initialization! No collider shape given.");
                assert(0);
            }

            col.ColliderPtr = rigid.RigidBodyPtr->addCollider(col.Shape, rpTrans);
            col.OwningBody = &rigid;
        }

        rigid.initialized = true;
    };

}  // namespace Vakol::Controller

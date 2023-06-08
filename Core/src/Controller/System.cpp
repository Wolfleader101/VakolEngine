#include "System.hpp"

#include <reactphysics3d/reactphysics3d.h>

#include <Controller/AssetLoader/AssetLoader.hpp>
#include <Controller/Physics/PhysicsPool.hpp>
#include <Controller/Scene.hpp>
#include <Model/Components.hpp>

#include <glm/gtc/quaternion.hpp>

using namespace Components;

static std::unordered_map<std::string, Components::Animator> s_animator_map;
static std::set<int> s_animation_set;

glm::vec3 to_glm(const rp3d::Vector3& v) { return {v.x, v.y, v.z}; }
glm::quat to_glm(const rp3d::Quaternion& q) { return {q.w, q.x, q.y, q.z}; }

rp3d::Vector3 to_rp3d(const glm::vec3& v) { return {v.x, v.y, v.z}; }
rp3d::Quaternion to_rp3d(const glm::quat& q) { return {q.x, q.y, q.z, q.w}; }

namespace Vakol::Controller {
    entt::registry* System::m_registry = nullptr;
    std::shared_ptr<ScenePhysics> System::m_SP = nullptr;
    EntityList* System::Entlist = nullptr;

    void System::BindScene(Scene& scene) {
        m_registry = &scene.entityList.m_Registry;
        m_SP = scene.scenePhysics;
        Entlist = &scene.entityList;
    }

    void System::Drawable_Init() {
        Terrain_Init();
        m_registry->view<Drawable>().each([&](auto& drawable) 
        { 
            if(drawable.model_ptr == nullptr)
                drawable.model_ptr = AssetLoader::GetModel(drawable.name, drawable.scale, drawable.animated, drawable.backfaceCull).first;
        });
    }

    void System::Terrain_Init() {
        m_registry->view<Drawable, Components::Terrain>().each([&](auto& drawable, auto& terrainComp) {
            std::shared_ptr<Terrain> terrain = AssetLoader::GetTerrain(terrainComp.name);

            if (!terrain) {
                terrain = AssetLoader::GetTerrain(terrainComp.name, terrainComp.path, terrainComp.min, terrainComp.max);
            }

            drawable.model_ptr = terrain->GetModel();
            terrainComp.terrain_ptr = terrain;
        });
    }

    void System::Drawable_Update(const Time& time, const std::shared_ptr<View::Renderer>& renderer) {
        m_registry->view<Transform, Drawable>().each([&](auto& transform, const Drawable& drawable) {
            auto euler_rads = glm::radians(transform.eulerAngles);

            transform.rot = glm::quat(euler_rads);

            if (!drawable.animated) renderer->Draw(transform, drawable);
        });

        m_registry->view<Components::Animator>().each([&](const Components::Animator& animator) {
            s_animator_map[animator.attached_model] = animator;
            
            for (const auto state : s_animation_set)
                s_animator_map.at(animator.attached_model).Update(state, time.deltaTime);
        });

        m_registry->view<Transform, Drawable, Components::Animation>().each([&](const auto& transform, const Drawable& drawable, const Components::Animation& animation)
        {
            s_animation_set.emplace(animation.state);

            renderer->DrawAnimated(transform, drawable, s_animator_map.at(animation.attached_model).c_animation(animation.state));
        });
    }

    void System::Script_Update(std::shared_ptr<LuaState> lua, EntityList& list, Scene* scene) {
        m_registry->view<Script>().each([&](auto entity_id, auto& script) {
            lua->RunFile("scripts/" + script.script_name);

            lua->GetState()["scene"] = scene;
            lua->GetState()["entity"] = list.GetEntity(static_cast<unsigned int>(entity_id));
            lua->GetState()["state"] = script.state;

            lua->RunFunction("update");
        });
    }

    void System::Physics_Init() {
        const auto view = m_registry->view<RigidBody>();

        for (auto entity : view) {
            auto&& ent = Entlist->GetEntity(static_cast<uint32_t>(entity));
            Physics_InitEntity(ent);
        }
    }

    void System::Physics_UpdateTransforms(const float factor) {
        m_registry->group<Transform, RigidBody>().each([&](Transform& trans, RigidBody& rigid) {
            if (rigid.Type == RigidBody::BODY_TYPE::STATIC) return;

            rp3d::Transform curr_transform = rigid.RigidBodyPtr->getTransform();

            if (rigid.use_transform && !rigid.is_colliding) {
                const auto pos = to_rp3d(trans.pos);
                const auto rot = to_rp3d(trans.rot);

                rp3d::Transform newTrans(pos, rot);
                rigid.RigidBodyPtr->setTransform(newTrans);
                curr_transform = newTrans;
            }

            // Compute the interpolated transform of the rigid body
            const rp3d::Transform interpolatedTransform =
                rp3d::Transform::interpolateTransforms(rigid.prevTransform, curr_transform, factor);

            rigid.prevTransform = curr_transform;

            trans.pos = to_glm(interpolatedTransform.getPosition());
            trans.rot = to_glm(interpolatedTransform.getOrientation());

            rigid.is_colliding = false;
        });
    }

    void System::Physics_SerializationPrep() {
        m_registry->group<RigidBody, Transform>().each(  // can deduce that a collider can't exist without a rigidbody
            [&](RigidBody& rigid, const Transform& trans) {
                if (rigid.RigidBodyPtr) {
                    rigid.Data.mass = rigid.RigidBodyPtr->getMass();
                    rigid.Data.grav = rigid.RigidBodyPtr->isGravityEnabled();
                    rigid.Data.ADamp = rigid.RigidBodyPtr->getAngularDamping();
                    rigid.Data.LDamp = rigid.RigidBodyPtr->getLinearDamping();
                    rigid.Data.AngularLock = rigid.RigidBodyPtr->getAngularLockAxisFactor();
                    rigid.Data.Orientation = rigid.RigidBodyPtr->getTransform().getOrientation().getVectorV();

                    rigid.Type = static_cast<RigidBody::BODY_TYPE>(rigid.RigidBodyPtr->getType());

                    const auto pos = to_rp3d(trans.pos);
                    const auto rot = to_rp3d(trans.rot);

                    rigid.prevTransform = rp3d::Transform(pos, rot);
                }
            });
    }

    void System::Physics_InitEntity(const Entity& ent) {
        const auto& trans = ent.GetComponent<Transform>();
        auto& rigid = ent.GetComponent<RigidBody>();

        if (rigid.initialized) return;

        if (!ent.HasComponent<RigidBody>()) {
            VK_CRITICAL("No rigid body component found on entity: {0}", ent.GetHandle());
            assert(0);
            return;
        }

        const auto pos = to_rp3d(trans.pos);

        const auto rpTrans =
            rp3d::Transform(pos, rp3d::Quaternion::fromEulerAngles(to_rp3d(radians(trans.eulerAngles))));

        rigid.owningWorld = m_SP;

        if (rigid.RigidBodyPtr) {
            m_SP->m_World->destroyCollisionBody(rigid.RigidBodyPtr);
        }

        rigid.RigidBodyPtr = m_SP->m_World->createRigidBody(rpTrans);

        rigid.SetRigidData(rigid.Data);

        rigid.prevTransform = rpTrans;

        if (ent.HasComponent<Collider>()) {
            auto& col = ent.GetComponent<Collider>();

            col.OwningBody = &rigid;

            const Collider::Bounds& bounds = col.bounds;

            if (col.ShapeName == Collider::ShapeName::BOX) {
                col.Shape = PhysicsPool::m_Common.createBoxShape(
                    (bounds.extents) * rp3d::Vector3(trans.scale.x, trans.scale.y, trans.scale.z));
            } else if (col.ShapeName == Collider::ShapeName::SPHERE) {
                col.Shape = PhysicsPool::m_Common.createSphereShape(bounds.radius * trans.scale.x);
            } else if (col.ShapeName == Collider::ShapeName::CAPSULE) {
                col.Shape = PhysicsPool::m_Common.createCapsuleShape(bounds.extents.x * trans.scale.x,
                                                                     bounds.extents.y * trans.scale.y);
            } else if (col.ShapeName == Collider::ShapeName::TRIANGLE_MESH) {
                if (!ent.HasComponent<Drawable>()) {
                    VK_CRITICAL("Trying to add triangle mesh collider without providing model!");
                    assert(0);
                }

                const auto& draw = ent.GetComponent<Drawable>();

                const auto mesh_ptr = PhysicsPool::m_Common.createTriangleMesh();

                for (auto& mesh : draw.model_ptr->meshes()) {
                    const auto tri_array = new rp3d::TriangleVertexArray(
                        mesh.nVertices(), mesh.vertices().data(), sizeof(float) * 3, mesh.nIndices() / 3,
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

        rigid.RigidBodyPtr->setUserData(static_cast<void*>(&rigid));

        rigid.initialized = true;
    };

    void System::Physics_AddTerrain(const Terrain& ter) { m_SP->AddTerrain(ter); }

    // void System::Script_Init()
    // {

    // }

}  // namespace Vakol::Controller

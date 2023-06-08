#include "System.hpp"

#include <reactphysics3d/reactphysics3d.h>

#include <Controller/AssetLoader/AssetLoader.hpp>
#include <Controller/Physics/PhysicsPool.hpp>
#include <Controller/Scene.hpp>
#include <Model/Components.hpp>

#pragma warning(push)
#pragma warning(disable : 4201)
#include <glm/gtc/quaternion.hpp>
#pragma warning(pop)

static std::set<int> s_unique;

using namespace Components;

glm::vec3 to_glm(const rp3d::Vector3& v) { return {v.x, v.y, v.z}; }
glm::quat to_glm(const rp3d::Quaternion& q) { return {q.w, q.x, q.y, q.z}; }

rp3d::Vector3 to_rp3d(const glm::vec3& v) { return {v.x, v.y, v.z}; }
rp3d::Quaternion to_rp3d(const glm::quat& q) { return {q.x, q.y, q.z, q.w}; }

namespace Vakol::Controller 
{
    entt::registry* System::m_registry = nullptr;
    std::shared_ptr<ScenePhysics> System::m_SP = nullptr;
    EntityList* System::Entlist = nullptr;

    void System::BindScene(Scene& scene) {
        m_registry = &scene.entityList.m_Registry;
        m_SP = scene.scenePhysics;
        Entlist = &scene.entityList;
    }

    void System::Drawable_Init()
    {
        Terrain_Init();
        m_registry->view<Drawable>().each([&](auto& drawable) 
        { 
            if(drawable.model_ptr == nullptr)
                drawable.model_ptr = AssetLoader::GetModel(drawable.name, drawable.scale, drawable.animated, drawable.backfaceCull).first;

        });
    }

    void System::Terrain_Init()
    {
        m_registry->view<Drawable, Components::Terrain>().each([&](auto& drawable, auto& terrainComp) 
        {
            std::shared_ptr<Terrain> terrain = AssetLoader::GetTerrain(terrainComp.name);

            if(!terrain)
            {
                terrain = AssetLoader::GetTerrain(terrainComp.name, terrainComp.path, terrainComp.min, terrainComp.max);
            }

            drawable.model_ptr = terrain->GetModel();
            terrainComp.terrain_ptr = terrain;
        });
        
    }

    void System::Unique_Search() 
    {
    }

    void System::Drawable_Update(const Time& time, const std::shared_ptr<View::Renderer>& renderer)
    {
        m_registry->view<Transform, Drawable>().each([&](auto& transform, const Drawable& drawable) 
        {
            auto euler_rads = glm::radians(transform.eulerAngles);

            transform.rot = glm::quat(euler_rads);

            if (!drawable.animated) renderer->Draw(transform, drawable);
        });

        auto animation_view = m_registry->view<Transform, Drawable, Components::Animation>();

        m_registry->view<Components::Animator>().each([&](Components::Animator& animator)
        {
            for (const auto state : s_unique)
                animator.Update(state, time.deltaTime);

            for (auto _entity : animation_view)
            {
                const auto& transform = animation_view.get<Transform>(_entity);
                const auto& drawable = animation_view.get<Drawable>(_entity);
                const auto& animation = animation_view.get<Components::Animation>(_entity);

                if (s_unique.insert(animation.state).second) continue;

                renderer->DrawAnimated(transform, drawable, animator.animation(animation.state));
            }         
        });

        // This works fine, just non-performant (I WANT SPEEEED)
        // 
        //m_registry->view<Components::Animator>().each([&](Components::Animator& animator)
        //{
        //    for (const auto state : s_unique)
        //        animator.Update(state, time.deltaTime);

        //    m_registry->view<Transform, Drawable, Components::Animation>().each([&](const auto& transform, const Drawable& drawable, const Components::Animation& _animation) 
        //    {
        //        if (!s_unique.insert(_animation.state).second) continue;

        //        renderer->DrawAnimated(transform, drawable, animator.animation(_animation.state));
        //    });
        //});
    }


    void System::Script_Init(std::shared_ptr<LuaState> lua, EntityList& list, Scene* scene)
    {
        m_registry->view<Script>().each([&](auto entity_id, auto& script) 
        {
            lua->RunFile("scripts/" + script.script_name);

            lua->GetState()["scene"] = scene;
            lua->GetState()["entity"] = list.GetEntity(static_cast<unsigned int>(entity_id));

            script.state = lua->GetState().create_table();
            lua->RunFunction("init");

            if(!script.data.data.size())
            {
                Controller::ConvertMapToSol(script.data, script.state);
            }
        });
    }

    void System::Script_Update(std::shared_ptr<LuaState> lua, EntityList& list, Scene* scene)
	{
        m_registry->view<Script>().each([&](auto entity_id, auto& script) 
        {
            lua->RunFile("scripts/" + script.script_name);

            lua->GetState()["scene"] = scene;
            lua->GetState()["entity"] = list.GetEntity(static_cast<unsigned int>(entity_id));
            lua->GetState()["state"] = script.state;

            lua->RunFunction("update");
        });
    }

    void System::Script_Deserialize(std::shared_ptr<LuaState> lua, EntityList& list, Scene* scene)
    {
        m_registry->view<Script>().each([&](auto entity_id, auto& script) 
        {
            lua->RunFile("scripts/" + script.script_name);

            lua->GetState()["scene"] = scene;
            lua->GetState()["entity"] = list.GetEntity(static_cast<unsigned int>(entity_id));

            script.state = lua->GetState().create_table();
            Controller::ConvertMapToSol(script.data, script.state);
            

            auto optFunc = lua->GetState()["deserialize"];
            if(optFunc.valid())
                lua->RunFunction("deserialize");
        });
    }

    void System::Physics_Init() {
        const auto view = m_registry->view<RigidBody>();

        for (auto entity : view) {
            auto&& ent = Entlist->GetEntity(static_cast<uint32_t>(entity));
            Physics_InitEntity(ent);
        }
    }

    void System::Physics_UpdateTransforms(const float factor)
    {
        m_registry->group<Transform, RigidBody>().each([&](auto& trans, auto& rigid) 
        {
            rp3d::Transform curr_transform = rigid.RigidBodyPtr->getTransform();

            // Compute the interpolated transform of the rigid body
            const rp3d::Transform interpolatedTransform =
                rp3d::Transform::interpolateTransforms(rigid.prevTransform, curr_transform, factor);

            rigid.prevTransform = curr_transform;

            trans.pos = to_glm(interpolatedTransform.getPosition());
            trans.rot = to_glm(interpolatedTransform.getOrientation());
        });
    }

    void System::Physics_SerializationPrep()
    {
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

        rigid.initialized = true;
    };

    void System::Physics_AddTerrain(const Terrain& ter) { m_SP->AddTerrain(ter); }

    

}  // namespace Vakol::Controller

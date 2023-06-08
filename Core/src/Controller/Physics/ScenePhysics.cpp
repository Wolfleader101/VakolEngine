#include "ScenePhysics.hpp"

#include <glad/glad.h>

#include <Controller/AssetLoader/AssetLoader.hpp>
#include <Controller/Logger.hpp>
#include <Controller/Physics/PhysicsPool.hpp>
#include <Controller/System.hpp>
#include <Controller/Terrain.hpp>

namespace Vakol::Controller::Physics {

    ScenePhysics::ScenePhysics(rp3d::PhysicsWorld* newWorld) : m_Terrain(nullptr), m_World(newWorld), m_callback() {
        m_DebugRenderer = View::DebugRenderer(m_World);

        m_World->setEventListener(&m_callback);
    };

    void ScenePhysics::Init() {
        System::Physics_Init();

        m_World->setEventListener(&m_callback);
    };

    void ScenePhysics::Update(const Time& time, const Camera& camera) {
        // Add the time difference in the accumulator
        m_accumulator += time.deltaTime;

        // While there is enough accumulated time to take
        // one or several physics steps
        while (m_accumulator >= m_timestep) {
            // Update the physics world with a constant time step
            m_World->update(m_timestep);

            // Decrease the accumulated time
            m_accumulator -= m_timestep;
        }

        // Compute the time interpolation factor
        float factor = m_accumulator / m_timestep;

        // call update on transforms
        System::Physics_UpdateTransforms(factor);

        if (m_DebugRenderer.IsEnabled()) {
            m_DebugRenderer.Update();
            m_DebugRenderer.Draw(camera);
        }
    }

    void ScenePhysics::EnableDebug(bool enable) { m_DebugRenderer.Enable(enable); }

    bool ScenePhysics::IsDebugEnabled() const { return m_DebugRenderer.IsEnabled(); }

    void ScenePhysics::AddTerrain(const Terrain& terr) {
        auto& HeightData = terr.GetHeightMap();
        const unsigned size = terr.GetSize();

        // get the min and max from height data
        float minH = std::min_element(HeightData.begin(), HeightData.end())[0];
        float maxH = std::max_element(HeightData.begin(), HeightData.end())[0];

        rp3d::HeightFieldShape* height = PhysicsPool::m_Common.createHeightFieldShape(
            size, size, minH, maxH, HeightData.data(), rp3d::HeightFieldShape::HeightDataType::HEIGHT_FLOAT_TYPE, 1, 1);

        // auto& vertexData = terr.GetStaticVertices();

        const auto trans = rp3d::Transform::identity();
        m_Terrain = m_World->createRigidBody(trans);
        m_Terrain->setType(rp3d::BodyType::STATIC);

        m_Terrain->addCollider(height, trans);
    }

    void ScenePhysics::MyCollisionCallback::onContact(const rp3d::CollisionCallback::CallbackData& callbackData) {
        for (int i = 0; i < callbackData.getNbContactPairs(); i++) {
            // Get the contact pair
            const ContactPair& contactPair = callbackData.getContactPair(i);

            VK_ERROR("TEST");
            // // Get the bodies involved in the contact
            // rp3d::RigidBody* body1 = static_cast<rp3d::RigidBody*>(contactPair.getBody1());
            // rp3d::RigidBody* body2 = static_cast<rp3d::RigidBody*>(contactPair.getBody2());

            // // Get the entities associated with the bodies
            // Components::RigidBody* rb1 = static_cast<Components::RigidBody*>(body1->getUserData());
            // Components::RigidBody* rb2 = static_cast<Components::RigidBody*>(body2->getUserData());
            // rb1->is_colliding = true;
            // rb2->is_colliding = true;
        }
    }
}  // namespace Vakol::Controller::Physics
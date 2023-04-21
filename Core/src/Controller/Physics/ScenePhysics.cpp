#include "ScenePhysics.hpp"

#include <glad/glad.h>

#include <Controller/AssetLoader/AssetLoader.hpp>
#include <Controller/Logger.hpp>
#include <Controller/Physics/PhysicsPool.hpp>
#include <Controller/System.hpp>
#include <Controller/Terrain.hpp>

namespace Vakol::Controller::Physics {

    ScenePhysics::ScenePhysics(rp3d::PhysicsWorld* newWorld) : m_World(newWorld), m_Terrain(nullptr){};

    ScenePhysics::~ScenePhysics(){};

    void ScenePhysics::Init() {
        System::Physics_Init();

    };

    void ScenePhysics::Update(const Time& time, const Vakol::Controller::Camera& camera) {
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
        Vakol::Controller::System::Physics_UpdateTransforms(factor);
    }

    void ScenePhysics::AddTerrain(const std::shared_ptr<Vakol::Controller::Terrain>& terr)
    {
        const auto HeightData = terr->GetHeightMap();
        const unsigned size = terr->GetSize();

        const float maxH = terr->GetMaxHeight();
        const float minH = terr->GetMinHeight();

        rp3d::HeightFieldShape* height = PhysicsPool::m_Common.createHeightFieldShape(
            size,
            size,
            minH,
            maxH,
            HeightData.data(),
            rp3d::HeightFieldShape::HeightDataType::HEIGHT_FLOAT_TYPE,
            1,
            1
        );

        const auto trans = rp3d::Transform::identity();
        m_Terrain = m_World->createRigidBody(trans);
        m_Terrain->setType(rp3d::BodyType::STATIC);

        m_Terrain->addCollider(height, trans);
       
    }

}  // namespace Vakol::Controller::Physics
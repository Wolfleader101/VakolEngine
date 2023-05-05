#include "ScenePhysics.hpp"

#include <glad/glad.h>

#include <Controller/AssetLoader/AssetLoader.hpp>
#include <Controller/Logger.hpp>
#include <Controller/Physics/PhysicsPool.hpp>
#include <Controller/System.hpp>
#include <Controller/Terrain.hpp>

namespace Vakol::Controller::Physics {

    ScenePhysics::ScenePhysics(rp3d::PhysicsWorld* newWorld) : m_World(newWorld), m_Terrain(nullptr)
    {
        m_DebugRenderer = View::DebugRenderer(m_World);
    };

    ScenePhysics::~ScenePhysics(){};

    void ScenePhysics::Init() { System::Physics_Init(); };

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

        if(m_DebugRenderer.IsEnabled())
        {
            m_DebugRenderer.Update();
            m_DebugRenderer.Draw(camera);
        }
            
    }

    void ScenePhysics::EnableDebug(bool enable)
    {
        m_DebugRenderer.Enable(enable);
    }

    bool ScenePhysics::IsDebugEnabled()
    {
        return m_DebugRenderer.IsEnabled();
    }

    void ScenePhysics::AddTerrain(const Terrain& terr) {
        auto& HeightData = terr.GetHeightMap();
        const unsigned size = terr.GetSize();

        // get the min and max from height data
        float minH = HeightData[0];
        float maxH = HeightData[0];
        for (unsigned i = 0; i < HeightData.size(); i++) {
            if (HeightData[i] < minH) {
                minH = HeightData[i];
            }
            if (HeightData[i] > maxH) {
                maxH = HeightData[i];
            }
        }

        rp3d::HeightFieldShape* height = PhysicsPool::m_Common.createHeightFieldShape(
            size, size, minH, maxH, HeightData.data(), rp3d::HeightFieldShape::HeightDataType::HEIGHT_FLOAT_TYPE, 1,
            1);

        // auto& vertexData = terr.GetStaticVertices();
        // auto& indexData = terr.GetModel()->GetMesh().GetVertexArray()->GetIndices();

        // auto MeshPtr = PhysicsPool::m_Common.createTriangleMesh();

        // rp3d::TriangleVertexArray* triArray = nullptr;

        // triArray = new rp3d::TriangleVertexArray(vertexData.size(), vertexData.data(), sizeof(float) * 3,
        //                                          indexData.size() / 3, indexData.data(), sizeof(unsigned int) * 3,
        //                                          rp3d::TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
        //                                          rp3d::TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE);

        // MeshPtr->addSubpart(triArray);

        // auto col = PhysicsPool::m_Common.createConcaveMeshShape(MeshPtr, rp3d::Vector3(1, 1, 1));

        const auto trans = rp3d::Transform::identity();
        m_Terrain = m_World->createRigidBody(trans);
        m_Terrain->setType(rp3d::BodyType::STATIC);

        m_Terrain->addCollider(height, trans);
    }

}  // namespace Vakol::Controller::Physics
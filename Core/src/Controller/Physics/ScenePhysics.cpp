#include "ScenePhysics.hpp"

#include <Controller/Physics/PhysicsPool.hpp>
#include <Controller/Logger.hpp>

#include <Controller/Terrain.hpp>

#include <Controller/System.hpp>


namespace Vakol::Controller::Physics 
{
    
    
    ScenePhysics::ScenePhysics(rp3d::PhysicsWorld* newWorld) : m_World(newWorld){};

    ScenePhysics::~ScenePhysics() { };

    void ScenePhysics::Init()
    {
        System::Physics_Init();
    };

    void ScenePhysics::Update(const Time& time)
    {
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
        auto& vertices = terr->GetVertices();
        auto& indices = terr->GetIndices();

        if (vertices.empty() || indices.empty())
        {
            VK_CRITICAL("Terrain has no vertice or indice data. Can not add to physics");
            assert(0);
        }

        rp3d::Transform trans = rp3d::Transform::identity(); //not sure how this is gonna go
        
        m_Terrain = m_World->createCollisionBody(trans);
        auto MeshPtr = PhysicsPool::m_Common.createTriangleMesh();

        rp3d::TriangleVertexArray* triArray = nullptr;

        triArray = new rp3d::TriangleVertexArray(
            vertices.size(), vertices.data(), sizeof(float) * 3,
            indices.size() / 3, indices.data(), sizeof(unsigned int) * 3,
            rp3d::TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
            rp3d::TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE
        );

        MeshPtr->addSubpart(triArray);

        rp3d::ConcaveMeshShape* ConcaveShape = PhysicsPool::m_Common.createConcaveMeshShape(MeshPtr);

        m_Terrain->addCollider(ConcaveShape, trans);






    };
}
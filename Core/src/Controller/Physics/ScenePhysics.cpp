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
        shader = std::make_shared<Model::Assets::GLShader>("coreAssets/shaders/debug.prog");

    	if(debug) EnableDebug();

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
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

        if (debug) {
            auto debugData = GetDebugTriangles();
            auto lineVerts = GetDebugLines();

            debugData.insert(debugData.end(), lineVerts.begin(), lineVerts.end());

            shader->Bind();

            shader->SetMat4("PV", camera.GetMatrix(2));
            glBindVertexArray(vao);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            glBufferData(GL_ARRAY_BUFFER, debugData.size() * sizeof(PhysicsDebugVertex), debugData.data(),
                         GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PhysicsDebugVertex), (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PhysicsDebugVertex), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glDrawArrays(GL_LINES, 0, debugData.size());

            glBindVertexArray(0);

            // shader->Unbind();
        }

        // Compute the time interpolation factor
        float factor = m_accumulator / m_timestep;

        // call update on transforms
        Vakol::Controller::System::Physics_UpdateTransforms(factor);
    }

    //void ScenePhysics::AddTerrain(const std::shared_ptr<Vakol::Controller::Terrain>& terr) {
    //    auto& vertices = terr->GetVertices();
    //    auto& indices = terr->ConvertStripToTriangles();

    //    if (vertices.empty() || indices.empty()) {
    //        VK_CRITICAL("Terrain has no vertice or indice data. Can not add to physics");
    //        assert(0);
    //    }

    //    rp3d::Transform trans = rp3d::Transform::identity();  // not sure how this is gonna go

    //    m_Terrain = m_World->createRigidBody(trans);

    //    m_Terrain->setType(rp3d::BodyType::STATIC);

    //    auto MeshPtr = PhysicsPool::m_Common.createTriangleMesh();

    //    rp3d::TriangleVertexArray* triArray = nullptr;

    //    triArray = new rp3d::TriangleVertexArray(vertices.size(), vertices.data(), sizeof(Vertex), indices.size() / 3,
    //                                             indices.data(), sizeof(unsigned int) * 3,
    //                                             rp3d::TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
    //                                             rp3d::TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE);

    //    MeshPtr->addSubpart(triArray);

    //    rp3d::ConcaveMeshShape* ConcaveShape = PhysicsPool::m_Common.createConcaveMeshShape(MeshPtr, {1, 1, 1});

    //    m_Terrain->addCollider(ConcaveShape, trans);

    //    int x = 0;
    //};

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

        m_Terrain->addCollider(height, trans);
       
    }

    glm::vec3 ScenePhysics::GetDebugColor(rp3d::uint32 color) {
        switch (color) {
            case static_cast<unsigned>(rp3d::DebugRenderer::DebugColor::RED):
                return {1, 0, 0};
            case static_cast<unsigned>(rp3d::DebugRenderer::DebugColor::GREEN):
                return {0, 1, 0};
            case static_cast<unsigned>(rp3d::DebugRenderer::DebugColor::BLUE):
                return {0, 0, 1};
            case static_cast<unsigned>(rp3d::DebugRenderer::DebugColor::BLACK):
                return {0, 0, 0};
            case static_cast<unsigned>(rp3d::DebugRenderer::DebugColor::WHITE):
                return {1, 1, 1};
            case static_cast<unsigned>(rp3d::DebugRenderer::DebugColor::YELLOW):
                return {1, 1, 0};
            case static_cast<unsigned>(rp3d::DebugRenderer::DebugColor::MAGENTA):
                return {1, 0, 1};
            case static_cast<unsigned>(rp3d::DebugRenderer::DebugColor::CYAN):
                return {0, 1, 1};
            default:
                return {1, 1, 1};
        }
    }

    /**
     * @brief Get the Debug Triangles
     *
     * @return std::vector<PhysicsDebugVertex>
     */
    std::vector<ScenePhysics::PhysicsDebugVertex> ScenePhysics::GetDebugTriangles() {
        std::vector<PhysicsDebugVertex> debugVec;
        for (const auto& tri : m_World->getDebugRenderer().getTriangles()) {
            glm::vec3 color1 = GetDebugColor(tri.color1);
            glm::vec3 color2 = GetDebugColor(tri.color2);
            glm::vec3 color3 = GetDebugColor(tri.color3);

            PhysicsDebugVertex v1{
                tri.point1.x, tri.point1.y, tri.point1.z, color1.x, color1.y, color1.z,
            };

            PhysicsDebugVertex v2{
                tri.point2.x, tri.point2.y, tri.point2.z, color2.x, color2.y, color2.z,
            };

            PhysicsDebugVertex v3{
                tri.point3.x, tri.point3.y, tri.point3.z, color3.x, color3.y, color3.z,
            };

            debugVec.push_back(v1);
            debugVec.push_back(v2);
            debugVec.push_back(v3);
        }

        return debugVec;
    };

    std::vector<ScenePhysics::PhysicsDebugVertex> ScenePhysics::GetDebugLines() {
        std::vector<PhysicsDebugVertex> debugVec;
        for (const auto& line : m_World->getDebugRenderer().getLines()) {
            glm::vec3 color1 = GetDebugColor(line.color1);
            glm::vec3 color2 = GetDebugColor(line.color2);

            PhysicsDebugVertex v1{
                line.point1.x, line.point1.y, line.point1.z, color1.x, color1.y, color1.z,
            };

            PhysicsDebugVertex v2{
                line.point2.x, line.point2.y, line.point2.z, color2.x, color2.y, color2.z,
            };

            debugVec.push_back(v1);
            debugVec.push_back(v2);
        }

        return debugVec;
    }

}  // namespace Vakol::Controller::Physics
#include "DebugRenderer.hpp"

#include <reactphysics3d/reactphysics3d.h>

namespace Vakol::View
{
    DebugRenderer::DebugRenderer(const std::shared_ptr<ScenePhysics>& scenePhysics, const std::shared_ptr<Shader>& shader)
    {
        m_scenePhysics = scenePhysics;
        m_Shader = shader;

        EnableWorldDebug();
    }

    void DebugRenderer::EnableWorldDebug()
    {
        m_scenePhysics->m_World->setIsDebugRenderingEnabled(true);

        m_rp3dRenderer->setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLIDER_AABB, true);
		m_rp3dRenderer->setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
		m_rp3dRenderer->setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::CONTACT_POINT, true);
        
        m_rp3dRenderer = &m_scenePhysics->m_World->getDebugRenderer();
    }

    void DebugRenderer::DisableWorldDebug()
    {
        m_scenePhysics->m_World->setIsDebugRenderingEnabled(false);
        m_rp3dRenderer = nullptr;
    }

    void DebugRenderer::SetScenePhysics(const std::shared_ptr<ScenePhysics> scenePhysics)
    {
        m_scenePhysics = scenePhysics;
        EnableWorldDebug();

        glCreateVertexArrays(1, &m_VAO);

        glCreateBuffers(1, &m_VBO);


    }

    void DebugRenderer::UnsetScenePhysics()
    {
        DisableWorldDebug();
        m_scenePhysics = nullptr;

        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        
    }

    void DebugRenderer::GetDebugColor(const uint32_t& color, glm::vec3& outColor) const
    {
        uint32_t colorValue = color;

        outColor.r = ((colorValue >> 16) & 0xFF) / 255.0f;
        outColor.g = ((colorValue >> 8) & 0xFF) / 255.0f;
        outColor.b = (colorValue & 0xFF) / 255.0f;
    }

    void DebugRenderer::GetTriangles()
    {
        glm::vec3 color, xyz;
        PhysicsDebugVertex tempVert;

        for (auto& tri : m_rp3dRenderer->getTriangles())
			{
                //v1
                GetDebugColor(tri.color1, color);

                xyz.x = tri.point1.x;
                xyz.y = tri.point1.y;
                xyz.z = tri.point1.z;

				tempVert = {xyz, color};

                m_DebugData.push_back(tempVert);

                //v2
                GetDebugColor(tri.color2, color);

                xyz.x = tri.point2.x;
                xyz.y = tri.point2.y;
                xyz.z = tri.point2.z;

				tempVert = {xyz, color};

                m_DebugData.push_back(tempVert);


                //v3
                GetDebugColor(tri.color3, color);

                xyz.x = tri.point3.x;
                xyz.y = tri.point3.y;
                xyz.z = tri.point3.z;

				tempVert = {xyz, color};

				m_DebugData.push_back(tempVert);
			}
    }

    void DebugRenderer::GetLines()
    {
        glm::vec3 color, xyz;

        PhysicsDebugVertex tempVert;

        for(auto& line : m_rp3dRenderer->getLines())
        {
            //v1
            GetDebugColor(line.color1, color);

            xyz.x = line.point1.x;
            xyz.y = line.point1.y;
            xyz.z = line.point1.z;

            tempVert = {xyz, color};

            m_DebugData.push_back(tempVert);

            //v2
            GetDebugColor(line.color2, color);

            xyz.x = line.point2.x;
            xyz.y = line.point2.y;
            xyz.z = line.point2.z;

            tempVert = {xyz, color};

            m_DebugData.push_back(tempVert);
        }
    }

    void DebugRenderer::Update()
    {
        if (!m_scenePhysics)
        {
            VK_ERROR("No scene physics set for debug renderer. Can't update.");
            return;
        }

        m_DebugData.clear();

        GetTriangles();
        GetLines();

    }

    void DebugRenderer::Draw(const Controller::Camera& camera) const
    {
        if (!m_scenePhysics)
        {
            VK_ERROR("No scene physics set for debug renderer. Can't draw.");
            return;
        }

        m_Shader->Bind();
        m_Shader->SetMat4("PV", camera.GetMatrix(PV_MATRIX));


        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_DebugData.size() * sizeof(PhysicsDebugVertex), m_DebugData.data(), GL_DYNAMIC_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PhysicsDebugVertex), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PhysicsDebugVertex), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glDrawArrays(GL_LINES, 0, m_DebugData.size());

        // Unbind the VAO
        glBindVertexArray(0);

        m_Shader->Unbind();

    }
}
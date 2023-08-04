#include "DebugRenderer.hpp"

#include <glad/glad.h>
#include <reactphysics3d/reactphysics3d.h>

#include "Controller/AssetLoader/AssetLoader.hpp"

namespace Vakol::View {
    DebugRenderer::DebugRenderer(rp3d::PhysicsWorld* WorldPtr) {
        m_World = WorldPtr;
        m_rp3dRenderer = &m_World->getDebugRenderer();

        m_Shader = Vakol::Controller::AssetLoader::GetShader("coreAssets/shaders/phyDebug.prog");

        glCreateVertexArrays(1, &m_VAO);
        glCreateBuffers(1, &m_VBO);

        VK_INFO("Debug Renderer Initialized");
    }

    void DebugRenderer::SetShader(const std::shared_ptr<Shader>& shader) { m_Shader = shader; }

    void DebugRenderer::Enable(bool enable) { enable ? EnableWorldDebug() : DisableWorldDebug(); }

    bool DebugRenderer::IsEnabled() const { return m_World->getIsDebugRenderingEnabled(); }

    void DebugRenderer::EnableWorldDebug() {
        m_World->setIsDebugRenderingEnabled(true);

        m_rp3dRenderer = &m_World->getDebugRenderer();

        m_rp3dRenderer->setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLIDER_AABB, true);
        m_rp3dRenderer->setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
        m_rp3dRenderer->setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::CONTACT_POINT, true);
    }

    void DebugRenderer::DisableWorldDebug() {
        m_World->setIsDebugRenderingEnabled(false);
        m_rp3dRenderer = nullptr;
    }

    void DebugRenderer::GetDebugColor(const uint32_t& color, glm::vec3& outColor) const {
        const uint32_t colorValue = color;

        outColor.x = (colorValue >> 16 & 0xFF) / 255.0f;
        outColor.y = (colorValue >> 8 & 0xFF) / 255.0f;
        outColor.z = (colorValue & 0xFF) / 255.0f;
    }

    void DebugRenderer::GetTriangles() {
        glm::vec3 color;
        PhysicsDebugVertex tempVert{};

        for (auto& tri : m_rp3dRenderer->getTriangles()) {
            glm::vec3 xyz;
            // v1
            GetDebugColor(tri.color1, color);

            xyz.x = static_cast<float>(tri.point1.x);
            xyz.y = static_cast<float>(tri.point1.y);
            xyz.z = static_cast<float>(tri.point1.z);

            tempVert = {xyz, color};

            m_DebugData.push_back(tempVert);

            // v2
            GetDebugColor(tri.color2, color);

            xyz.x = static_cast<float>(tri.point2.x);
            xyz.y = static_cast<float>(tri.point2.y);
            xyz.z = static_cast<float>(tri.point2.z);

            tempVert = {xyz, color};

            m_DebugData.push_back(tempVert);

            // v3
            GetDebugColor(tri.color3, color);

            xyz.x = static_cast<float>(tri.point3.x);
            xyz.y = static_cast<float>(tri.point3.y);
            xyz.z = static_cast<float>(tri.point3.z);

            tempVert = {xyz, color};

            m_DebugData.push_back(tempVert);
        }
    }

    void DebugRenderer::GetLines() {
        glm::vec3 color;

        for (auto& line : m_rp3dRenderer->getLines()) {
            glm::vec3 xyz;
            // v1
            GetDebugColor(line.color1, color);

            xyz.x = static_cast<float>(line.point1.x);
            xyz.y = static_cast<float>(line.point1.y);
            xyz.z = static_cast<float>(line.point1.z);

            PhysicsDebugVertex tempVert = {xyz, color};

            m_DebugData.push_back(tempVert);

            // v2
            GetDebugColor(line.color2, color);

            xyz.x = static_cast<float>(line.point2.x);
            xyz.y = static_cast<float>(line.point2.y);
            xyz.z = static_cast<float>(line.point2.z);

            tempVert = {xyz, color};

            m_DebugData.push_back(tempVert);
        }
    }

    void DebugRenderer::Update() {
        m_DebugData.clear();

        GetTriangles();
        GetLines();
    }

    void DebugRenderer::Draw() const {
        m_Shader->Bind();

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_DebugData.size() * sizeof(PhysicsDebugVertex)),
                     m_DebugData.data(), GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PhysicsDebugVertex), nullptr);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PhysicsDebugVertex),
                              reinterpret_cast<void*>(3 * sizeof(float)));

        glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(m_DebugData.size()));

        // Unbind the VAO
        glBindVertexArray(0);

        m_Shader->Unbind();
    }
}  // namespace Vakol::View
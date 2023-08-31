#include "Physics/PhysicsScene.hpp"

#include <reactphysics3d/reactphysics3d.h>

#include "Rendering/RenderData.hpp"
#include "Rendering/RenderEngine.hpp"

namespace Vakol
{
    static Math::Vec3 Vec3(const rp3d::Vector3& v)
    {
        return {v.x, v.y, v.z};
    }

    CollisionListener PhysicsScene::m_collisionListener;

    static Math::Vec3 GetDebugColor(rp3d::uint32 color);

    PhysicsScene::PhysicsScene(rp3d::PhysicsWorld* world) : m_world(world)
    {
        m_world->setEventListener(&m_collisionListener);
        m_world->setIsDebugRenderingEnabled(m_debugEnabled);

        CreateDebugScene();
    }

    RigidBody PhysicsScene::CreateRigidBody(const Math::Vec3& pos, const Math::Quat& orientation) const
    {
        RigidBody rb;

        rb.collisionData = std::make_shared<CollisionData>();

        const auto dPos = static_cast<Math::dVec3>(pos);
        const auto dRot = static_cast<Math::dQuat>(orientation); 

        const auto& transform = rp3d::Transform(rp3d::Vector3(dPos.x, dPos.y, dPos.z), rp3d::Quaternion(dRot.x, dRot.y, dRot.z, dRot.w));

        rb.collisionBody = m_world->createCollisionBody(transform);

        rb.collisionBody->setUserData(rb.collisionData.get());

        return rb;
    }

    void PhysicsScene::CreateDebugScene()
    {
        auto& debugRenderer = m_world->getDebugRenderer();

        debugRenderer.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLIDER_AABB, true);
        debugRenderer.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
        debugRenderer.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::CONTACT_POINT, true);
        debugRenderer.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::CONTACT_NORMAL, true);

        Rendering::RenderEngine::GenerateDebugScene(m_debugScene);
    }

    void PhysicsScene::UpdateDebugScene() const
    {
        Rendering::DebugVertex p1{};
        Rendering::DebugVertex p2{};
        Rendering::DebugVertex p3{};

        std::vector<Rendering::DebugVertex> vertices;

        for (auto& tri : m_world->getDebugRenderer().getTriangles())
        {
            p1.position = Vec3(tri.point1);
            p2.position = Vec3(tri.point2);
            p3.position = Vec3(tri.point3);

            p1.color = GetDebugColor(tri.color1);
            p2.color = GetDebugColor(tri.color2);
            p3.color = GetDebugColor(tri.color3);

            vertices.emplace_back(p1);
            vertices.emplace_back(p2);
            vertices.emplace_back(p3);
        }

        for (auto& line : m_world->getDebugRenderer().getLines())
        {
            p1.position = Vec3(line.point1);
            p2.position = Vec3(line.point2);

            p1.color = GetDebugColor(line.color1);
            p2.color = GetDebugColor(line.color2);

            vertices.emplace_back(p1);
            vertices.emplace_back(p2);
        }

        Rendering::RenderAPI::SetDebugVertexArray(std::move(vertices), m_debugScene);
    }

    void PhysicsScene::Update(const double timeStep) const
    {
        m_world->update(timeStep);

        if (m_debugEnabled)
            UpdateDebugScene();
    }

    const Rendering::DebugScene& PhysicsScene::GetDebugScene() const
    {
        return m_debugScene;
    }

    bool PhysicsScene::IsDebugEnabled() const
    {
        return m_debugEnabled;
    }

    const xg::Guid& PhysicsScene::GetGuid() const
    {
        return m_guid;
    }

    Math::Vec3 GetDebugColor(const rp3d::uint32 color)
    {
        switch (color)
        {
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
} // namespace Vakol
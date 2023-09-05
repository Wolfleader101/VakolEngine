#include "Physics/PhysicsScene.hpp"

#include <reactphysics3d/reactphysics3d.h>

#include "Rendering/RenderData.hpp"

namespace Vakol
{
    static Math::Vec3 Vec3(const rp3d::Vector3& v);
    static Math::Vec3 GetDebugColor(rp3d::uint32 color);

    PhysicsScene::PhysicsScene(rp3d::PhysicsWorld* world, const bool debugEnabled) : m_world(world)
    {
        m_world->setEventListener(&m_collisionListener);
        m_world->setIsDebugRenderingEnabled(debugEnabled);

        if (debugEnabled)
            SetDebugActive();
    }

    RigidBody PhysicsScene::CreateRigidBody(Math::Vec3& pos, Math::Quat& orientation)
    {
        RigidBody rb;

        rb.collisionData = std::make_shared<CollisionData>();

        rp3d::Transform trans(rp3d::Vector3(pos.x, pos.y, pos.z),
                              rp3d::Quaternion(orientation.x, orientation.y, orientation.z, orientation.w));
        rb.collisionBody = m_world->createCollisionBody(trans);

        rb.collisionBody->setUserData(static_cast<void*>(rb.collisionData.get()));

        return rb;
    }

    void PhysicsScene::SetDebugActive() const
    {
        auto& debugRenderer = m_world->getDebugRenderer();

        debugRenderer.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLIDER_AABB, true);
        debugRenderer.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
        debugRenderer.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::CONTACT_POINT, true);
        debugRenderer.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::CONTACT_NORMAL, true);
    }

    void PhysicsScene::GetVertices(std::vector<Rendering::DebugVertex>& vertices) const
    {
        Rendering::DebugVertex p1{};
        Rendering::DebugVertex p2{};
        Rendering::DebugVertex p3{};

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
    }

    const GUID& PhysicsScene::GetGuid() const
    {
        return m_guid;   
    }

    void PhysicsScene::Update(const double timeStep)
    {
        m_world->update(timeStep);
    }

    Math::Vec3 Vec3(const rp3d::Vector3& v)
    {
        return {v.x, v.y, v.z};
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
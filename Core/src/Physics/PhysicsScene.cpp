#include "Physics/PhysicsScene.hpp"

#include <reactphysics3d/reactphysics3d.h>

#include "Rendering/RenderData.hpp"

namespace Vakol
{
    static Math::Vec3 GetDebugColor(rp3d::uint32 color);

    PhysicsScene::PhysicsScene(rp3d::PhysicsWorld* world) : m_world(world)
    {
        m_world->setEventListener(&m_collisionListener);
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

    void PhysicsScene::EnableDebug() const
    {
        m_world->setIsDebugRenderingEnabled(true);

        auto& debugRenderer = m_world->getDebugRenderer();

        debugRenderer.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLIDER_AABB, true);
        debugRenderer.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
        debugRenderer.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::CONTACT_POINT, true);
        debugRenderer.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::CONTACT_NORMAL, true);
    }

    void PhysicsScene::DisableDebug() const
    {
        m_world->setIsDebugRenderingEnabled(false);
    }

    void PhysicsScene::GetVertices(std::vector<float>& vertices) const
    {

        for (rp3d::DebugRenderer::DebugTriangle& tri : m_world->getDebugRenderer().getTriangles())
        {
            vertices.emplace_back(tri.point1.x);
            vertices.emplace_back(tri.point1.y);
            vertices.emplace_back(tri.point1.z);

            Math::Vec3 color = GetDebugColor(tri.color1);

            vertices.emplace_back(color.r);
            vertices.emplace_back(color.g);
            vertices.emplace_back(color.b);

            vertices.emplace_back(tri.point2.x);
            vertices.emplace_back(tri.point2.y);
            vertices.emplace_back(tri.point2.z);

            color = GetDebugColor(tri.color2);

            vertices.emplace_back(color.r);
            vertices.emplace_back(color.g);
            vertices.emplace_back(color.b);

            vertices.emplace_back(tri.point3.x);
            vertices.emplace_back(tri.point3.y);
            vertices.emplace_back(tri.point3.z);

            color = GetDebugColor(tri.color3);

            vertices.emplace_back(color.r);
            vertices.emplace_back(color.g);
            vertices.emplace_back(color.b);
        }

        for (rp3d::DebugRenderer::DebugLine& line : m_world->getDebugRenderer().getLines())
        {
            vertices.emplace_back(line.point1.x);
            vertices.emplace_back(line.point1.y);
            vertices.emplace_back(line.point1.z);

            Math::Vec3 color = GetDebugColor(line.color1);

            vertices.emplace_back(color.r);
            vertices.emplace_back(color.g);
            vertices.emplace_back(color.b);

            vertices.emplace_back(line.point2.x);
            vertices.emplace_back(line.point2.y);
            vertices.emplace_back(line.point2.z);

            color = GetDebugColor(line.color2);

            vertices.emplace_back(color.r);
            vertices.emplace_back(color.g);
            vertices.emplace_back(color.b);
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
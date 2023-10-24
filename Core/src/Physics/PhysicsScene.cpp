#include "Physics/PhysicsScene.hpp"

#include "Logger/Logger.hpp"

#include <reactphysics3d/reactphysics3d.h>

class RayCastCallback : public rp3d::RaycastCallback
{
  public:
    bool firstHit = true;

    rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& info) override
    {
        if (firstHit || info.hitFraction < hit.distance)
        {
            hit.distance = info.hitFraction;

            hit.point = Vakol::Math::Vec3(info.worldPoint.x, info.worldPoint.y, info.worldPoint.z);
            hit.normal = Vakol::Math::Vec3(info.worldNormal.x, info.worldNormal.y, info.worldNormal.z);
            hit.isHit = true;

            hit.rigidbody = static_cast<Vakol::CollisionData*>(info.body->getUserData())->parentBody;
            firstHit = false;
        }

        return hit.distance;
    }

    Vakol::RayCastHitInfo hit;
};

namespace Vakol
{
    CollisionListener PhysicsScene::m_collisionListener;

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

        rb.collisionBody->setUserData(rb.collisionData.get());

        return rb;
    }

    bool PhysicsScene::RayCast(Math::Vec3& origin, Math::Vec3& direction, const float maxDistance,
                               RayCastHitInfo& info) const
    {
        Math::Normalize(direction);

        Math::Vec3 endPoint = origin + direction * maxDistance;

        const rp3d::Ray ray(ToRPVec3(origin), ToRPVec3(endPoint));

        RayCastCallback callback;

        m_world->raycast(ray, &callback);

        info = callback.hit;

        return info.isHit;
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

    void PhysicsScene::Update(const float timeStep)
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
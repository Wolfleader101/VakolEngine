#include "Physics/PhysicsScene.hpp"

#include <reactphysics3d/reactphysics3d.h>

namespace Vakol
{

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

    void PhysicsScene::Update(double timeStep)
    {
        m_world->update(timeStep);
    }

    const xg::Guid& PhysicsScene::GetGuid() const
    {
        return m_guid;
    }
} // namespace Vakol

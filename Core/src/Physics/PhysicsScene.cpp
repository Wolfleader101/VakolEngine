#include "Physics/PhysicsScene.hpp"

#include <reactphysics3d/reactphysics3d.h>

namespace Vakol
{
    CollisionListener PhysicsScene::m_collisionListener;

    PhysicsScene::PhysicsScene(rp3d::PhysicsWorld* world) : m_world(world)
    {
        m_world->setEventListener(&m_collisionListener);
    }

    RigidBody& PhysicsScene::CreateRigidBody(Math::Vec3& pos, Math::Quat& orientation)
    {
        std::shared_ptr<RigidBody> newBodyPtr = std::make_shared<RigidBody>();

        newBodyPtr->collisionData = std::make_shared<CollisionData>();
        rp3d::Transform trans(rp3d::Vector3(pos.x, pos.y, pos.z),
                              rp3d::Quaternion(orientation.x, orientation.y, orientation.z, orientation.w));
        newBodyPtr->collisionBody = m_world->createCollisionBody(trans);

        newBodyPtr->collisionBody->setUserData(static_cast<void*>(newBodyPtr->collisionData.get()));

        m_rigidBodies.push_back(newBodyPtr);
        m_rigidBodies.back()->collisionData->parentBody = m_rigidBodies.back().get();

        return *m_rigidBodies.back();
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
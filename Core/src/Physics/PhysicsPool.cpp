#include "Physics/PhysicsPool.hpp"

#include "Logger/Logger.hpp"
#include "Physics/ScenePhysics.hpp"

namespace Vakol::Physics
{
    rp3d::PhysicsCommon PhysicsPool::m_Common;

    ScenePhysics PhysicsPool::CreatePhysicsWorld()
    {
        return ScenePhysics(m_Common.createPhysicsWorld());
    }

    void PhysicsPool::DeletePhysicsWorld(ScenePhysics& toDestroy)
    {
        if (toDestroy.m_World != nullptr)
        {
            m_Common.destroyPhysicsWorld(toDestroy.m_World);
            toDestroy.m_World = nullptr;
        }
        else
        {
            VK_ERROR("Trying to delete PhysicsWorld that is already null");
        }
    }

} // namespace Vakol::Physics
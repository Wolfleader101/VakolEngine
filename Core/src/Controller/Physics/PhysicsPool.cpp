#include <Controller/Logger.hpp>
#include <Controller/Physics/PhysicsPool.hpp>
#include <Controller/Physics/ScenePhysics.hpp>

namespace Vakol::Controller::Physics
{
    rp3d::PhysicsCommon PhysicsPool::m_Common;

    ScenePhysics PhysicsPool::CreatePhysicsWorld()
    {
        return ScenePhysics(m_Common.createPhysicsWorld());
    }

    void PhysicsPool::DeletePhysicsWorld(ScenePhysics &toDestroy)
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

} // namespace Vakol::Controller::Physics
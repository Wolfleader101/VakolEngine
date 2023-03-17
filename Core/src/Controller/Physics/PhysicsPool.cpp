#include <Controller/Physics/PhysicsPool.hpp>
#include <Controller/Physics/ScenePhysics.hpp>

namespace Vakol::Controller::Physics
{
    

    ScenePhysics PhysicsPool::CreatePhysicsWorld() 
    { 
        return ScenePhysics(m_Common.createPhysicsWorld()); 
    }

    void PhysicsPool::DeletePhysicsWorld() {}



}
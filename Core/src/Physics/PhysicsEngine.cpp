#include "Physics/PhysicsEngine.hpp"
#include "Physics/PhysicsScene.hpp"

#include <algorithm>

namespace Vakol
{
    PhysicsEngine::PhysicsEngine()
    {
    }

    PhysicsScene& PhysicsEngine::CreateScene()
    {
        rp3d::PhysicsWorld* newWorld = m_rp_common.createPhysicsWorld();
        PhysicsScene newScene(newWorld);
        m_scenes.push_back(newScene);

        return m_scenes.back();
    }

    void PhysicsEngine::DestroyScene(PhysicsScene& scene)
    {
        auto& targetGuid = scene.GetGuid();

        auto it = std::find_if(m_scenes.begin(), m_scenes.end(),
                               [&targetGuid](const PhysicsScene& s) { return s.GetGuid() == targetGuid; });

        if (it != m_scenes.end())
        {
            m_rp_common.destroyPhysicsWorld(it->m_world);
            m_scenes.erase(it);
        }
    }

    void PhysicsEngine::ApplyForces(RigidBody& rb)
    {
    }

    void PhysicsEngine::DetectCollisions(PhysicsScene& scene)
    {
        scene.Update(m_timeStep);
    }

    void PhysicsEngine::ResolveCollisions(RigidBody& rb)
    {
    }

    void PhysicsEngine::SetTimeStep(double step)
    {
        m_timeStep = step;
    }

    double PhysicsEngine::GetTimeStep() const
    {
        return m_timeStep;
    }

} // namespace Vakol
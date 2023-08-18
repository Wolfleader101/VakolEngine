#pragma once

#include <memory>
#include <vector>

#include "PhysicsTypes.hpp"

#include "Math/Math.hpp"

namespace Vakol
{

    class PhysicsScene;

    class PhysicsEngine
    {
      public:
        PhysicsEngine();
        PhysicsScene& CreateScene();

        void DestroyScene(PhysicsScene& scene);

        void ApplyForces(RigidBody& rb);            // applies forces to the rigidbodies
        void DetectCollisions(PhysicsScene& scene); // detect collisions of bodies
        void ResolveCollisions(RigidBody& rb);      // handles collision resolution

        void SetTimeStep(double step);
        double GetTimeStep() const;

      private:
        std::vector<std::unique_ptr<PhysicsScene>> m_scenes;

        double m_timeStep = 1.0 / 60.0;

        rp3d::PhysicsCommon m_rp_common;
    };

} // namespace Vakol
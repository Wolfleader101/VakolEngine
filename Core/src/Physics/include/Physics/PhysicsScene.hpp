#pragma once

#include "CollisionListener.hpp"
#include "PhysicsTypes.hpp"

#include <crossguid/guid.hpp>

namespace Vakol
{

    class PhysicsEngine;

    class PhysicsScene
    {
      public:
        RigidBody CreateRigidBody(Math::Vec3& pos, Math::Quat& orientation);

        void Update(double timeStep);

        const xg::Guid& GetGuid() const;

      private:
        PhysicsScene(rp3d::PhysicsWorld* world);

        rp3d::PhysicsWorld* m_world;
        CollisionListener m_collisionListener;
        xg::Guid m_guid;

        friend class PhysicsEngine;
    };
} // namespace Vakol
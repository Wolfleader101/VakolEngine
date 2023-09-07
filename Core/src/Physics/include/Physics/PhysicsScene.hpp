#pragma once

#include "CollisionListener.hpp"
#include "PhysicsTypes.hpp"

#include "Utils/GUID.hpp"

namespace Vakol
{
    class PhysicsEngine;

    /**
     * @brief Physics scene class
     *
     */
    class PhysicsScene
    {
      public:
        /**
         * @brief Create a Rigid Body object
         *
         * @param pos of body
         * @param orientation of body
         * @return RigidBody created
         */
        RigidBody CreateRigidBody(Math::Vec3& pos, Math::Quat& orientation);

        void EnableDebug() const;
        void DisableDebug() const;

        void GetVertices(std::vector<float>& vertices) const;

        const GUID& GetGuid() const;

      private:
        /**
         * @brief Construct a new Physics Scene object
         *
         * @param world to create scene from
         */
        explicit PhysicsScene(rp3d::PhysicsWorld* world);

        /**
         * @brief update a physics scene
         *
         * @param timeStep to update with
         */
        void Update(double timeStep);

        /**
         * @brief rp3d physics world ptr
         *
         */
        rp3d::PhysicsWorld* m_world = nullptr;

        /**
         * @brief collision listener
         *
         */
        CollisionListener m_collisionListener;

        GUID m_guid;

        friend class PhysicsEngine;
    };
} // namespace Vakol
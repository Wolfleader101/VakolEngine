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
     * This class represents a physics scene where rigid bodies can exist, move, and interact according to the laws of
     * physics defined in the scene.
     */
    class PhysicsScene
    {
      public:
        /**
         * @brief Creates a rigid body object with a specified position and orientation.
         *
         * @param pos The initial position of the body in the scene.
         * @param orientation The initial orientation of the body in the scene.
         * @return RigidBody The rigid body created with the specified position and orientation.
         */
        RigidBody CreateRigidBody(Math::Vec3& pos, Math::Quat& orientation);

        void DestroyRigidBody(RigidBody& rigidBody);

        void setOnCollisionCallback(std::function<void(RigidBody&, RigidBody&)> callback);

        /**
         * @brief Performs a raycast in the scene from a specified origin in a given direction, up to a maximum
         * distance.
         *
         * The raycast can be used to find objects in the path of the ray. It populates a RayCastHitInfo structure with
         * information about the hit, if any.
         *
         * @param origin The starting point of the ray.
         * @param direction The direction in which the ray should be cast.
         * @param maxDistance The maximum distance the ray should be cast.
         * @param info A structure to hold information about the hit, if any.
         * @return true If the ray hit an object within maxDistance.
         * @return false If the ray did not hit any object within maxDistance.
         */
        bool RayCast(Math::Vec3& origin, Math::Vec3& direction, float maxDistance, RayCastHitInfo& info) const;

        /**
         * @brief Enables debug mode, providing more detailed information for debugging purposes.
         *
         */
        void EnableDebug() const;

        /**
         * @brief Disables debug mode, reducing the detail of information provided for debugging purposes.
         *
         */
        void DisableDebug() const;

        /**
         * @brief Retrieves the vertices of all the rigid bodies in the scene.
         *
         * @param vertices A vector to hold the vertices data.
         */
        void GetVertices(std::vector<float>& vertices) const;

        /**
         * @brief Retrieves the unique identifier (GUID) of the physics scene.
         *
         * @return const GUID& The GUID of the physics scene.
         */
        const GUID& GetGuid() const;

      private:
        /**
         * @brief Constructs a new Physics Scene object with a specified physics world.
         *
         * @param world The physics world from which to create the scene.
         */
        explicit PhysicsScene(rp3d::PhysicsWorld* world);

        /**
         * @brief Updates the physics scene with a new timestep, simulating physics for that duration.
         *
         * @param timeStep The time interval over which to update the physics simulation.
         */
        void Update(double timeStep);

        /**
         * @brief Pointer to the rp3d physics world object representing the physical world of the scene.
         *
         */
        rp3d::PhysicsWorld* m_world = nullptr;

        /**
         * @brief The collision listener that handles collision events in the physics scene.
         *
         */
        static CollisionListener m_collisionListener;

        /**
         * @brief The unique identifier (GUID) for this physics scene.
         *
         */
        GUID m_guid;

        friend class PhysicsEngine;
    };
} // namespace Vakol
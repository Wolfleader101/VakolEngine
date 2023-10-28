#pragma once

#include <memory>
#include <vector>

#include "PhysicsTypes.hpp"

#include "Math/Math.hpp"

namespace Vakol
{

    class PhysicsScene;

    /**
     * @brief Physics engine class
     *
     */
    class PhysicsEngine
    {
      public:
        /**
         * @brief Construct a new Physics Engine object
         *
         */
        PhysicsEngine();

        /**
         * @brief Create a Scene object
         *
         * @return PhysicsScene& thats been created
         */
        PhysicsScene& CreateScene();

        /**
         * @brief Destroy a Scene object
         *
         * @param scene to destroy
         */
        void DestroyScene(PhysicsScene& scene);

        CollisionShape CreateBoxShape(Math::Vec3& halfExtents);
        CollisionShape CreateSphereShape(float radius);
        CollisionShape CreateCapsuleShape(float radius, float height);
        CollisionShape CreateMeshShape(std::vector<Math::Point>& vertices, std::vector<unsigned int>& indices);

        /**
         * @brief calculate the centre of mass of a rigidbody
         *
         * @param rb rigidbody to calculate
         * @param compoundCollider to calculate
         */
        void CalculateCentreOfMass(RigidBody& rb, CompoundCollider& compoundCollider);

        void CalculateCombinedIntertia(RigidBody& rb, CompoundCollider& compoundCollider);

        /**
         * @brief Attach a collider to a rigidbody
         *
         * @param rb body to attach too
         * @param compoundCollider to attach too
         * @param relativePos position relative to the rigidbody
         */
        void AttachCollider(RigidBody& rb, CompoundCollider& compoundCollider, Math::Vec3& relativePos);

        /**
         * @brief Apply forces to a rigidbody
         *
         * @param rb to apply forces
         */
        void ApplyForces(Math::Vec3& pos, Math::Quat& rot, RigidBody& rb);

        /**
         * @brief detect collisions in a world
         *
         * @param scene to check
         */
        void DetectCollisions(PhysicsScene& scene);

        /**
         * @brief Set the Time Step object
         *
         * @param step to set
         */
        void SetTimeStep(float step);

        /**
         * @brief Get the Time Step object
         *
         * @return double time step
         */
        float GetTimeStep() const;

        /**
         * @brief damping for velocities
         *
         */
        float velocityDamping = 0.992f;

        /**
         * @brief gravity vector
         *
         */
        Math::Vec3 gravity = Math::Vec3(0.0f, -9.82f, 0.0f);

      private:
        /**
         * @brief scenes vector
         *
         */
        std::vector<std::unique_ptr<PhysicsScene>> m_scenes;

        /**
         * @brief time step value
         *
         */
        float m_timeStep = 1.0f / 60.0f;

        /**
         * @brief react physics common object
         *
         */
        rp3d::PhysicsCommon m_rpCommon;
    };

} // namespace Vakol
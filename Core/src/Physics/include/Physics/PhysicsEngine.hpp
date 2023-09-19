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

        /**
         * @brief create an AABB collider
         *
         * @param halfExtents of AABB
         * @return BoxCollider created
         */
        BoxCollider CreateBoxCollider(Math::Vec3& halfExtents);

        /**
         * @brief Create a Sphere Collider object
         *
         * @param radius of sphere
         * @return SphereCollider
         */
        SphereCollider CreateSphereCollider(float radius);

        /**
         * @brief Create a Capsule Collider object
         *
         * @param radius of capsule
         * @param height of capsule
         * @return CapsuleCollider
         */
        CapsuleCollider CreateCapsuleCollider(float radius, float height);

        /**
         * @brief Create a Mesh Collider object
         *
         * @param vertices vertices of mesh
         * @param indices indices of mesh
         * @return MeshCollider
         */
        MeshCollider CreateMeshCollider(std::vector<Math::Point>& vertices, std::vector<unsigned int>& indices);

        /**
         * @brief Attach a collider to a rigidbody
         *
         * @param rb body to attach too
         * @param collider to attach
         */
        void AttachCollider(RigidBody& rb, BoxCollider& collider);

        /**
         * @brief Attach a collider to a rigidbody
         *
         * @param rb body to attach too
         * @param collider to attach
         */
        void AttachCollider(RigidBody& rb, SphereCollider& collider);

        /**
         * @brief Attach a collider to a rigidbody
         *
         * @param rb body to attach too
         * @param collider to attach
         */
        void AttachCollider(RigidBody& rb, CapsuleCollider& collider);

        /**
         * @brief Attach a collider to a rigidbody
         *
         * @param rb body to attach too
         * @param collider to attach
         */
        void AttachCollider(RigidBody& rb, MeshCollider& collider);

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
         * @brief handle collision resolution for a rigidbody
         *
         * @param pos pos of body
         * @param rb to handle resolution
         */
        void ResolveCollisions(Math::Vec3& pos, RigidBody& rb);

        static float SolveLambda(RigidBody& rb1, RigidBody& rb2);

        /**
         * @brief handle depenetration for a rigidbody
         *
         * @param pos pos of body
         * @param rb rigidbody
         */
        void Depenetration(Math::Vec3& pos, RigidBody& rb);

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
        float m_timeStep = 1.0f / 120.0f;

        /**
         * @brief react physics common object
         *
         */
        rp3d::PhysicsCommon m_rpCommon;
    };

} // namespace Vakol
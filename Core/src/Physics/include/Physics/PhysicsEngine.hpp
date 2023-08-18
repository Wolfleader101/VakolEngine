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
         * @brief Apply forces to a rigidbody
         *
         * @param rb to apply forces
         */
        void ApplyForces(RigidBody& rb);

        /**
         * @brief detect collisions in a world
         *
         * @param scene to check
         */
        void DetectCollisions(PhysicsScene& scene);

        /**
         * @brief handle collision resolution for a rigidbody
         *
         * @param rb to handle resolution
         */
        void ResolveCollisions(RigidBody& rb);

        /**
         * @brief Set the Time Step object
         *
         * @param step to set
         */
        void SetTimeStep(double step);

        /**
         * @brief Get the Time Step object
         *
         * @return double time step
         */
        double GetTimeStep() const;

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
        double m_timeStep = 1.0 / 60.0;

        /**
         * @brief react physics common object
         *
         */
        rp3d::PhysicsCommon m_rp_common;
    };

} // namespace Vakol
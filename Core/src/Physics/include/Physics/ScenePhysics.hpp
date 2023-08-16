#pragma once

#include <reactphysics3d/reactphysics3d.h>

#include "Camera/Camera.hpp"
#include "Rendering/DebugRenderer.hpp"
#include "Terrain/Terrain.hpp"
#include "Time/Time.hpp"

namespace Vakol
{
    class System;
}

namespace Vakol::Physics
{
    /**
     * @brief Forward declaration of PhysicsPool class.
     */
    class PhysicsPool;

    /**
     * @brief Class representing a physics scene.
     */
    class ScenePhysics
    {
      public:
        ScenePhysics() = delete;
        ~ScenePhysics() = default;

        /**
         * @brief Initialize the physics scene.
         */
        void Init();

        /**
         * @brief Update the physics scene.
         *
         * @param time The time information for the update.
         */
        void Update(const Time& time);

        /**
         * @brief Enable or disable physics debugging.
         *
         * @param enable Flag indicating whether to enable or disable debugging.
         */
        void EnableDebug(bool enable);

        /**
         * @brief Check if physics debugging is enabled.
         *
         * @return bool True if debugging is enabled, false otherwise.
         */
        bool IsDebugEnabled() const;

        /**
         * @brief Add a terrain to the physics scene.
         *
         * @param terrain The terrain to add.
         */
        void AddTerrain(const Terrain& terrain);

        /**
         * @brief Custom collision callback class.
         */
        class MyCollisionCallback : public rp3d::EventListener
        {
          public:
            /**
             * @brief Override function for handling collision events.
             *
             * @param callbackData The collision callback data.
             */
            virtual void onContact(const rp3d::CollisionCallback::CallbackData& callbackData) override;
        };

      private:
        /**
         * @brief Private constructor for creating a ScenePhysics instance.
         *
         * @param newWorld The physics world for the scene.
         */
        ScenePhysics(rp3d::PhysicsWorld* newWorld);

        rp3d::RigidBody* m_Terrain;
        rp3d::PhysicsWorld* m_World;
        static MyCollisionCallback m_callback;

        double m_timestep = 1.0f / 60.0f;

        double m_accumulator = 0.0f;

        /**
         * @brief Debug renderer for physics.
         *
         */
        DebugRenderer m_DebugRenderer;

        friend class PhysicsPool;
        friend class System;
    };
} // namespace Vakol::Physics
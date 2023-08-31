#pragma once

#include "CollisionListener.hpp"
#include "PhysicsTypes.hpp"
#include "Rendering/RenderData.hpp"

#include <crossguid/guid.hpp>

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
        RigidBody CreateRigidBody(const Math::Vec3& pos = Math::Vec3(0.00f),
                                  const Math::Quat& orientation = Math::Quat(1.0f, 0.0f, 0.0f, 0.0f)) const;

        /**
         * @brief Get the Guid object
         *
         * @return const xg::Guid& guid of scene
         */
        [[nodiscard]] const xg::Guid& GetGuid() const;

        void CreateDebugScene();
        void UpdateDebugScene() const;
        const Rendering::DebugScene& GetDebugScene() const;

        bool IsDebugEnabled() const;

      private:
        bool m_debugEnabled = true;

        Rendering::DebugScene m_debugScene;
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
        void Update(double timeStep) const;

        /**
         * @brief rp3d physics world ptr
         *
         */
        rp3d::PhysicsWorld* m_world = nullptr;

        /**
         * @brief collision listener
         *
         */
        static CollisionListener m_collisionListener;

        /**
         * @brief guid of scene
         *
         */
        xg::Guid m_guid;

        friend class PhysicsEngine;
    };
} // namespace Vakol
#pragma once

#include <reactphysics3d/reactphysics3d.h>

#include "Physics/PhysicsTypes.hpp"

namespace Vakol
{

    /**
     * @brief Collision listener class.
     *
     */
    class CollisionListener : public rp3d::EventListener
    {
      public:
        /**
         * @brief On Contact event.
         *
         * @param data from react physics
         */
        virtual void onContact(const rp3d::CollisionCallback::CallbackData& data) override;

        void setOnCollisionCallback(std::function<void(RigidBody&, RigidBody&)> callback);

      private:
        std::function<void(RigidBody&, RigidBody&)> m_onCollisionCallback;
    };
} // namespace Vakol
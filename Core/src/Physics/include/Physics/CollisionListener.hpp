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
        virtual void onContact(const CallbackData& data) override;
    };
} // namespace Vakol
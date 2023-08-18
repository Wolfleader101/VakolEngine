#pragma once

#include <reactphysics3d/reactphysics3d.h>

#include "Physics/PhysicsTypes.hpp"

namespace Vakol
{
    class CollisionListener : public rp3d::EventListener
    {
      public:
        virtual void onContact(const rp3d::CollisionCallback::CallbackData& data) override;
    };
}
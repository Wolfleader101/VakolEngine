#pragma once

#include <reactphysics3d/reactphysics3d.h>

namespace Vakol::Controller::Physics {
    class ScenePhysics;

    class PhysicsPool {
       public:
        PhysicsPool() = delete;

        static ScenePhysics CreatePhysicsWorld();
        static void DeletePhysicsWorld();

       private:
        static rp3d::PhysicsCommon m_Common;
        friend class ScenePhysics;
    };
};  // namespace Vakol::Controller::Physics
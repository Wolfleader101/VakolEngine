#pragma once

#include <reactphysics3d/reactphysics3d.h>

class System;

namespace Vakol::Controller::Physics {
    class ScenePhysics;

    class PhysicsPool {
       public:
        PhysicsPool() = delete;

        static ScenePhysics CreatePhysicsWorld();
        static void DeletePhysicsWorld(ScenePhysics& toDestroy);

       private:
        static rp3d::PhysicsCommon m_Common;

        friend class ScenePhysics;
        friend class System;
    };
};  // namespace Vakol::Controller::Physics
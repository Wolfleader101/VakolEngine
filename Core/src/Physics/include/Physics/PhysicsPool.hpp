#pragma once

#include <reactphysics3d/reactphysics3d.h>

namespace Vakol
{
    class System;

}

namespace Vakol::Physics
{
    class ScenePhysics;
    /**
     * @brief Class representing a pool of physics worlds.
     */
    class PhysicsPool
    {
      public:
        PhysicsPool() = delete;

        /**
         * @brief Create a new physics world.
         *
         * @return ScenePhysics The created physics world.
         */
        static ScenePhysics CreatePhysicsWorld();

        /**
         * @brief Delete a physics world.
         *
         * @param toDestroy The physics world to delete.
         */
        static void DeletePhysicsWorld(ScenePhysics& toDestroy);

      private:
        /**
         * @brief The common physics object used by the pool.
         */
        static rp3d::PhysicsCommon m_Common;

        friend class ScenePhysics;
        friend class System;
    };
}; // namespace Vakol::Physics
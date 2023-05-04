#pragma once

#include <reactphysics3d/reactphysics3d.h>

#include <Controller/Camera.hpp>
#include <Controller/Terrain.hpp>
#include <Controller/Time.hpp>
#include <Model/gl/GLShader.hpp>

class System;

namespace Vakol::Controller::Physics {
    class PhysicsPool;
    class Vakol::View::DebugRenderer;

    using Terrain = Vakol::Controller::Terrain;

    class ScenePhysics {
       public:
        ScenePhysics() = delete;
        ~ScenePhysics();
        void Init();

        void Update(const Vakol::Controller::Time& time, const Vakol::Controller::Camera& camera);

        void AddTerrain(Terrain& terrain);

       private:
        ScenePhysics(rp3d::PhysicsWorld* newWorld);

        rp3d::RigidBody* m_Terrain;
        rp3d::PhysicsWorld* m_World;

        float m_timestep = 1.0f / 60.0f;

        float m_accumulator = 0.0f;

        friend class PhysicsPool;
        friend class System;
        friend class Vakol::View::DebugRenderer;
    };
}  // namespace Vakol::Controller::Physics

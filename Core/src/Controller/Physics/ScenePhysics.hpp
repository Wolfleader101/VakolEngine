#pragma once

#include <reactphysics3d/reactphysics3d.h>

#include <memory>

#include <Controller/Time.hpp>

namespace Vakol::Controller::Physics
{
    class PhysicsPool;

	class ScenePhysics
	{
		public:

			ScenePhysics() = delete;
			void Init();

			void Update(const Vakol::Controller::Time& time);

			void SetGrav(); //..just an example, potentially more
            void GetGrav();
		
		private:

			ScenePhysics(rp3d::PhysicsWorld*& newWorld);

			
			rp3d::PhysicsWorld* m_World;

			float m_timestep = 1.0f / 60.0f; 
			//not static as we might want to change timesteps seperately

			float m_accumulator = 0.0f;
			friend class PhysicsPool;
	};
}
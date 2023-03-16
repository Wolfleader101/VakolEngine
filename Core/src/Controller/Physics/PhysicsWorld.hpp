#include <reactphysics3d/reactphysics3d.h>

#include <memory>

#include <Controller/Time.hpp>
#include <Model/Entity.hpp>

namespace Vakol::Controller::Physics
{
    class PhysicsCommon;

	class PhysicsWorld
	{
		using Time = Vakol::Controller::Time;
		public:

			PhysicsWorld() = delete;
			void Init();

			void Update(const Time& time);

			void SetGrav(); //..just an example, potentially more
            void GetGrav();
		
		private:

			PhysicsWorld(std::unique_ptr<rp3d::PhysicsWorld> newWorld);

			
			std::unique_ptr<rp3d::PhysicsWorld> m_World;

			friend class PhysicsCommon;
	};
}
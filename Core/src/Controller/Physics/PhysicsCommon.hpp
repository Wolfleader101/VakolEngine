#include <memory>

#include <reactphysics3d/reactphysics3d.h>

namespace Vakol::Controller::Physics
{
    class PhysicsWorld;

	class PhysicsWorldCommon
	{
		public:
			
			PhysicsWorldCommon() = delete;

			static PhysicsWorld CreatePhysicsWorld(); //would make world with unique ptr
            static void DeletePhysicsWorld();
			
		
		private:

			

			static rp3d::PhysicsCommon m_Common; 
                 friend class PhysicsWorld;
	};
}
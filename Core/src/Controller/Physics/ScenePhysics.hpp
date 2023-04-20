#pragma once

#include <reactphysics3d/reactphysics3d.h>


#include <Controller/Time.hpp>
#include <Controller/Terrain.hpp>

#include <Model/gl/GLShader.hpp>

#include <Controller/Camera.hpp>

class System;


namespace Vakol::Controller::Physics
{
    class PhysicsPool;

	using Terrain = Vakol::Controller::Terrain;


	class ScenePhysics
	{
		public:

			

			ScenePhysics() = delete;
            ~ScenePhysics();
			void Init();

			void Update(const Vakol::Controller::Time& time, const Vakol::Controller::Camera& camera);

			void AddTerrain(const std::shared_ptr<Terrain>& terrain);

			
			std::shared_ptr<Model::Assets::Shader> shader;
			
		
		private:

			ScenePhysics(rp3d::PhysicsWorld* newWorld);

			rp3d::RigidBody* m_Terrain;
			rp3d::PhysicsWorld* m_World;

			void EnableDebug()
			{
				m_World->setIsDebugRenderingEnabled(true);
				rp3d::DebugRenderer& debugRenderer = m_World->getDebugRenderer();
				debugRenderer.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
				//debugRenderer.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLIDER_AABB, true);
			}


			float m_timestep = 1.0f / 60.0f; 

			float m_accumulator = 0.0f;
			friend class PhysicsPool;
			friend class System;
	};
}

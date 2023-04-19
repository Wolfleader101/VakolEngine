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

			struct PhysicsDebugVertex {
				float x{ 0 }; /**< vertice x*/
				float y{ 0 }; /**< vertice y*/
				float z{ 0 }; /**< vertice z*/
				float r{ 1 }; /**< vertice r*/
				float g{ 0 }; /**< vertice g*/
				float b{ 0 }; /**< vertice b*/
			};

			ScenePhysics() = delete;
            ~ScenePhysics();
			void Init();

			void Update(const Vakol::Controller::Time& time, const Vakol::Controller::Camera& camera);

			void AddTerrain(const std::shared_ptr<Terrain>& terrain);

			glm::vec3 GetDebugColor(rp3d::uint32 color);

			std::vector<PhysicsDebugVertex> GetDebugTriangles();
			std::vector<PhysicsDebugVertex> GetDebugLines();

			bool debug = true;

			
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

			unsigned vao = 0;
			unsigned vbo = 0;


			float m_timestep = 1.0f / 60.0f; 

			float m_accumulator = 0.0f;
			friend class PhysicsPool;
			friend class System;
	};
}

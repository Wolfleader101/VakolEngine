#pragma once

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <reactphysics3d/reactphysics3d.h>

#include <memory>
#include <vector>

#include "Controller/Logger.hpp"
#include "Model/Assets/Shader.hpp"

#include "View/Renderer/Renderer.hpp"

namespace Vakol::Controller::Physics
{
    class ScenePhysics;
}

namespace Vakol::View
{

    using Shader = Vakol::Model::Assets::Shader;
    class DebugRenderer
    {
        public:

            void SetShader(const std::shared_ptr<Shader>& shader);

            void Enable(bool enable);
            bool IsEnabled();

            void Update();
            void Draw(const Controller::Camera& camera) const;


        private:

            DebugRenderer() = default;
            DebugRenderer(rp3d::PhysicsWorld* WorldPtr);
            struct PhysicsDebugVertex {
                glm::vec3 xyz;
                glm::vec3 color;   
		    };

            void EnableWorldDebug();
            void DisableWorldDebug();

            void GetDebugColor(const uint32_t& color, glm::vec3& outColor) const;

            void GetTriangles();
            void GetLines();

            std::vector<PhysicsDebugVertex> m_DebugData;

            rp3d::PhysicsWorld* m_World = nullptr;
            std::shared_ptr<Shader> m_Shader = nullptr;
            rp3d::DebugRenderer* m_rp3dRenderer = nullptr;

            uint32_t m_VAO, m_VBO;

            friend class Vakol::Controller::Physics::ScenePhysics;
    };

}

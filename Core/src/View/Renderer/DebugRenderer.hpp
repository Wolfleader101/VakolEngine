#pragma once

#include <glad/glad.h>
#include <glm/vec3.hpp>

#include <memory>
#include <vector>

#include "Controller/Physics/ScenePhysics.hpp"
#include "Controller/Logger.hpp"

#include "View/Renderer/Renderer.hpp"


using namespace Vakol::Controller::Physics;

namespace Vakol::View
{

    
    class DebugRenderer
    {
        public:

            DebugRenderer() = default;

            DebugRenderer(const std::shared_ptr<ScenePhysics>& scenePhysics, const std::shared_ptr<Shader>& shader);

            void SetScenePhysics(const std::shared_ptr<ScenePhysics> scenePhysics);
            void UnsetScenePhysics();

            void Update();
            void Draw(const Controller::Camera& camera) const;

            bool ScenePhysicsSet() const;

        private:

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

            std::shared_ptr<ScenePhysics> m_scenePhysics = nullptr;
            std::shared_ptr<Shader> m_Shader = nullptr;
            rp3d::DebugRenderer* m_rp3dRenderer;

            uint32_t m_VAO, m_VBO;
    };

}


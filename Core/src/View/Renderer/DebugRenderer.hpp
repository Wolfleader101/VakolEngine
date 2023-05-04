#pragma once

#include <glad/glad.h>

#include <memory>

#include "Controller/Physics/ScenePhysics.hpp"

#include "View/Renderer/Renderer.hpp"
#include "Controller/Logger.hpp"

using namespace Vakol::Controller::Physics;

namespace Vakol::View
{
    class DebugRenderer
    {
        public:
            void SetScenePhysics(const std::shared_ptr<ScenePhysics> scenePhysics);
            void UnsetScenePhysics();
            void Update();
            void Draw() const;

            bool ScenePhysicsSet() const;

        private:

            bool m_ScenePhysicsTiedSet = false;
            std::shared_ptr<ScenePhysics> m_scenePhysics = nullptr;
            std::shared_ptr<Shader> sdfsdf = nullptr;
    };

}


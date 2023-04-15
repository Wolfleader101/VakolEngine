#pragma once

#include "Model/Assets/Shader.hpp"
#include "Model/VertexArray.hpp"
#include "Renderer.hpp"

namespace Vakol::View {
    class GLRenderer : public Renderer {
       public:
        GLRenderer(const std::shared_ptr<Window> window);

        void Draw(const Controller::Time& time, const Controller::Camera& camera, const Model::Components::Transform,
                  const Model::Components::Drawable& drawable) const override;

        void Update() const override;
    };
}  // namespace Vakol::View

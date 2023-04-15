#pragma once

#include "Model/Assets/Shader.hpp"
#include "Model/VertexArray.hpp"
#include "Renderer.hpp"

namespace Vakol::View {
    class GLRenderer : public Renderer {
       public:
        GLRenderer(const std::shared_ptr<Window> window);

        void ClearColor(const glm::vec4& color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) const override;
        void ClearColor(const float r, const float g, const float b, const float a = 1.0f) const override;

        void ClearBuffer(const unsigned int buffer_bit) const override;

        void Draw(const Controller::Time& time, const Controller::Camera& camera, const Model::Components::Transform,
                  const Model::Components::Drawable& drawable) const override;

        void Update() const override;
    };
}  // namespace Vakol::View

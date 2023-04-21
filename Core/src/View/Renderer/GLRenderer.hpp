#pragma once

#include "Renderer.hpp"

namespace Vakol::View {
    class GLRenderer : public Renderer {
       public:
        GLRenderer(const std::shared_ptr<Window> window);

        void AddUniform(const int size, const int binding) override;
        void SetUniformData(const int index, const int offset, const int size, const void* data) const override;

        void ClearColor(const glm::vec4& color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) const override;
        void ClearColor(const float r, const float g, const float b, const float a = 1.0f) const override;

        void ClearBuffer(const unsigned int buffer_bit) const override;

        void Draw(const Controller::Time& time, const Controller::Camera& camera, const Model::Components::Transform,
                  const Model::Components::Drawable& drawable) const override;

        void Update() const override;

        void ToggleWireframe() override {
            isWireframe = !isWireframe;

            if (isWireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
        }
    };
}  // namespace Vakol::View

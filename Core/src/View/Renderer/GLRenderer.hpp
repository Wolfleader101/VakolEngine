#pragma once

#include "Model/VertexArray.hpp"
#include "Model/Assets/Shader.hpp"
#include "Renderer.hpp"

namespace Vakol::View {
    class GLRenderer : public Renderer {
       public:
        GLRenderer(const std::shared_ptr<Window> window);
        // void Draw(const Controller::Time& time,
        //           const std::shared_ptr<Model::Components::Drawable> drawable) override;
        void Update(Controller::Camera& cam) override;

       private:
        std::shared_ptr<VertexArray> m_vertexArray = nullptr;
        std::shared_ptr<Model::Assets::Shader> m_shader = nullptr;
    };
}  // namespace Vakol::View

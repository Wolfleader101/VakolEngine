#pragma once

#include "Renderer.hpp"

namespace Vakol::View {
    class GLRenderer : public Renderer {
       public:
        GLRenderer(const std::shared_ptr<Window> window);
        void Draw(const Controller::Time& time,
                  const std::shared_ptr<Model::Components::Drawable> drawable) override;
        void Update(const Controller::Time& time) override;
    };
}  // namespace Vakol::View

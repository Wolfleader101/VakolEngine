#pragma once

#include "Renderer.hpp"

namespace Vakol::View {
    class GLRenderer : public Renderer {
       public:
        GLRenderer(const std::shared_ptr<Window> window);

        void Draw(const Controller::Time& time, const Model::Components::Drawable& drawable) const override;
    };
}  // namespace Vakol::View

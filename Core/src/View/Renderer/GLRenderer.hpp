#pragma once

#include <Model/gl/GLModel.hpp>

#include "Renderer.hpp"

using Vakol::Model::GLModel;

namespace Vakol::View {
    class GLRenderer : public Renderer {
       public:
        GLRenderer(const std::shared_ptr<Window> window);
        void Update(const Controller::Time& time) override;

       private:
        GLModel model;
    };
}  // namespace Vakol::View

#pragma once

#include "Renderer.hpp"

namespace Vakol::View 
{
    class GLRenderer : public Renderer 
    {
    public:
        GLRenderer(const std::shared_ptr<Window> window);
        void Update(const Controller::Time& time) override;
    };
}  // namespace Vakol::View

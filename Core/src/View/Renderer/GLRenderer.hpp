#pragma once

#include "Renderer.hpp"

#include <Controller/MeshRenderer.hpp>

using Vakol::Controller::MeshRenderer;

namespace Vakol::View 
{
    class GLRenderer : public Renderer 
    {
    public:
        GLRenderer(const std::shared_ptr<Window> window);
        void Update(const Controller::Time& time) override;

    private:
        MeshRenderer render;
    };
}
#pragma once

#include "Renderer.hpp"

#include <Controller/ModelLoader.hpp>

using Vakol::Controller::ModelLoader;

namespace Vakol::View 
{
    class GLRenderer : public Renderer 
    {
    public:
        GLRenderer(const std::shared_ptr<Window> window);
        void Update(const Controller::Time& time) override;
    private:
        ModelLoader ml;
    };
}
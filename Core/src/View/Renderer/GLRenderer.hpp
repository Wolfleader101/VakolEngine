#pragma once

#include "Renderer.hpp"

#include <Model/GLModel.hpp>

using Vakol::Model::GLModel;
using Vakol::Model::GLVertexArray;

namespace Vakol::View 
{
    class GLRenderer : public Renderer 
    {
    public:
        GLRenderer(const std::shared_ptr<Window> window);
        void Update(const Controller::Time& time) override;

    private:
        unsigned int shader;
        std::shared_ptr<GLVertexArray> VAO;
    };
}
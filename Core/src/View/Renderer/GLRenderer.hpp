#pragma once

#include "Renderer.hpp"

#include <Model/GLModel.hpp>

using Vakol::Model::GLVertexArray;
using Vakol::Model::GLShader;

namespace Vakol::View 
{
    class GLRenderer : public Renderer 
    {
    public:
        GLRenderer(const std::shared_ptr<Window> window);
        void Update(const Controller::Time& time) override;

    private:
        std::shared_ptr<GLShader> shader;
        GLVertexArray VAO;
    };
}
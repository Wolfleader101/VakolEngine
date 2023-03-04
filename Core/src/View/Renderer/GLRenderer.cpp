#include "GLRenderer.hpp"

#include <glad/glad.h>

#include "Model/GLMesh.hpp"
#include "Model/GLShader.hpp"

using Vakol::Model::GLShader;

using Vakol::Model::GLVertexArray;
using Vakol::Model::Vertex;

using Vakol::Model::Math::Vec3;
using Vakol::Model::Math::Vec2;

namespace Vakol::View 
{
    GLRenderer::GLRenderer(const std::shared_ptr<Window> window) : Renderer(window) 
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);

        auto shader = std::make_shared<GLShader>("basic.vert", "basic.frag");

        std::vector<Vertex> vertices =
        {
            {Vec3(0.5f, -0.5f, 0.0f),   Vec3(0.0f, 0.0f, 0.0f), Vec2(0.0f, 0.0f)},
            {Vec3(-0.5f, -0.5f, 0.0f),  Vec3(0.0f, 0.0f, 0.0f), Vec2(1.0f, 0.0f)},
            {Vec3(0.0f, 0.5f, 0.0f),    Vec3(0.0f, 0.0f, 0.0f), Vec2(0.5f, 1.0f)}
        };

        std::vector<unsigned int> indices = { 0, 1, 3 };

        auto vao = std::make_shared<GLVertexArray>(vertices, indices);
    };

    void GLRenderer::Update(const Controller::Time& time) 
    {
        glClearColor(0.52941f, 0.80784f, 0.92157f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    };
}
#include "GLRenderer.hpp"

#include <glad/glad.h>

#include <Model/Math/Vector3.hpp>
#include <Model/Math/Vector2.hpp>

#include <vector>
#include <iostream>

using Vakol::Model::GLMesh;
using Vakol::Model::DRAW_TYPE;

using Vakol::Model::Vertex;
using Vakol::Model::GLVertexArray;

using Vakol::Model::Math::Vec3;
using Vakol::Model::Math::Vec2;

namespace Vakol::View 
{
    GLRenderer::GLRenderer(const std::shared_ptr<Window> window) : Renderer(window) 
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);

        shader = std::make_shared<GLShader>("basic.vert", "basic.frag");

        std::vector<Vertex> vertices = { 
            {
                {Vec3( 0.5f,   0.5f, 0.0f)},
                {Vec3( 0.5f,  -0.5f, 0.0f)},
                {Vec3(-0.5f,  -0.5f, 0.0f)},
                {Vec3(-0.5f,   0.5f, 0.0f)}
            }
        };

        std::vector<unsigned int> indices =
        {
            0, 1, 3,
            1, 2, 3
        };

        VAO = std::make_shared<GLVertexArray>(vertices, indices);
    };

    void GLRenderer::Update(const Controller::Time& time) 
    {
        glClearColor(0.52941f, 0.80784f, 0.92157f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->Bind();
        
        VAO->Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    };
}
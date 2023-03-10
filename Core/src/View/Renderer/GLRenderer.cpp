#include "GLRenderer.hpp"

#include <glad/glad.h>

#include <Model/Math/Vector3.hpp>
#include <Model/Math/Vector2.hpp>

#include <vector>
#include <iostream>

using Vakol::Model::GLMesh;

using Vakol::Model::Vertex;
using Vakol::Model::GLVertexArray;

using Vakol::Model::GLShader;
using Vakol::Model::GLMaterial;

using Vakol::Model::Math::Vec3;
using Vakol::Model::Math::Vec2;

namespace Vakol::View 
{
    GLRenderer::GLRenderer(const std::shared_ptr<Window> window) : Renderer(window) 
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);

        GLShader shader("assets/basic.vert", "assets/basic.frag");

        std::vector<Vertex> vertices = { 
            {
                {Vec3( 0.5f,   0.5f, 0.0f), Vec2(1.0f, 1.0f)},
                {Vec3( 0.5f,  -0.5f, 0.0f), Vec2(1.0f, 0.0f)},
                {Vec3(-0.5f,  -0.5f, 0.0f), Vec2(0.0f, 0.0f)},
                {Vec3(-0.5f,   0.5f, 0.0f), Vec2(0.0f, 1.0f)}
            }
        };

        std::vector<unsigned int> indices =
        {
            0, 1, 3,
            1, 2, 3
        };

        auto mesh = GLMesh(GLVertexArray(vertices, indices));

        model.AddMesh(mesh, GLMaterial(shader));
    };

    void GLRenderer::Update(const Controller::Time& time) 
    {
        glClearColor(0.52941f, 0.80784f, 0.92157f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        model.Draw(GL_SHADER);

        model.Draw(GL_VERTEX_ARRAY);
    };
}
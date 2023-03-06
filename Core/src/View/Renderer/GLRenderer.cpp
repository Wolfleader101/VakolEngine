#include "GLRenderer.hpp"

#include <glad/glad.h>

#include <Model/Math/Vector3.hpp>
#include <Model/Math/Vector2.hpp>

#include <vector>
#include <iostream>

using Vakol::Model::GLShader;
using Vakol::Model::GLMesh;

using Vakol::Model::DRAW_TYPE;

using Vakol::Model::Vertex;

using Vakol::Model::GLVertexArray;

using Vakol::Model::Math::Vec3;
using Vakol::Model::Math::Vec2;

using Vakol::Model::GLMaterial;

namespace Vakol::View 
{
    GLRenderer::GLRenderer(const std::shared_ptr<Window> window) : Renderer(window) 
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);

        shader = GLShader("basic.vert", "basic.frag").GetID();

        std::vector<Vertex> vertices = { 
            {
                {Vec3(-0.5f,  -0.5f, 0.0f)},
                {Vec3( 0.5f,  -0.5f, 0.0f)},
                {Vec3( 0.0f,   0.5f, 0.0f)}
            }
        };

        std::vector<unsigned int> indices = 
        {
            0, 1, 3
        };

        VAO = GLVertexArray(vertices, indices);

        std::cout << VAO.GetID() << std::endl;

        //glGenVertexArrays(1, &vao);
        //glGenBuffers(1, &vbo);
        //glGenBuffers(1, &ibo);

        //glBindVertexArray(vao);

        //glBindBuffer(GL_ARRAY_BUFFER, vbo);
        //glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        //glEnableVertexAttribArray(0);
        //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        //glBindVertexArray(0);
    };

    void GLRenderer::Update(const Controller::Time& time) 
    {
        glClearColor(0.52941f, 0.80784f, 0.92157f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader);
        

        glBindVertexArray(VAO.GetID());
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    };
}
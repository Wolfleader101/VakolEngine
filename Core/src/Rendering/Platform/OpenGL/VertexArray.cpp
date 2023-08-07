#include "VertexArray.hpp"

#include <glad/glad.h>

#include "Rendering/RenderCommand.hpp"
#include "Rendering/RenderData.hpp"

namespace Vakol::Rendering::OpenGL
{
    void GenerateVertexArray(const void* vertexData, const void* indexData, Rendering::VertexCommand& command)
    {
        unsigned int ebo = 0;

        glGenVertexArrays(1, &command.vao);
        glGenBuffers(1, &command.vbo);

        if (command.nIndices > 0)
            glGenBuffers(1, &ebo);

        glBindVertexArray(command.vao);

        glBindBuffer(GL_ARRAY_BUFFER, command.vbo);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(command.nVertices * sizeof(Vertex)), vertexData, GL_STATIC_DRAW);

        if (command.nIndices > 0) 
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(command.nIndices * sizeof(unsigned int)), indexData, GL_STATIC_DRAW);
        }

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, normal)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, uv)));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, tangent)));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, bitangent)));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}
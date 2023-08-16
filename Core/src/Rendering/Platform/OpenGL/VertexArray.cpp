#include "Platform/OpenGL/VertexArray.hpp"

#include <glad/glad.h>

#include "Rendering/RenderCommand.hpp"
#include "Rendering/RenderData.hpp"

namespace Vakol::Rendering::OpenGL
{
    void GenerateVertexArray(const void* vertexData, const void* indexData, VertexCommand& command)
    {
        unsigned int ebo = 0;

        glGenVertexArrays(1, &command.vertexArray);
        glGenBuffers(1, &command.vertexBuffer);

        if (command.nIndices > 0)
            glGenBuffers(1, &ebo);

        glBindVertexArray(command.vertexArray);

        glBindBuffer(GL_ARRAY_BUFFER, command.vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(command.nVertices * sizeof(Vertex)), vertexData,
                     GL_STATIC_DRAW);

        if (command.nIndices > 0)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(command.nIndices * sizeof(unsigned int)),
                         indexData, GL_STATIC_DRAW);
        }

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              reinterpret_cast<const void*>(offsetof(Vertex, normal)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              reinterpret_cast<const void*>(offsetof(Vertex, uv)));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              reinterpret_cast<const void*>(offsetof(Vertex, tangent)));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              reinterpret_cast<const void*>(offsetof(Vertex, bitangent)));

        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, boneIDs)));

        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              reinterpret_cast<const void*>(offsetof(Vertex, boneWeights)));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void BindVertexArray(const unsigned vertexArray)
    {
        glBindVertexArray(vertexArray);
    }

    void UnbindVertexArray()
    {
        glBindVertexArray(0);
    }

} // namespace Vakol::Rendering::OpenGL
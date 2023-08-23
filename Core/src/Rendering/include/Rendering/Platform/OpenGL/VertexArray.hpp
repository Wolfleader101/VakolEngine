#pragma once

namespace Vakol::Rendering
{
    struct VertexCommand;
}

namespace Vakol::Rendering::OpenGL
{
    void GenerateVertexArray(const void* vertexData, const void* indexData, VertexCommand& command);
    void GenerateSkyboxVertexArray(const void* vertexData, VertexCommand& command);

    void BindVertexArray(unsigned int vertexArray);
    void UnbindVertexArray();
} // namespace Vakol::Rendering::OpenGL
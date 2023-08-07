#pragma once


namespace Vakol::Rendering {
    struct VertexCommand;
}

namespace Vakol::Rendering::OpenGL
{

    void GenerateVertexArray(const void* vertexData, const void* indexData, VertexCommand& command);
}  // namespace Vakol::Rendering::OpenGL
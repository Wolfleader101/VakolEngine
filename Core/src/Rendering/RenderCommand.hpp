#pragma once

#include <string>

namespace Vakol::Rendering
{
    struct VertexCommand
    {
        int nVertices = 0;
        int nIndices = 0;

        unsigned int vertexArray = 0u;
        unsigned int vertexBuffer = 0u;
    };

    [[nodiscard]] std::string GenerateID();
}

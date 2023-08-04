#pragma once

namespace Vakol::Rendering
{
    struct VertexCommand
    {
        int nVertices = 0;
        int nIndices = 0;

        unsigned int vao = 0u;
        unsigned int vbo = 0u;
    };

    struct ShaderCommand
    {
        unsigned int program = 0u;  // the shader program id
    };

    struct TextureCommand
    {
        unsigned int texture = 0u;  // the texture id
    };
}

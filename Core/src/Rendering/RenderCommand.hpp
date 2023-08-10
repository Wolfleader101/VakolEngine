#pragma once


namespace Vakol::Rendering
{
    struct VertexCommand
    {
        int nVertices = 0;
        int nIndices = 0;

        unsigned int vertexArray = 0u;
        unsigned int vertexBuffer = 0u;
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

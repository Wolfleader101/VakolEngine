#pragma once

#include <queue>

namespace Vakol::Rendering
{
    struct VertexArray;
    struct Shader;
    struct Texture;

    struct VertexCommand;

    class RenderAPI
    {
    public:
        static void GenerateVertexCommand(VertexArray&& vertexArray);
        static void GenerateShaderCommand(Shader&& shader);
        static void GenerateTextureCommand(Texture&& texture);
    private:
        static std::queue<VertexCommand> m_vertexQueue;
    };
}



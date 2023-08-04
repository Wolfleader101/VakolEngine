#pragma once

#include "RenderQueue.hpp"

namespace Vakol::Rendering
{
    constexpr int BIT(const int x) { return 1 << x; }

    struct VertexArray;
    struct Shader;
    struct Texture;

    struct VertexCommand;

    enum VK_RENDER_DATA_HINT { VK_VERTEX_DATA, VK_SHADER_DATA, VK_TEXTURE_DATA };

    enum VK_RENDER_API_HINT { OPENGL, VULKAN, DIRECT3D, METAL };
    enum VK_BUFFER_HINT { VK_COLOR_BUFFER = BIT(0), VK_DEPTH_BUFFER = BIT(1), VK_STENCIL_BUFFER = BIT(2) };

    struct RenderSettings
    {
        VK_RENDER_API_HINT API = OPENGL;
    };

    class RenderAPI
    {
    public:
        static void ClearColor(const float color[]);
        static void Clear(unsigned int mask);

        static void GenerateVertexCommand(VertexArray&& vertexArray);
        static void GenerateShaderCommand(Shader&& shader);
        static void GenerateTextureCommand(Texture&& texture);
    private:
        static RenderQueue<VertexCommand> m_vertexQueue;

        static RenderSettings m_settings;
    };
}



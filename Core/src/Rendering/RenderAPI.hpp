#pragma once

#include "RenderQueue.hpp"

#include "Math/Math.hpp"

namespace Vakol::Rendering::Assets
{
    struct Shader;
}

namespace Vakol::Rendering
{
    constexpr int BIT(const int x) { return 1 << x; }

    struct VertexArray;

    struct Texture;

    struct VertexCommand;
    struct ShaderCommand;
    struct TextureCommand;

    struct Transform;

    enum VK_RENDER_API_HINT { OPENGL, VULKAN, DIRECT3D, METAL };
    enum VK_BUFFER_HINT { VK_COLOR_BUFFER = BIT(0), VK_DEPTH_BUFFER = BIT(1), VK_STENCIL_BUFFER = BIT(2) };

    struct RenderSettings
    {
        VK_RENDER_API_HINT API = OPENGL;
        int width = 1920;
        int height = 1080;
    };

    class RenderAPI
    {
    public:
        static void ClearColor(const float color[]);
        static void Clear(unsigned int mask);

        static void BeginDraw();
        static void EndDraw();

        // transform / model matrix
        static Math::Mat4 GetTransformMatrix(Transform& transform);

        static Math::Mat4 GetProjectionMatrix();
        static Math::Mat4 GetViewMatrix(const Math::Vec3& position, const Math::Vec3& lookDirection = Math::Vec3(0.0f, 0.0f, -1.0f), const Math::Vec3& up = Math::Vec3(0.0f, 1.0f, 0.0f));

        static void GenerateVertexCommand(VertexArray&& vertexArray);
        static void GenerateShaderCommand(Assets::Shader&& shader);
        static void GenerateTextureCommand(Texture&& texture);
    private:
        static RenderQueue<VertexCommand>  m_vertexQueue;
        static RenderQueue<ShaderCommand>  m_shaderQueue;
        static RenderQueue<TextureCommand> m_textureQueue;

        static RenderSettings m_settings;
    };
}



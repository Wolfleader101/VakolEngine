#pragma once

#include "Math/Math.hpp"

#include <map>
#include <vector>
#include <string>

namespace Vakol::Rendering::Assets
{
    struct Shader;
    struct Texture;
}

namespace Vakol::Model::Components
{
    struct Transform;
}

namespace Vakol::Rendering
{
    constexpr int BIT(const int x) { return 1 << x; }

    struct VertexArray;

    struct VertexCommand;
    struct ShaderCommand;
    struct TextureCommand;

    struct Drawable;

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
        static void EnableDepth();

        static void ClearColor(const float color[]);
        static void Clear(unsigned int mask);

        static void BeginDraw(const std::string& vertexID, const std::string& shaderID, const std::string& materialID);
        static void EndDraw();

        static void PrepareVertexArray();

        static Math::Mat4 GetModelMatrix(Model::Components::Transform& transform);

        static void GenerateVertexCommand(VertexArray&& vertexArray, const Drawable& drawable);
        static void GenerateShader(Assets::Shader&& shader, Drawable& drawable);
        static unsigned int GenerateTexture(Assets::Texture& texture, const Drawable& drawable);
    private:
        static std::map<std::string, std::vector<VertexCommand>> m_vertexLibrary;

        static RenderSettings m_settings;

        static void DefaultShaderSetup(const std::string& shaderID);
    };
}



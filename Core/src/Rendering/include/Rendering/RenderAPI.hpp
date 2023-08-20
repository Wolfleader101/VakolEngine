#pragma once

#include "Math/Math.hpp"

#include <map>
#include <string>
#include <vector>

namespace Vakol::Rendering::Assets
{
    struct Shader;
    struct Texture;
} // namespace Vakol::Rendering::Assets

namespace Vakol::Components
{
    struct Transform;
}

namespace Vakol::Rendering
{
    struct VertexArray;

    struct VertexCommand;
    struct ShaderCommand;
    struct TextureCommand;

    struct Drawable;

    enum VK_BUFFER_HINT
    {
        VK_COLOR_BUFFER = 1,
        VK_DEPTH_BUFFER = 2,
        VK_STENCIL_BUFFER = 4
    };

    struct RenderConfig
    {
        std::string API = "OPENGL";

        int width = 800;
        int height = 600;
    };

    class RenderAPI
    {
      public:
        static void SetupConfig(int width, int height, const std::string& API);

        static void EnableDepth();

        static void ClearColor(float r, float g, float b, float a = 1.0f);
        static void Clear(unsigned int mask);

        static void BeginDraw(const std::string& vertexID, const std::string& shaderID, const std::string& materialID);
        static void EndDraw();

        static void PrepareVertexArray();

        static Math::Mat4 GetModelMatrix(Components::Transform& transform);

        static void GenerateVertexCommand(VertexArray&& vertexArray, const Drawable& drawable);
        static void GenerateShader(Assets::Shader&& shader, Drawable& drawable);

        static unsigned int GenerateTexture(int width, int height, int channels, const unsigned char* pixels);

      private:
        static std::map<std::string, std::vector<VertexCommand>> m_vertexLibrary;

        static RenderConfig m_config;
    };
} // namespace Vakol::Rendering

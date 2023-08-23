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
    struct SkyboxVertexArray;

    struct VertexCommand;
    struct ShaderCommand;
    struct TextureCommand;

    struct Drawable;
    struct Skybox;

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
        static void EnableMultisample();
        static void EnableSRGB();

        static void ClearColor(float r, float g, float b, float a = 1.0f);
        static void Clear(unsigned int mask);

        static void BeginDraw(const std::string& modelID, const std::string& shaderID);
        static void EndDraw();

        static void BeginSkyboxDraw(const std::string& vertexID, const std::string& shaderID, unsigned int textureID);
        static void EndSkyboxDraw();

        static Math::Mat4 GetModelMatrix(Components::Transform& transform);

        static void GenerateVertexCommand(VertexArray&& vertexArray);
        static void GenerateVertexCommand(SkyboxVertexArray&& vertexArray);

        static void GenerateShader(Assets::Shader&& shader, Drawable& drawable);
        static void GenerateSkyboxShader(Assets::Shader&& shader, Skybox& skybox);

        static unsigned int GenerateTexture(int levels, int width, int height, int channels,
                                            const unsigned char* pixels);
        static unsigned int GenerateTexture(const std::vector<std::string>& faces);

      private:
        static std::map<std::string, VertexCommand> m_vertexLibrary;

        static RenderConfig m_config;
    };
} // namespace Vakol::Rendering

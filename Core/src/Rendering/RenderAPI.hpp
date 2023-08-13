#pragma once

#include "Math/Math.hpp"

#include <map>
#include <string>

namespace Vakol::Rendering::Assets
{
    struct Shader;
}

namespace Vakol::Model::Components
{
    struct Transform;
}

namespace Vakol::Rendering
{
    constexpr int BIT(const int x) { return 1 << x; }

    struct VertexArray;

    struct Texture;

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
        static void ClearColor(const float color[]);
        static void Clear(unsigned int mask);

        static void BeginDraw(const std::string& vertexID, const std::string& shaderID);
        static void EndDraw();

        static Math::Mat4 GetProjectionMatrix();
        static Math::Mat4 GetViewMatrix(const Math::Vec3& position, const Math::Vec3& lookDirection = Math::Vec3(0.0f, 0.0f, -1.0f), const Math::Vec3& up = Math::Vec3(0.0f, 1.0f, 0.0f));
        static Math::Mat4 GetModelMatrix(Model::Components::Transform& transform);

        static void GenerateVertexCommand(VertexArray&& vertexArray, Drawable& drawable);
        static void GenerateShader(Assets::Shader&& shader, Drawable& drawable);
        static void GenerateTexture(Texture&& texture, Drawable& drawable);
    private:
        static std::map<std::string, VertexCommand> m_vertexLibrary;

        static RenderSettings m_settings;
    };
}



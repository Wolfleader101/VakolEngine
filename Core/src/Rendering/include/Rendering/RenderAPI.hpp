#pragma once

#include "Math/Math.hpp"

#include <map>
#include <string>
#include <vector>

namespace Vakol
{
    class ShaderLibrary;
}

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
        /**
         * \brief
         * \param width
         * \param height
         * \param API
         */
        static void SetupConfig(int width, int height, const std::string& API);
        /**
         * \brief
         */
        static void EnableDepth();
        /**
         * \brief
         */
        static void EnableMultisample();
        /**
         * \brief
         */
        static void EnableSRGB();

        /**
         * \brief
         * \param r
         * \param g
         * \param b
         * \param a
         */
        static void ClearColor(float r, float g, float b, float a = 1.0f);
        /**
         * \brief
         * \param mask
         */
        static void Clear(unsigned int mask);

        /**
         * \brief
         * \param modelID
         * \param shaderID
         */
        static void BeginDraw(const std::string& modelID, const std::string& shaderID);

        /**
         * \brief
         */
        static void EndDraw();

        /**
         * \brief
         * \param vertexID
         * \param shaderID
         * \param textureID
         */
        static void BeginSkyboxDraw(const std::string& vertexID, const std::string& shaderID, unsigned int textureID);
        /**
         * \brief
         */
        static void EndSkyboxDraw();

        /**
         * \brief
         * \param transform
         * \return
         */
        static Math::Mat4 GetModelMatrix(Components::Transform& transform);

        /**
         * \brief
         * \param vertexArray
         */
        static void GenerateVertexCommand(VertexArray&& vertexArray);
        /**
         * \brief
         * \param vertexArray
         */
        static void GenerateVertexCommand(SkyboxVertexArray&& vertexArray);
        /**
         * \brief
         * \param shader
         * \param drawable
         */
        static void GenerateShader(Assets::Shader&& shader, Drawable& drawable);
        /**
         * \brief
         * \param shader
         * \param skybox
         */
        static void GenerateSkyboxShader(Assets::Shader&& shader, Skybox& skybox);
        /**
         * \brief
         * \param levels
         * \param width
         * \param height
         * \param channels
         * \param pixels
         * \return
         */
        static unsigned int GenerateTexture(int levels, int width, int height, int channels,
                                            const unsigned char* pixels);
        /**
         * \brief
         * \param textures
         * \return
         */
        static unsigned int GenerateTexture(std::vector<Assets::Texture>&& textures);
        /**
         * \brief
         * \param shaderID
         * \param shader
         */
        static void AddShader(const std::string& shaderID, unsigned int shader);
        /**
         * \brief
         * \param shader
         * \param name
         * \param value
         */
        static void SetBool(unsigned int shader, const char* name, bool value);
        /**
         * \brief
         * \param shader
         * \param name
         * \param value
         */
        static void SetInt(unsigned int shader, const char* name, int value);
        /**
         * \brief
         * \param shader
         * \param name
         * \param value
         */
        static void SetFloat(unsigned int shader, const char* name, float value);

        /**
         * \brief
         * \param shader
         * \param name
         * \param value
         */
        static void SetVec2(unsigned int shader, const char* name, const Math::Vec2& value);
        /**
         * \brief
         * \param shader
         * \param name
         * \param value
         */
        static void SetVec3(unsigned int shader, const char* name, const Math::Vec3& value);
        /**
         * \brief
         * \param shader
         * \param name
         * \param value
         */
        static void SetVec4(unsigned int shader, const char* name, const Math::Vec4& value);

        /**
         * \brief
         * \param shader
         * \param name
         * \param transpose
         * \param value
         */
        static void SetMat3(unsigned int shader, const char* name, bool transpose, const Math::Mat3& value);

        /**
         * \brief
         * \param shader
         * \param name
         * \param transpose
         * \param value
         */
        static void SetMat4(unsigned int shader, const char* name, bool transpose, const Math::Mat4& value);

      private:
        static ShaderLibrary m_shaderLibrary;
        static std::map<std::string, VertexCommand> m_vertexLibrary;

        static RenderConfig m_config;
    };
} // namespace Vakol::Rendering

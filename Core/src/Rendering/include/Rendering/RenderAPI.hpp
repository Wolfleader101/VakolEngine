#pragma once

#include "Math/Math.hpp"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace Vakol
{
    class GUID;
    class ShaderLibrary;
} // namespace Vakol

namespace Vakol::Rendering::Assets
{
    struct Material;
    struct Shader;
    struct Texture;
} // namespace Vakol::Rendering::Assets

namespace Vakol::Components
{
    struct Transform;
}

namespace Vakol::Rendering
{
    struct DebugVertex;
    struct DebugScene;
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
         * \brief setup the rendering API
         * \param width the width of a window
         * \param height the height of a window
         * \param API the third-party rendering API of the users' choosing.
         */
        static void SetupConfig(int width, int height, const std::string& API);

#pragma region RenderAPI Calls
        /**
         * \brief Enables depth testing
         */
        static void EnableDepth();
        /**
         * \brief Enabled multi-sampling (MSAA)
         */
        static void EnableMultisample();
        /**
         * \brief Enables SRGB color-grading
         */
        static void EnableSRGB();

        /**
         * \brief Enables transparent textures and materials
         */
        static void EnableBlending();

        /**
         * \brief Enable Culling of faces
         */
        static void EnableCulling();

        /**
         * \brief Cull the faces
         */
        static void CullFaces();

        /**
         * \brief clear the color buffer's color
         * \param r red
         * \param g green
         * \param b blue
         * \param a alpha
         */
        static void ClearColor(float r, float g, float b, float a = 1.0f);
        /**
         * \brief clears a buffer
         * \param mask the buffer format
         */
        static void Clear(unsigned int mask);

        /**
         * \brief
         * \param x
         * \param y
         * \param width
         * \param height
         */
        static void ResizeScreen(int x, int y, int width, int height);

#pragma endregion

        /**
         * \brief Start drawing an item
         * \param modelID the path of the model
         * \param shaderID a unique ID generated by the application
         */
        static void BeginDraw(const GUID& modelID, const std::string& shaderID);

        /**
         * \brief stops drawing an item
         */
        static void EndDraw();

        /**
         * \brief begins drawing a skybox
         * \param vertexID a unique ID generated by the application (represents the vertexArray)
         * \param shaderID a unique ID generated by the application (represents the vertexArray)
         * \param textureID the ID returned after generation by the 3rd party rendering API.
         */
        static void BeginSkyboxDraw(const std::string& vertexID, const std::string& shaderID, unsigned int textureID);

        static void BeginDebugSceneDraw(const std::string& vertexID, const std::string& shaderID);

        /**
         * \brief Get the model matrix based on a model's transform.
         * \param transform
         * \return the model-transform matrix
         */
        static Math::Mat4 GetModelMatrix(Components::Transform& transform);

        /**
         * \brief Generates a vertex command that contains the necessary info to render an item's vertices.
         * \param vertexArray the vertexArray consisting of the vertices and indices of a mesh
         */
        static void GenerateVertexCommand(VertexArray&& vertexArray);
        /**
         * \brief Generates a vertex command that contains the necessary info to render an item's vertices.
         * \param vertexArray the vertexArray consisting of the vertices and indices of a mesh
         */
        static void GenerateVertexCommand(SkyboxVertexArray&& vertexArray);

        static void GenerateEmptyDebugVertexCommand(const DebugScene& debugScene);
        static void SetDebugVertexArray(std::vector<DebugVertex>&& vertices, const DebugScene& debugScene);
        /**
         * \brief generate a shader
         * \param shader the shader
         * \param drawable the drawable component that is modified by this function
         */
        static void GenerateShader(Assets::Shader&& shader, Drawable& drawable);
        /**
         * \brief
         * \param shader
         * \param skybox
         */
        static void GenerateSkyboxShader(Assets::Shader&& shader, Skybox& skybox);

        static void GenerateDebugShader(Assets::Shader&& shader, DebugScene& debugScene);
        /**
         * \brief generate a texture
         * \param levels the number of mipmap levels
         * \param width the width of the texture
         * \param height the height of the texture
         * \param channels the number of color channels of the texture.
         * \param pixels the pixel data of the texture.
         * \return a unique texture id generated by an external rendering API
         */
        static unsigned int GenerateTexture(int levels, int width, int height, int channels,
                                            const unsigned char* pixels);
        /**
         * \brief generate a cubemap texture based on its faces
         * \param faces the paths of each face of a mesh
         * \return a unique texture id generated by an external rendering API
         */
        static unsigned int GenerateTexture(std::vector<std::string>&& faces);
        /**
         * \brief Adds a shader to the shader library
         * \param shaderID the unique id of a shader generated by the applcation.
         * \param shader the unique id of a shader program generated by an external rendering API
         */
        static void AddShader(const std::string& shaderID, unsigned int shader);

        /**
         * \brief set material properties to shader
         * \param shader the shader
         * \param material the material to send to shader
         */
        static void SetMaterial(unsigned int shader, const Assets::Material& material);

        /**
         * \brief get the shader's rendering id based on its application id.
         * \param shaderID id of the shader
         * \return the unique shader id generated by an external rendering API
         */
        static unsigned int GetShader(const std::string& shaderID);
#pragma region Shader Uniforms
        /**
         * \brief sets a bool shader uniform
         * \param shader the shader program.
         * \param name the name of the uniform bool
         * \param value the value of the uniform bool
         * \param binding
         */
        static void SetBool(unsigned int shader, const char* name, bool value, bool binding = true);
        /**
         * \brief sets a int shader uniform
         * \param shader the shader program
         * \param name the name of the uniform int
         * \param value the value of the uniform int
         * \param binding
         */
        static void SetInt(unsigned int shader, const char* name, int value, bool binding = true);
        /**
         * \brief sets a float shader uniform
         * \param shader the shader program
         * \param name the name of the uniform float
         * \param value the value of the uniform float
         * \param binding
         */
        static void SetFloat(unsigned int shader, const char* name, float value, bool binding = true);

        /**
         * \brief sets a vector2 shader uniform
         * \param shader the shader program
         * \param name the name of the uniform vector
         * \param value the value of the uniform vector
         * \param binding
         */
        static void SetVec2(unsigned int shader, const char* name, const Math::Vec2& value, bool binding = true);
        /**
         * \brief sets a vector of 3 values
         * \param shader the shader program
         * \param name the name of the uniform vector
         * \param value the value of the uniform vector
         * \param binding
         */
        static void SetVec3(unsigned int shader, const char* name, const Math::Vec3& value, bool binding = true);
        /**
         * \brief sets a vector of 4 values
         * \param shader the shader program
         * \param name the name of the uniform vector
         * \param value the value of the uniform vector
         * \param binding
         */
        static void SetVec4(unsigned int shader, const char* name, const Math::Vec4& value, bool binding = true);

        /**
         * \brief sets a 3x3 matrix uniform
         * \param shader the shader program
         * \param name the name of the uniform matrix
         * \param transpose option to transpose the matrix.
         * \param value the value of the uniform
         * \param binding
         */
        static void SetMat3(unsigned int shader, const char* name, bool transpose, const Math::Mat3& value,
                            bool binding = true);

        /**
         * \brief sets a 4x4 matrix uniform
         * \param shader the shader program
         * \param name the name of the uniform matrix
         * \param transpose option to transpose the matrix.
         * \param value the value of the uniform
         * \param binding
         */
        static void SetMat4(unsigned int shader, const char* name, bool transpose, const Math::Mat4& value,
                            bool binding = true);
#pragma endregion

      private:
        static ShaderLibrary m_shaderLibrary;
        static std::map<std::string, VertexCommand> m_vertexLibrary;

        static RenderConfig m_config;
    };
} // namespace Vakol::Rendering
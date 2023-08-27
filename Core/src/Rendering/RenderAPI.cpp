#include "Rendering/RenderAPI.hpp"

#include "AssetLoader/AssetLoader.hpp"
#include "Rendering/RenderCommand.hpp"
#include "Rendering/RenderData.hpp"

#include "AssetLoader/ShaderLibrary.hpp"

#include "Rendering/Assets/Material.hpp"
#include "Rendering/Assets/Shader.hpp"

#include "Rendering/Platform/OpenGL/common.h"

#include "ECS/Components.hpp"
#include "Logger/Logger.hpp"

namespace Vakol::Rendering
{
    ShaderLibrary RenderAPI::m_shaderLibrary;
    std::map<std::string, VertexCommand> RenderAPI::m_vertexLibrary;

    RenderConfig RenderAPI::m_config;

    void RenderAPI::SetupConfig(const int width, const int height, const std::string& API)
    {
        m_config.width = width;
        m_config.height = height;

        m_config.API = API;
    }

    void RenderAPI::BeginDraw(const std::string& modelID, const std::string& shaderID)
    {
        const auto shader = GetShader(shaderID);

        OpenGL::BindShaderProgram(shader);

        for (const auto& mesh : AssetLoader::GetMeshes(modelID))
        {
            const auto& material = mesh.material;

            // SetMaterial(shader, material);

            for (const auto& texture : material->textures)
            {
                OpenGL::SetActiveTexture(static_cast<int>(texture.type));
                OpenGL::BindTexture(texture.ID);
            }

            const auto& vertexArray = m_vertexLibrary.at(mesh.ID);

            OpenGL::BindVertexArray(vertexArray.vertexArray);
            OpenGL::DrawTriangleElements(vertexArray.nIndices);
            OpenGL::UnbindVertexArray();
        }
    }

    void RenderAPI::EndDraw()
    {
        OpenGL::UnbindShaderProgram();
    }

    void RenderAPI::BeginSkyboxDraw(const std::string& vertexID, const std::string& shaderID,
                                    const unsigned int textureID)
    {
        OpenGL::BindShaderProgram(GetShader(shaderID));

        OpenGL::DepthLEQUAL();

        const auto& vertexArray = m_vertexLibrary.at(vertexID);

        OpenGL::SetActiveTexture(0);
        OpenGL::BindCubemapTexture(textureID);

        OpenGL::BindVertexArray(vertexArray.vertexArray);
        OpenGL::DrawTriangleArrays(vertexArray.nVertices);
        OpenGL::UnbindVertexArray();

        OpenGL::DepthLESS();
    }

    void RenderAPI::EndSkyboxDraw()
    {
        OpenGL::UnbindShaderProgram();
    }

    void RenderAPI::GenerateVertexCommand(VertexArray&& vertexArray)
    {
        VertexCommand command;

        command.nVertices = static_cast<int>(vertexArray.vertices.size());
        command.nIndices = static_cast<int>(vertexArray.indices.size());

        if (m_config.API == "OPENGL")
        {
            OpenGL::GenerateVertexArray(vertexArray.vertices.data(), vertexArray.indices.data(), command);
        }
        else if (m_config.API == "VULKAN")
        {
            VK_WARN("Vulkan rendering has not been implemented yet.");
        }
        else if (m_config.API == "DIRECT3D")
        {
            VK_WARN("Direct3D rendering has not been implemented yet.");
        }
        else if (m_config.API == "METAL")
        {
            VK_WARN("Metal rendering has not been implemented yet.");
        }

        m_vertexLibrary[vertexArray.ID] = command;

        std::vector<Vertex>().swap(vertexArray.vertices);
        std::vector<unsigned int>().swap(vertexArray.indices);
    }

    void RenderAPI::GenerateVertexCommand(SkyboxVertexArray&& vertexArray)
    {
        VertexCommand command;

        command.nVertices = static_cast<int>(vertexArray.vertices.size());
        command.nIndices = 0;

        if (m_config.API == "OPENGL")
        {
            OpenGL::GenerateSkyboxVertexArray(vertexArray.vertices.data(), command);
        }
        else if (m_config.API == "VULKAN")
        {
            VK_WARN("Vulkan rendering has not been implemented yet.");
        }
        else if (m_config.API == "DIRECT3D")
        {
            VK_WARN("Direct3D rendering has not been implemented yet.");
        }
        else if (m_config.API == "METAL")
        {
            VK_WARN("Metal rendering has not been implemented yet.");
        }

        m_vertexLibrary[vertexArray.ID] = command;

        std::vector<float>().swap(vertexArray.vertices);
    }

    void RenderAPI::GenerateShader(Assets::Shader&& shader, Drawable& drawable)
    {
        drawable.shaderID = GenerateID();

        const unsigned int program =
            OpenGL::GenerateShaderProgram(shader.vertSrc, shader.geomSrc, shader.tscSrc, shader.tseSrc, shader.fragSrc);

        AddShader(drawable.shaderID, program);
    }

    void RenderAPI::GenerateSkyboxShader(Assets::Shader&& shader, Skybox& skybox)
    {
        skybox.shaderID = GenerateID();

        const unsigned int program =
            OpenGL::GenerateShaderProgram(shader.vertSrc, shader.geomSrc, shader.tscSrc, shader.tseSrc, shader.fragSrc);

        AddShader(skybox.shaderID, program);
    }

    unsigned int RenderAPI::GenerateTexture(const int levels, const int width, const int height, const int channels,
                                            const unsigned char* pixels)
    {
        return OpenGL::GenerateTexture(levels, width, height, channels, pixels);
    }

    unsigned RenderAPI::GenerateTexture(std::vector<Assets::Texture>&& textures)
    {
        return OpenGL::GenerateTexture(std::move(textures));
    }

    void RenderAPI::EnableDepth()
    {
        if (m_config.API == "OPENGL")
        {
            OpenGL::EnableDepth();
        }
        else if (m_config.API == "VULKAN")
        {
            VK_WARN("Vulkan rendering has not been implemented yet.");
        }
        else if (m_config.API == "DIRECT3D")
        {
            VK_WARN("Direct3D rendering has not been implemented yet.");
        }
        else if (m_config.API == "METAL")
        {
            VK_WARN("Metal rendering has not been implemented yet.");
        }
    }

    void RenderAPI::EnableSRGB()
    {
        if (m_config.API == "OPENGL")
        {
            OpenGL::EnableSRGB();
        }
        else if (m_config.API == "VULKAN")
        {
            VK_WARN("Vulkan rendering has not been implemented yet.");
        }
        else if (m_config.API == "DIRECT3D")
        {
            VK_WARN("Direct3D rendering has not been implemented yet.");
        }
        else if (m_config.API == "METAL")
        {
            VK_WARN("Metal rendering has not been implemented yet.");
        }
    }

    void RenderAPI::EnableMultisample()
    {
        if (m_config.API == "OPENGL")
        {
            OpenGL::EnableMultisample();
        }
        else if (m_config.API == "VULKAN")
        {
            VK_WARN("Vulkan rendering has not been implemented yet.");
        }
        else if (m_config.API == "DIRECT3D")
        {
            VK_WARN("Direct3D rendering has not been implemented yet.");
        }
        else if (m_config.API == "METAL")
        {
            VK_WARN("Metal rendering has not been implemented yet.");
        }
    }

    void RenderAPI::EnableBlending()
    {
        if (m_config.API == "OPENGL")
        {
            OpenGL::EnableBlending();
        }
        else if (m_config.API == "VULKAN")
        {
            VK_WARN("Vulkan rendering has not been implemented yet.");
        }
        else if (m_config.API == "DIRECT3D")
        {
            VK_WARN("Direct3D rendering has not been implemented yet.");
        }
        else if (m_config.API == "METAL")
        {
            VK_WARN("Metal rendering has not been implemented yet.");
        }
    }

    void RenderAPI::EnableCulling()
    {
        if (m_config.API == "OPENGL")
        {
            OpenGL::EnableCulling();
        }
        else if (m_config.API == "VULKAN")
        {
            VK_WARN("Vulkan rendering has not been implemented yet.");
        }
        else if (m_config.API == "DIRECT3D")
        {
            VK_WARN("Direct3D rendering has not been implemented yet.");
        }
        else if (m_config.API == "METAL")
        {
            VK_WARN("Metal rendering has not been implemented yet.");
        }
    }

    void RenderAPI::CullFaces()
    {
        if (m_config.API == "OPENGL")
        {
            OpenGL::CullFaces(OpenGL::BACK_FACE());
        }
        else if (m_config.API == "VULKAN")
        {
            VK_WARN("Vulkan rendering has not been implemented yet.");
        }
        else if (m_config.API == "DIRECT3D")
        {
            VK_WARN("Direct3D rendering has not been implemented yet.");
        }
        else if (m_config.API == "METAL")
        {
            VK_WARN("Metal rendering has not been implemented yet.");
        }
    }

    void RenderAPI::ClearColor(const float r, const float g, const float b, const float a)
    {
        if (m_config.API == "OPENGL")
        {
            OpenGL::ClearColor(r, g, b, a);
        }
        else if (m_config.API == "VULKAN")
        {
            VK_WARN("Vulkan rendering has not been implemented yet.");
        }
        else if (m_config.API == "DIRECT3D")
        {
            VK_WARN("Direct3D rendering has not been implemented yet.");
        }
        else if (m_config.API == "METAL")
        {
            VK_WARN("Metal rendering has not been implemented yet.");
        }
    }

    void RenderAPI::Clear(const unsigned int mask)
    {
        if (m_config.API == "OPENGL")
        {
            if (mask == VK_COLOR_BUFFER)
                OpenGL::Clear(OpenGL::COLOR_BUFFER_BIT());
            else if (mask == VK_DEPTH_BUFFER)
                OpenGL::Clear(OpenGL::DEPTH_BUFFER_BIT());
            else if (mask == (VK_COLOR_BUFFER | VK_DEPTH_BUFFER))
                OpenGL::Clear(OpenGL::COLOR_BUFFER_BIT() | OpenGL::DEPTH_BUFFER_BIT());
        }
        else if (m_config.API == "VULKAN")
        {
            VK_WARN("Vulkan rendering has not been implemented yet.");
        }
        else if (m_config.API == "DIRECT3D")
        {
            VK_WARN("Direct3D rendering has not been implemented yet.");
        }
        else if (m_config.API == "METAL")
        {
            VK_WARN("Metal rendering has not been implemented yet.");
        }
    }

    Math::Mat4 RenderAPI::GetModelMatrix(Components::Transform& transform)
    {
        auto transform_matrix = Math::Mat4(1.0f);

        transform_matrix = Math::Translate(transform_matrix, transform.pos);

        transform.rot = Math::Quat(Math::DegToRad(transform.eulerAngles));

        const auto rotation_matrix = Math::Mat4Cast(transform.rot);

        transform_matrix = Math::Scale(transform_matrix, transform.scale);

        return transform_matrix * rotation_matrix;
    }

    void RenderAPI::SetMaterial(const unsigned int shader, const std::shared_ptr<Assets::Material>& material)
    {
        const auto& properties = material->properties;

        if (properties)
        {
            SetVec3(shader, "material.ambient_color", properties->ambient_color);
            SetVec3(shader, "material.diffuse_color", properties->diffuse_color);
            SetVec3(shader, "material.specular_color", properties->specular_color);
            SetVec3(shader, "material.emissive_color", properties->emissive_color);

            SetFloat(shader, "material.shininess", properties->shininess);
            SetFloat(shader, "material.shininess_strength", properties->shininess_strength);
            SetFloat(shader, "material.opacity", properties->opacity);
        }
    }

    void RenderAPI::AddShader(const std::string& shaderID, const unsigned int shader)
    {
        m_shaderLibrary.AddShader(shaderID, shader);
    }

    unsigned RenderAPI::GetShader(const std::string& shaderID)
    {
        return m_shaderLibrary.GetShader(shaderID);
    }

    void RenderAPI::SetBool(const unsigned int shader, const char* name, const bool value)
    {
        m_shaderLibrary.SetBool(shader, name, value);
    }

    void RenderAPI::SetInt(const unsigned int shader, const char* name, const int value)
    {
        m_shaderLibrary.SetInt(shader, name, value);
    }

    void RenderAPI::SetFloat(const unsigned shader, const char* name, const float value)
    {
        m_shaderLibrary.SetFloat(shader, name, value);
    }

    void RenderAPI::SetVec2(const unsigned int shader, const char* name, const Math::Vec2& value)
    {
        m_shaderLibrary.SetVec2(shader, name, value);
    }

    void RenderAPI::SetVec3(const unsigned int shader, const char* name, const Math::Vec3& value)
    {
        m_shaderLibrary.SetVec3(shader, name, value);
    }

    void RenderAPI::SetVec4(const unsigned int shader, const char* name, const Math::Vec4& value)
    {
        m_shaderLibrary.SetVec4(shader, name, value);
    }

    void RenderAPI::SetMat3(const unsigned int shader, const char* name, const bool transpose, const Math::Mat3& value)
    {
        m_shaderLibrary.SetMat3(shader, name, transpose, value);
    }

    void RenderAPI::SetMat4(const unsigned int shader, const char* name, const bool transpose, const Math::Mat4& value)
    {
        m_shaderLibrary.SetMat4(shader, name, transpose, value);
    }
} // namespace Vakol::Rendering

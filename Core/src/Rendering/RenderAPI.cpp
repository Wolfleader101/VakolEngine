#include "Rendering/RenderAPI.hpp"

#include "Rendering/MaterialLibrary.hpp"
#include "Rendering/RenderCommand.hpp"
#include "Rendering/RenderData.hpp"

#include "Rendering/ShaderLibrary.hpp"

#include "Rendering/Assets/Shader.hpp"

#include "Rendering/Platform/OpenGL/common.h"

#include "ECS/Components.hpp"
#include "Logger/Logger.hpp"

#include "AssetLoader/TextureLoader.hpp"

namespace Vakol::Rendering
{
    std::map<std::string, std::vector<VertexCommand>> RenderAPI::m_vertexLibrary;

    RenderConfig RenderAPI::m_config;

    void RenderAPI::SetupConfig(const int width, const int height, const std::string& API)
    {
        m_config.width = width;
        m_config.height = height;

        m_config.API = API;
    }

    void RenderAPI::BeginDraw(const std::string& vertexID, const std::string& shaderID, const std::string& materialID)
    {
        const auto program = ShaderLibrary::GetShader(shaderID);

        OpenGL::BindShaderProgram(program);

        DefaultShaderSetup(shaderID);

        std::vector<Assets::Texture> textures{};

        if (const auto valid = MaterialLibrary::GetTextures(materialID, textures); valid)
        {
            for (const auto& texture : textures)
            {
                OpenGL::SetActiveTexture(texture.type);
                OpenGL::BindTexture(texture.ID);
            }
        }

        for (const auto& vertexArray : m_vertexLibrary.at(vertexID))
        {
            OpenGL::BindVertexArray(vertexArray.vertexArray);
            OpenGL::DrawTriangleElements(vertexArray.nIndices);
        }
    }

    void RenderAPI::EndDraw()
    {
        OpenGL::UnbindVertexArray();

        OpenGL::UnbindShaderProgram();
    }

    void RenderAPI::GenerateVertexCommand(VertexArray&& vertexArray, const Drawable& drawable)
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

        m_vertexLibrary[drawable.vertexArrayID].emplace_back(command);

        std::vector<Vertex>().swap(vertexArray.vertices);
        std::vector<unsigned int>().swap(vertexArray.indices);
    }

    void RenderAPI::PrepareVertexArray()
    {
        m_vertexLibrary.emplace();
    }

    void RenderAPI::GenerateShader(Assets::Shader&& shader, Drawable& drawable)
    {
        drawable.shaderID = GenerateID();
        VK_TRACE("Shader ID: {0}", drawable.shaderID);

        const unsigned int program = OpenGL::GenerateShaderProgram(shader.sources);
        ShaderLibrary::GetShaderUniforms(program);

        ShaderLibrary::AddShader(drawable.shaderID, program);
    }

    unsigned int RenderAPI::GenerateTexture(Assets::Texture& texture, const Drawable& drawable)
    {
        if (!texture.embedded)
        {
            unsigned char* pixels = nullptr;

            ImportTexture(texture.path, texture.width, texture.height, texture.channels, pixels);

            return OpenGL::GenerateTexture(texture.width, texture.height, texture.channels, pixels);
        }

        return 0u;
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

    void RenderAPI::DefaultShaderSetup(const std::string& shaderID)
    {
        ShaderLibrary::SetInt(ShaderLibrary::GetShader(shaderID), "material.diffuse_map", Assets::VK_TEXTURE_DIFFUSE);
        ShaderLibrary::SetInt(ShaderLibrary::GetShader(shaderID), "material.specular_map", Assets::VK_TEXTURE_SPECULAR);
        ShaderLibrary::SetInt(ShaderLibrary::GetShader(shaderID), "material.ambient_map", Assets::VK_TEXTURE_AMBIENT);
        ShaderLibrary::SetInt(ShaderLibrary::GetShader(shaderID), "material.emission_map", Assets::VK_TEXTURE_EMISSION);
        ShaderLibrary::SetInt(ShaderLibrary::GetShader(shaderID), "material.height_map", Assets::VK_TEXTURE_HEIGHT);
        ShaderLibrary::SetInt(ShaderLibrary::GetShader(shaderID), "material.normal_map", Assets::VK_TEXTURE_NORMAL);
    }

    Math::Mat4 RenderAPI::GetModelMatrix(Components::Transform& transform)
    {
        auto transform_matrix = Math::Mat4(1.0f);

        transform_matrix = Math::Translate(transform_matrix, transform.pos);

        transform.rot = Math::Quat(Math::Radians(transform.eulerAngles));

        const auto rotation_matrix = Math::Mat4Cast(transform.rot);

        transform_matrix = Math::Scale(transform_matrix, transform.scale);

        return transform_matrix * rotation_matrix;
    }
} // namespace Vakol::Rendering

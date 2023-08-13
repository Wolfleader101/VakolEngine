#include "RenderAPI.hpp"

#include "RenderData.hpp"
#include "RenderCommand.hpp"

#include "ShaderLibrary.hpp"

#include "Assets/Shader.hpp"

#include "Platform/OpenGL/common.h"

#include "Controller/Logger.hpp"
#include "Model/Components.hpp"

namespace Vakol::Rendering
{
    std::map<std::string, VertexCommand> RenderAPI::m_vertexLibrary;

    RenderSettings RenderAPI::m_settings;

    void RenderAPI::GenerateVertexCommand(VertexArray&& vertexArray, Drawable& drawable)
    {
        VertexCommand command;

        drawable.vertexArrayID = GenerateID();
        VK_TRACE("Vertex Array ID: {0}", drawable.vertexArrayID);

        command.nVertices = static_cast<int>(vertexArray.vertices.size());
        command.nIndices = static_cast<int>(vertexArray.indices.size());

        switch (m_settings.API) {
            case OPENGL:
                OpenGL::GenerateVertexArray(vertexArray.vertices.data(), vertexArray.indices.data(), command);
                break;
            case VULKAN:
                VK_TRACE("Vulkan rendering has not been implemented yet.");
                break;
            case DIRECT3D:
                VK_TRACE("Direct3D rendering has not been implemented yet.");
                break;
            case METAL:
                VK_TRACE("Metal rendering has not been implemented yet.");
                break;
        }

        m_vertexLibrary.emplace(drawable.vertexArrayID, command);

        std::vector<Vertex>().swap(vertexArray.vertices);
        std::vector<unsigned int>().swap(vertexArray.indices);
    }

    void RenderAPI::GenerateShader(Assets::Shader&& shader, Drawable& drawable)
    {
        drawable.shaderID = GenerateID();
        VK_TRACE("SHADER ID: {0}", drawable.shaderID);

        const unsigned int program = OpenGL::GenerateShaderProgram(std::move(shader.sources));
        ShaderLibrary::GetShaderUniforms(program);

        ShaderLibrary::AddShader(drawable.shaderID, program);
    }

    void RenderAPI::GenerateTexture(Texture&& texture, Drawable& drawable)
    {

    }
    
    void RenderAPI::ClearColor(const float color[])
    {
        switch (m_settings.API) 
        {
            case OPENGL:
                OpenGL::ClearColor(color);
                break;
            case VULKAN:
                VK_TRACE("Vulkan rendering has not been implemented yet.");
                break;
            case DIRECT3D:
                VK_TRACE("Direct3D rendering has not been implemented yet.");
                break;
            case METAL:
                VK_TRACE("Metal rendering has not been implemented yet.");
                break;
        }
    }

    void RenderAPI::Clear(const unsigned int mask)
    {
        switch (m_settings.API) 
        {
            case OPENGL: 
            {
                if (mask == VK_COLOR_BUFFER)
                    OpenGL::Clear(OpenGL::COLOR_BUFFER_BIT());
                else if (mask == VK_DEPTH_BUFFER)
                    OpenGL::Clear(OpenGL::DEPTH_BUFFER_BIT());
                else if (mask == (VK_COLOR_BUFFER | VK_DEPTH_BUFFER))
                    OpenGL::Clear(OpenGL::COLOR_BUFFER_BIT() | OpenGL::DEPTH_BUFFER_BIT());

                break;    
            }
            case VULKAN:
                VK_TRACE("Vulkan rendering has not been implemented yet.");
                break;
            case DIRECT3D:
                VK_TRACE("Direct3D rendering has not been implemented yet.");
                break;
            case METAL:
                VK_TRACE("Metal rendering has not been implemented yet.");
                break;
        }
    }

    void RenderAPI::BeginDraw(const std::string& vertexID, const std::string& shaderID)
    {
        const auto& [nVertices, nIndices, vertexArray, vertexBuffer] = m_vertexLibrary.at(vertexID);
        const auto program = ShaderLibrary::GetShader(shaderID);

        OpenGL::BindShaderProgram(program);

        OpenGL::BindVertexArray(vertexArray);
        OpenGL::DrawTriangleElements(nIndices);

        ShaderLibrary::SetMat4(program, "PV_MATRIX", false, GetProjectionMatrix() * GetViewMatrix(Math::Vec3(0.0f, 0.0f, -5.0f)));
    }

    void RenderAPI::EndDraw()
    {
        OpenGL::UnbindVertexArray();

        OpenGL::UnbindShaderProgram();
    }

    Math::Mat4 RenderAPI::GetProjectionMatrix()
    {
        return Math::Perspective(Math::Radians(45.0f), static_cast<float>(m_settings.width) / static_cast<float>(m_settings.height), 0.01f, 1000.0f);
    }

    Math::Mat4 RenderAPI::GetViewMatrix(const Math::Vec3& position, const Math::Vec3& lookDirection, const Math::Vec3& up)
    {
        return Math::LookAt(position, lookDirection, up);
    }

    Math::Mat4 RenderAPI::GetModelMatrix(Model::Components::Transform& transform)
    {
        auto transform_matrix = Math::Mat4(1.0f);

        transform_matrix = Math::Translate(transform_matrix, transform.pos);

        transform.rot = Math::Quaternion(Math::Radians(transform.eulerAngles));

        const auto rotation_matrix = Math::Mat4Cast(transform.rot);

        transform_matrix = Math::Scale(transform_matrix, transform.scale);

        return transform_matrix * rotation_matrix;
    }
}

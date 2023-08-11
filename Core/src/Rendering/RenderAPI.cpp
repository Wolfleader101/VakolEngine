#include "RenderAPI.hpp"

#include "RenderData.hpp"
#include "RenderCommand.hpp"

#include "ShaderLibrary.hpp"

#include "Assets/Shader.hpp"

#include "Platform/OpenGL/common.h"

#include "Controller/Logger.hpp"

namespace Vakol::Rendering
{
    RenderQueue<VertexCommand>  RenderAPI::m_vertexQueue;
    RenderQueue<ShaderCommand>  RenderAPI::m_shaderQueue;
    RenderQueue<TextureCommand> RenderAPI::m_textureQueue;

    RenderSettings RenderAPI::m_settings;

    void RenderAPI::GenerateVertexCommand(VertexArray&& vertexArray)
    {
        VertexCommand command;

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

        m_vertexQueue.Emplace(command);
    }

    void RenderAPI::GenerateShaderCommand(Assets::Shader&& shader)
    {
        ShaderCommand command;

        command.program = OpenGL::GenerateShaderProgram(std::move(shader.sources));
        ShaderLibrary::GetShaderUniforms(command.program);

        ShaderLibrary::AddShader(shader.path.c_str(), command.program);

        m_shaderQueue.Emplace(command);
    }

    void RenderAPI::GenerateTextureCommand(Texture&& texture)
    {
        TextureCommand command;
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

    void RenderAPI::BeginDraw()
    {
        const auto [nVertices, nIndices, vertexArray, vertexBuffer] = m_vertexQueue.Front();
        const auto& [program] = m_shaderQueue.Front();

        OpenGL::BindShaderProgram(program);

        OpenGL::BindVertexArray(vertexArray);
        OpenGL::DrawElements(nIndices);

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

    Math::Mat4 RenderAPI::GetModelMatrix(Transform& transform)
    {
        auto transform_matrix = Math::Mat4(1.0f);

        transform_matrix = translate(transform_matrix, transform.position);

        transform.m_rotation = Math::Quaternion(radians(transform.rotation));

        const auto rotation_matrix = mat4_cast(transform.m_rotation);

        transform_matrix = scale(transform_matrix, transform.scale);

        return transform_matrix * rotation_matrix;
    }

}

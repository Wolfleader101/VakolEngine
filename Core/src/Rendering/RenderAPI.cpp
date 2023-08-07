#include "RenderAPI.hpp"

#include "RenderData.hpp"
#include "RenderCommand.hpp"

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

        m_vertexQueue.AddToQueue(command);
    }

    void RenderAPI::GenerateShaderCommand(Shader&& shader)
    {
        ShaderCommand command;
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

    Math::Mat4& RenderAPI::GetTransformMatrix(Transform& transform)
    {
        auto transform_matrix = Math::Mat4(1.0f);
    }

}

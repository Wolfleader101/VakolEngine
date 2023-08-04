#include "RenderAPI.hpp"

#include "RenderData.hpp"
#include "RenderCommand.hpp"

#include "Platform/OpenGL/common.h"

namespace Vakol::Rendering
{
    RenderQueue<VertexCommand> RenderAPI::m_vertexQueue;
    RenderSettings RenderAPI::m_settings;

    void RenderAPI::GenerateVertexCommand(VertexArray&& vertexArray)
    {
        VertexCommand command;

        command.nVertices = static_cast<int>(vertexArray.vertices.size());
        command.nIndices = static_cast<int>(vertexArray.indices.size());

        m_vertexQueue.AddToQueue(command);
    }

    void RenderAPI::GenerateShaderCommand(Shader&& shader)
    {
        
    }

    void RenderAPI::GenerateTextureCommand(Texture&& texture)
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
                break;
            case DIRECT3D:
                break;
            case METAL:
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
                break;
            case DIRECT3D:
                break;
            case METAL:
                break;
        }
    }

}

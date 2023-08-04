#include "RenderAPI.hpp"

#include "RenderData.hpp"
#include "RenderCommand.hpp"

namespace Vakol::Rendering
{
    void RenderAPI::GenerateVertexCommand(VertexArray&& vertexArray)
    {
        VertexCommand command;

        command.nVertices = static_cast<int>(vertexArray.vertices.size());
        command.nIndices = static_cast<int>(vertexArray.indices.size());

        m_vertexQueue.emplace(command);
    }

    void RenderAPI::GenerateShaderCommand(Shader&& shader)
    {
        
    }

    void RenderAPI::GenerateTextureCommand(Texture&& texture)
    {

    }
}

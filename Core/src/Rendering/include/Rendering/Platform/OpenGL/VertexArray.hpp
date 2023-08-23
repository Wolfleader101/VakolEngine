#pragma once

namespace Vakol::Rendering
{
    struct VertexCommand;
}

namespace Vakol::Rendering::OpenGL
{
    /**
     * \brief 
     * \param vertexData 
     * \param indexData 
     * \param command 
     */
    void GenerateVertexArray(const void* vertexData, const void* indexData, VertexCommand& command);
    /**
     * \brief 
     * \param vertexData 
     * \param command 
     */
    void GenerateSkyboxVertexArray(const void* vertexData, VertexCommand& command);

    /**
     * \brief 
     * \param vertexArray 
     */
    void BindVertexArray(unsigned int vertexArray);

    /**
     * \brief 
     */
    void UnbindVertexArray();
} // namespace Vakol::Rendering::OpenGL
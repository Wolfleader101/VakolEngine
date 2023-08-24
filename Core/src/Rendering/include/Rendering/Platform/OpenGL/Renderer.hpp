#pragma once

namespace Vakol::Rendering::OpenGL
{
    /**
     * \brief
     * \param nVertices
     */
    void DrawTriangleArrays(int nVertices);

    /**
     * \brief
     * \param nIndices
     * \param offset
     */
    void DrawTriangleElements(int nIndices, const void* offset = nullptr);

    /**
     * \brief
     * \param nVertices
     */
    void DrawLineArrays(int nVertices);

    /**
     * \brief
     * \param nIndices
     * \param offset
     */
    void DrawLineElements(int nIndices, const void* offset = nullptr);

    /**
     * \brief
     * \param enabled
     */
    void SetWireframe(bool enabled);
} // namespace Vakol::Rendering::OpenGL
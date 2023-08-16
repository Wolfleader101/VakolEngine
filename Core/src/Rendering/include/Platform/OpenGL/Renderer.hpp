#pragma once

namespace Vakol::Rendering::OpenGL
{
    void DrawTriangleArrays(int nVertices);
    /// <summary>
    /// Draw elements using indices
    /// </summary>
    /// <param name="nIndices"></param>
    /// <param name="offset"></param>
    void DrawTriangleElements(int nIndices, const void* offset = nullptr);

    void DrawLineArrays(int nVertices);
    void DrawLineElements(int nIndices, const void* offset = nullptr);

    void SetWireframe(bool enabled);
}
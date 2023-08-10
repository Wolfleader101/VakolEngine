#pragma once

namespace Vakol::Rendering::OpenGL
{
    /// <summary>
    /// Draw elements using indices
    /// </summary>
    /// <param name="nIndices"></param>
    /// <param name="offset"></param>
    void DrawElements(int nIndices, const void* offset = nullptr);
}
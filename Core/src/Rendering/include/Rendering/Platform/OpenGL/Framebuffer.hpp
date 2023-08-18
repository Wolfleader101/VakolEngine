#pragma once

namespace Vakol::Rendering::OpenGL
{
    unsigned int DEPTH_BUFFER_BIT();
    unsigned int COLOR_BUFFER_BIT();

    void EnableDepth();

    void ClearColor(float r, float g, float b, float a);
    void Clear(unsigned int mask);
} // namespace Vakol::Rendering::OpenGL
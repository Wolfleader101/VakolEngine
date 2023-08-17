#pragma once

namespace Vakol::Rendering::OpenGL
{
    unsigned int DEPTH_BUFFER_BIT();
    unsigned int COLOR_BUFFER_BIT();

    void EnableDepth();

    void ClearColor(const float color[4]);
    void ClearColor(float r, float g, float b, float a = 1.0f);
    void Clear(unsigned int mask);
} // namespace Vakol::Rendering::OpenGL
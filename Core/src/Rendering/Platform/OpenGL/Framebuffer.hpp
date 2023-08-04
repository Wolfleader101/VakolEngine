#pragma once

namespace Vakol::Rendering::OpenGL
{
    unsigned int DEPTH_BUFFER_BIT();
    unsigned int COLOR_BUFFER_BIT();

    void ClearColor(const float color[]);
    void ClearColor(float r, float g, float b, float a = 1.0f);
    void Clear(unsigned int mask);
}
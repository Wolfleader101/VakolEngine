#pragma once

namespace Vakol::Rendering::OpenGL
{
    unsigned int DEPTH_BUFFER_BIT();
    unsigned int COLOR_BUFFER_BIT();

    void EnableDepth();
    void EnableMultisample();
    void EnableSRGB();

    /**
     * \brief less than or equal depth function
     */
    void DepthLEQUAL();

    /**
     * \brief less than depth function (OPENGL DEFAULT)
     */
    void DepthLESS();

    void ClearColor(float r, float g, float b, float a);
    void Clear(unsigned int mask);
} // namespace Vakol::Rendering::OpenGL
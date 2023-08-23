#include "Rendering/Platform/OpenGL/Framebuffer.hpp"

#include <glad/glad.h>

namespace Vakol::Rendering::OpenGL
{
    unsigned int COLOR_BUFFER_BIT()
    {
        return GL_COLOR_BUFFER_BIT;
    }

    unsigned int DEPTH_BUFFER_BIT()
    {
        return GL_DEPTH_BUFFER_BIT;
    }

    void EnableDepth()
    {
        glEnable(GL_DEPTH_TEST);
    }

    void EnableSRGB()
    {
        glEnable(GL_FRAMEBUFFER_SRGB);
    }

    void EnableMultisample()
    {
        glEnable(GL_MULTISAMPLE);
    }

    void ClearColor(const float r, const float g, const float b, const float a)
    {
        glClearColor(r, g, b, a);
    }

    void Clear(const unsigned int mask)
    {
        glClear(mask);
    }

    void DepthLEQUAL()
    {
        glDepthFunc(GL_LEQUAL);
    }

    void DepthLESS()
    {
        glDepthFunc(GL_LESS);
    }

} // namespace Vakol::Rendering::OpenGL

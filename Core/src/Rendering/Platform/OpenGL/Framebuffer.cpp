#include "Platform/OpenGL/Framebuffer.hpp"

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

    void ClearColor(const float color[])
    {
        glClearColor(color[0], color[1], color[2], color[3]);
    }
    void ClearColor(const float r, const float g, const float b, const float a)
    {
        glClearColor(r, g, b, a);
    }
    void Clear(const unsigned int mask)
    {
        glClear(mask);
    }
} // namespace Vakol::Rendering::OpenGL

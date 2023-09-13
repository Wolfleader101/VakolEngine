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

    unsigned int FRONT_FACE()
    {
        return GL_FRONT_FACE;
    }

    unsigned int BACK_FACE()
    {
        return GL_BACK;
    }

    unsigned int FRONT_AND_BACK_FACE()
    {
        return GL_FRONT_AND_BACK;
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

    void EnableBlending()
    {
        glEnable(GL_BLEND);
        BlendFunc();
    }

    void DisableBlending()
    {
        glDisable(GL_BLEND);
    }

    void BlendFunc()
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void EnableCulling()
    {
        glEnable(GL_CULL_FACE);
        CullFaces(GL_BACK);
    }

    void CullFaces(const unsigned int face)
    {
        glCullFace(face);
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

    void Viewport(const int x, const int y, const int width, const int height)
    {
        glViewport(x, y, width, height);
    }

} // namespace Vakol::Rendering::OpenGL
#pragma once

#include <glad/glad.h>

namespace Vakol::Rendering::OpenGL
{
    void DrawElements(const int nIndices, const void* offset)
    {
        glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, offset);
    }
}
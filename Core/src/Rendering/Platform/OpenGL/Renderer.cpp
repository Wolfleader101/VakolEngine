#include "Rendering/Platform/OpenGL/Renderer.hpp"

#include <glad/glad.h>

namespace Vakol::Rendering::OpenGL
{
    void DrawTriangleArrays(const int nVertices)
    {
        glDrawArrays(GL_TRIANGLES, 0, nVertices);
    }

    void DrawTriangleElements(const int nIndices, const void* offset)
    {
        glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, offset);
    }

    void DrawLineArrays(const int nVertices)
    {
        glDrawArrays(GL_LINES, 0, nVertices);
    }

    void DrawLineElements(const int nIndices, const void* offset)
    {
        glDrawElements(GL_LINES, nIndices, GL_UNSIGNED_INT, offset);
    }

    void SetWireframe(const bool enabled)
    {
        if (enabled)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

} // namespace Vakol::Rendering::OpenGL
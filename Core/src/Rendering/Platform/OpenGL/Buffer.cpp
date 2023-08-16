#include "Platform/OpenGL/Buffer.hpp"

#include <glad/glad.h>

namespace Vakol::Rendering::OpenGL
{
    void GenBuffers(unsigned int& buffer)
    {
        glGenBuffers(1, &buffer);
    }

    void BindUniformBuffer(const unsigned int buffer)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, buffer);
    }

    void UnbindUniformBuffer()
    {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void BindUniformBufferBase(const unsigned int binding, const unsigned int buffer)
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, buffer);
    }

    void BindUniformBufferRange(const unsigned int binding, const unsigned int buffer, const int offset, const int size)
    {
        glBindBufferRange(GL_UNIFORM_BUFFER, binding, buffer, offset, size);
    }

    void SetUniformBufferData(const unsigned int buffer, const int size, const void* data)
    {
        glNamedBufferData(buffer, size, data, GL_STATIC_DRAW);
    }

    void SetUniformBufferSubData(const unsigned int buffer, const int offset, const int size, const void* data)
    {
        
    }

}
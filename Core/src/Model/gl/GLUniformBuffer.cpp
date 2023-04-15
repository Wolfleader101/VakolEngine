#include "GLUniformBuffer.hpp"

#include <glad/glad.h>

#include <Controller/Logger.hpp>

namespace Vakol::Model::Assets
{
    GLUniformBuffer::GLUniformBuffer(const int size, const int binding) : UniformBuffer(size, binding)
    {
        glCreateBuffers(1, &this->m_ID);
        glNamedBufferData(this->m_ID, size, nullptr, GL_STATIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, this->m_ID);

        VK_TRACE(this->m_ID);
    }

    GLUniformBuffer::~GLUniformBuffer()
    {
        glDeleteBuffers(1, &this->m_ID);
    }

    void GLUniformBuffer::Bind() const
    {
        glBindBuffer(GL_UNIFORM_BUFFER, this->m_ID);
    }

    void GLUniformBuffer::Unbind() const
    {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void GLUniformBuffer::SetData(const void* data, const int size, const int offset)
    {
        glNamedBufferSubData(this->m_ID, offset, size, data);
    }   
}

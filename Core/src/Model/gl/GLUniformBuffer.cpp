#include "GLUniformBuffer.hpp"

#include <glad/glad.h>

#include <Controller/Logger.hpp>

namespace Vakol::Model::Assets
{
    GLUniformBuffer::GLUniformBuffer(const int size, const int binding) : UniformBuffer(size, binding)
    {
        glGenBuffers(1, &this->m_ID);
        
        this->Bind();
        glNamedBufferData(this->m_ID, size, nullptr, GL_STATIC_DRAW);
        this->Unbind();

        //glBindBufferBase(GL_UNIFORM_BUFFER, binding, this->m_ID);

        // define the range of the buffer that links to a uniform binding point
        glBindBufferRange(GL_UNIFORM_BUFFER, binding, this->m_ID, 0, size);
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

    void GLUniformBuffer::SetData(const int offset, const int size, const void* data) const
    {
        this->Bind();
        glNamedBufferSubData(this->m_ID, offset, size, data);
        this->Unbind();
    }   
}

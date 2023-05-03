#include "Buffer.hpp"

#include <glad/glad.h>

namespace Vakol::Model
{
    Buffer::Buffer(const unsigned int type, const int size, const unsigned int binding, const void* data, const unsigned int usage)
    {
        this->GenBuffer(1);

        this->Bind(type);
        this->SetData(size, data, usage);
        this->Unbind(type);

        //this->BindBufferBase(type, binding);
        this->BindBufferRange(type, binding, 0, size);
    }

    Buffer::Buffer(const unsigned int type, const int size, const unsigned int binding, const unsigned int usage)
    {
        this->GenBuffer(1);

        this->Bind(type);
        this->SetData(size, nullptr, usage);
        this->Unbind(type);

        //this->BindBufferBase(type, binding);
        this->BindBufferRange(type, binding, 0, size);
    }

    Buffer::~Buffer()
    {
        glDeleteBuffers(1, &this->ID);
    }

    void Buffer::BindBufferBase(const unsigned int type, const unsigned int binding) const
    {
        glBindBufferBase(static_cast<GLenum>(type), static_cast<GLuint>(binding), this->ID);
    }

    void Buffer::BindBufferRange(const unsigned int type, const unsigned int binding, const int offset, const int size) const
    {
        glBindBufferRange(static_cast<GLenum>(type), static_cast<GLuint>(binding), this->ID, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size));
    }

    void Buffer::GenBuffer(const int n)
    {
        glGenBuffers(static_cast<GLsizei>(n), static_cast<GLuint*>(&this->ID));
    }

    void Buffer::Bind(const unsigned int type) const
    {
        glBindBuffer(static_cast<GLenum>(type), static_cast<GLuint>(this->ID));
    }

    void Buffer::Unbind(const unsigned int type) const
    {
        glBindBuffer(static_cast<GLenum>(type), 0);
    }

    void Buffer::SetData(const int size, const void* data, const unsigned int usage) const
    {
        glNamedBufferData(this->ID, static_cast<GLsizeiptr>(size), data, static_cast<GLenum>(usage));
    }

    void Buffer::SetSubData(const int offset, const int size, const void* data) const
    {
        glNamedBufferSubData(this->ID, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size), data);
    }
}

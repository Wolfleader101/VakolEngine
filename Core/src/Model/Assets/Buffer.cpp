#include "Buffer.hpp"

#include <glad/glad.h>

Buffer::~Buffer()
{
    glDeleteBuffers(1, &this->ID);
}

void Buffer::SetData(const unsigned int type, const int size, const void* data, const unsigned int usage) const
{
    glBufferData(static_cast<GLenum>(type), static_cast<GLsizeiptr>(size), data, static_cast<GLenum>(usage));
}

void Buffer::GenBuffer(const int n)
{
    glGenBuffers(static_cast<GLsizei>(n), static_cast<GLuint*>(&this->ID));
}

void Buffer::BindBuffer(const unsigned int type) const
{
    glBindBuffer(static_cast<GLenum>(type), static_cast<GLuint>(this->ID));
}
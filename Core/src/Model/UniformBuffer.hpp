#pragma once

class UniformBuffer
{
public:
    UniformBuffer(const int size, const int binding) : m_size(size), m_binding(binding) {};
    virtual ~UniformBuffer() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetData(const void* data, const int size, const int offset) = 0;

protected:
    int m_size = 0;
    int m_binding = 0;

    unsigned int m_ID = 0;
};
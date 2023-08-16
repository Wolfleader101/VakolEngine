#pragma once

namespace Vakol::Rendering::OpenGL
{
    void GenBuffers(unsigned int& buffer);

    void BindUniformBuffer(unsigned int buffer);
    void UnbindUniformBuffer();

    void BindUniformBufferBase(unsigned int binding, unsigned int buffer);
    void BindUniformBufferRange(unsigned int binding, unsigned int buffer, int offset, int size);

    void SetUniformBufferData(unsigned int buffer, int size, const void* data);
    void SetUniformBufferSubData(unsigned int buffer, int offset, int size, const void* data);
} // namespace Vakol::Rendering::OpenGL
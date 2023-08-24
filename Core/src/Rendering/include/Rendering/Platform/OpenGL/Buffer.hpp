#pragma once

namespace Vakol::Rendering::OpenGL
{
    /**
     * \brief
     * \param buffer
     */
    void GenBuffers(unsigned int& buffer);
    /**
     * \brief
     * \param buffer
     */
    void BindUniformBuffer(unsigned int buffer);
    /**
     * \brief
     */
    void UnbindUniformBuffer();
    /**
     * \brief
     * \param binding
     * \param buffer
     */
    void BindUniformBufferBase(unsigned int binding, unsigned int buffer);
    /**
     * \brief
     * \param binding
     * \param buffer
     * \param offset
     * \param size
     */
    void BindUniformBufferRange(unsigned int binding, unsigned int buffer, int offset, int size);

    /**
     * \brief
     * \param buffer
     * \param size
     * \param data
     */
    void SetUniformBufferData(unsigned int buffer, int size, const void* data);

    /**
     * \brief
     * \param buffer
     * \param offset
     * \param size
     * \param data
     */
    void SetUniformBufferSubData(unsigned int buffer, int offset, int size, const void* data);
} // namespace Vakol::Rendering::OpenGL
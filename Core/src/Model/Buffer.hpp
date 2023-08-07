#pragma once

#include <glm/vec4.hpp>

namespace Vakol::Model
{
    struct ATTACHMENT
    {
        /**
         * \struct COLOR_ATTACHMENT
         * \brief Represents a color attachment.
         */
        struct COLOR_ATTACHMENT
        {
            int attachment = 0;     /**< The attachment index. */
            int internalformat = 0; /**< The internal format. */
            int format = 0;         /**< The format. */

            int width = 0, height = 0; /**< The width and height of the attachment. */
        };

        /**
         * \struct DEPTH_ATTACHMENT
         * \brief Represents a depth attachment.
         */
        struct DEPTH_ATTACHMENT
        {
            int attachment = 0;     /**< The attachment index. */
            int internalformat = 0; /**< The internal format. */
        };

        COLOR_ATTACHMENT color; /**< The color attachment. */
        DEPTH_ATTACHMENT depth; /**< The depth attachment. */

        /**
         * \brief Constructs an ATTACHMENT object with a color attachment.
         * \param color The color attachment.
         */
        explicit ATTACHMENT(const COLOR_ATTACHMENT& color) : color(color)
        {
        }

        /**
         * \brief Constructs an ATTACHMENT object with a color and depth attachment.
         * \param color The color attachment.
         * \param depth The depth attachment.
         */
        ATTACHMENT(const COLOR_ATTACHMENT& color, const DEPTH_ATTACHMENT& depth) : color(color), depth(depth)
        {
        }
    };

    struct Buffer
    {
        /**
         * \brief Default constructor for Buffer.
         */
        Buffer() = default;

        /**
         * \brief Constructs a Buffer object with specified parameters.
         * \param type The type of the buffer.
         * \param size The size of the buffer.
         * \param binding The binding index of the buffer.
         * \param data Pointer to the data for the buffer.
         * \param usage The usage pattern of the buffer.
         */
        Buffer(unsigned int type, int size, unsigned int binding, const void* data,
               unsigned int usage); // go ahead and set the data

        /**
         * \brief Constructs a Buffer object with specified parameters.
         * \param type The type of the buffer.
         * \param size The size of the buffer.
         * \param binding The binding index of the buffer.
         * \param usage The usage pattern of the buffer.
         */
        Buffer(unsigned int type, int size, unsigned int binding, unsigned int usage); // set data empty for now...

        /**
         * \brief Destructor for Buffer.
         */
        ~Buffer();

        /**
         * \brief Binds the buffer to the specified type and binding index.
         * \param type The type of the buffer.
         * \param binding The binding index of the buffer.
         */
        void BindBufferBase(unsigned int type, unsigned int binding) const;

        /**
         * \brief Binds a range of the buffer to the specified type, binding index, offset, and size.
         * \param type The type of the buffer.
         * \param binding The binding index of the buffer.
         * \param offset The offset within the buffer.
         * \param size The size of the range.
         */
        void BindBufferRange(unsigned int type, unsigned int binding, int offset, int size) const;

        /**
         * \brief Generates a buffer ID based on the specified number of buffers.
         * \param n The number of buffer IDs to generate.
         */
        void GenBuffer(int n); // generate a buffer id based on 'n'- amount of buffers

        /**
         * \brief Binds the buffer to the specified type.
         * \param type The type of the buffer.
         */
        void Bind(unsigned int type) const;

        /**
         * \brief Unbinds the buffer from the specified type.
         * \param type The type of the buffer.
         */
        void Unbind(unsigned int type) const;

        /**
         * \brief Sets the data for the buffer with the specified size, data, and usage.
         * \param size The size of the data.
         * \param data Pointer to the data.
         * \param usage The usage pattern of the buffer.
         */
        void SetData(int size, const void* data, unsigned int usage) const;

        /**
         * \brief Sets a portion of the buffer's data with the specified offset, size, and data.
         * \param offset The offset within the buffer.
         * \param size The size of the data.
         * \param data Pointer to the data.
         */
        void SetSubData(int offset, int size, const void* data) const;

        /**
         * \brief Retrieves the ID of the buffer.
         * \return The buffer ID.
         */
        [[nodiscard]] unsigned int GetID() const
        {
            return ID;
        }

      private:
        unsigned int ID = 0;
    };

    struct FrameBuffer // is-a buffer (impl too different for derived class)
    {
        /**
         * \brief Default constructor for FrameBuffer.
         */
        FrameBuffer();

        /**
         * \brief Constructs a FrameBuffer object with specified parameters.
         * \param attachment The frame buffer attachment.
         * \param depth Flag indicating whether the frame buffer has depth attachment.
         */
        FrameBuffer(const ATTACHMENT& attachment, bool depth);

        /**
         * \brief Destructor for FrameBuffer.
         */
        ~FrameBuffer();

        /**
         * \brief Binds the frame buffer.
         */
        void Bind() const;

        /**
         * \brief Unbinds the frame buffer.
         */
        void Unbind() const;

        /**
         * \brief Clears the specified buffer in the frame buffer.
         * \param buffer The buffer to clear.
         */
        void ClearBuffer(unsigned int buffer) const;

        /**
         * \brief Clears the color buffer with the specified color.
         * \param color The color to clear.
         */
        void ClearColor(const glm::vec4& color) const;

        /**
         * \brief Clears the color buffer with the specified color.
         * \param r Red component of the color.
         * \param g Green component of the color.
         * \param b Blue component of the color.
         * \param a Alpha component of the color.
         */
        void ClearColor(float r, float g, float b, float a) const;

        /**
         * \brief Sets a texture attachment for the frame buffer.
         * \param attachment The type of attachment to set (e.g., GL_COLOR_ATTACHMENT_0).
         * \param target The texture target (e.g., GL_TEXTURE_2D).
         * \param internalformat The internal format of the attachment.
         * \param width The width of the texture.
         * \param height The height of the texture.
         * \param format The format of the pixel data (e.g., GL_RGB, GL_RED, GL_RGBA).
         * \param type The data type of the pixels (keep at GL_UNSIGNED_BYTE).
         * \param data Pointer to the image data in memory.
         */
        void SetTextureAttachment(unsigned int attachment, unsigned int target, int internalformat, int width,
                                  int height, unsigned int format, unsigned int type, const void* data = nullptr);

        /**
         * \brief Sets a depth attachment for the frame buffer.
         * \param attachment The type of attachment to set (e.g., GL_DEPTH_ATTACHMENT).
         * \param internalformat The internal format of the attachment.
         * \param width The width of the texture.
         * \param height The height of the texture.
         */
        void SetDepthAttachment(unsigned int attachment, int internalformat, int width, int height);

        /**
         * \brief Retrieves the ID of the frame buffer.
         * \return The frame buffer ID.
         */
        [[nodiscard]] unsigned int GetID() const
        {
            return ID;
        }

        /**
         * \brief Retrieves the ID of the color attachment.
         * \return The color attachment ID.
         */
        [[nodiscard]] unsigned int GetColorAttachmentID() const
        {
            return m_ColorAttachmentID;
        }

        /**
         * \brief Checks if the frame buffer has a depth attachment.
         * \return True if the frame buffer has a depth attachment, false otherwise.
         */
        [[nodiscard]] bool HasDepth() const
        {
            return hasDepth;
        }

      private:
        void CreateRenderBuffer();
        void DeleteRenderBuffer() const;

        void BindRenderBuffer() const;
        void UnbindRenderBuffer() const;

        void RenderBufferStorage(int internalformat, int width, int height) const;
        void AttachRenderBuffer(unsigned int attachment) const;

        unsigned int ID = 0;
        unsigned int m_RenderBufferID = 0;

        unsigned int m_ColorAttachmentID = 0;

        bool hasDepth = false;
    };
} // namespace Vakol::Model

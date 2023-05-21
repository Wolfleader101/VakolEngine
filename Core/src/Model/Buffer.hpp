#pragma once

#include <glm/vec4.hpp>

namespace Vakol::Model
{
    struct ATTACHMENT
    {
        struct COLOR_ATTACHMENT
        {
            int attachment = 0;
            int internalformat = 0;
            int format = 0;

            int width = 0, height = 0;
        };

        struct DEPTH_ATTACHMENT
        {
            int attachment = 0;
            int internalformat = 0;
        };

        explicit ATTACHMENT(const COLOR_ATTACHMENT& color) : color(color) {}
        ATTACHMENT(const COLOR_ATTACHMENT& color, const DEPTH_ATTACHMENT& depth) : color(color), depth(depth) {}

        COLOR_ATTACHMENT color;
        DEPTH_ATTACHMENT depth;
    };

    struct Buffer
    {
        Buffer() = default;

        Buffer(unsigned int type, int size, unsigned int binding, const void* data, unsigned int usage); // go ahead and set the data
        Buffer(unsigned int type, int size, unsigned int binding, unsigned int usage); // set data empty for now...

        ~Buffer();

        void BindBufferBase(unsigned int type, unsigned int binding) const;
        void BindBufferRange(unsigned int type, unsigned int binding, int offset, int size) const;

        void GenBuffer(int n); // generate a buffer id based on 'n'- amount of buffers
        
        void Bind(unsigned int type) const;
        void Unbind(unsigned int type) const;

        void SetData(int size, const void* data, unsigned int usage) const;
        void SetSubData(int offset, int size, const void* data) const;

        [[nodiscard]] unsigned int GetID() const { return ID; }
    private:
        unsigned int ID = 0;
    };

    struct FrameBuffer // is-a buffer (impl too different for derived class)
    {
        FrameBuffer();
        FrameBuffer(const ATTACHMENT& attachment, bool depth);

        ~FrameBuffer();

        void Bind() const;
        void Unbind() const;

        void ClearBuffer(unsigned int buffer) const;
        void ClearColor(const glm::vec4& color) const;
        void ClearColor(float r, float g, float b, float a) const;

        /**
         * \brief todo
         * \param attachment - the type of attachment you want to set (i.e. GL_COLOR_ATTACHMENT_0)
         * \param target - the texture target : i.e. GL_TEXTURE_2D
         * \param level - level of detail for mip-maps (leave this at 0 if u don't know what ur doing)
         * \param internalformat - internal format
         * \param width - width of tht texture
         * \param height - height of the texture
         * \param format - the formation of pixel data : ie. GL_RGB, GL_RED, GL_RGBA
         * \param type - data type of pixels (KEEP AT GL_UNSIGNED_BYTE)
         * \param data - pointer to the image data in memory
         */
        void SetTextureAttachment(unsigned int attachment, unsigned int target, int level, int internalformat, int width, int height, unsigned int format, unsigned int type, const void* data = nullptr);
        void SetDepthAttachment(unsigned int attachment, int internalformat, int width, int height);

        [[nodiscard]] unsigned int GetID() const { return ID; }
        [[nodiscard]] unsigned int GetColorAttachmentID() const { return m_ColorAttachmentID; }

        [[nodiscard]] bool HasDepth() const { return hasDepth; }
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
}

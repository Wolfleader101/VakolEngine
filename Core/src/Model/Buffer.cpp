#include "Buffer.hpp"

#include <glad/glad.h>

#include <Controller/Logger.hpp>

static int s_hitcount = 0;

namespace Vakol::Model {
    Buffer::Buffer(const unsigned int type, const int size, const unsigned int binding, const void* data,
                   const unsigned int usage) {
        this->GenBuffer(1);

        this->Bind(type);
        this->SetData(size, data, usage);
        this->Unbind(type);

        // this->BindBufferBase(type, binding);
        this->BindBufferRange(type, binding, 0, size);
    }

    Buffer::Buffer(const unsigned int type, const int size, const unsigned int binding, const unsigned int usage) {
        this->GenBuffer(1);

        this->Bind(type);
        this->SetData(size, nullptr, usage);
        this->Unbind(type);

        // this->BindBufferBase(type, binding);
        this->BindBufferRange(type, binding, 0, size);
    }

    Buffer::~Buffer() { glDeleteBuffers(1, &this->ID); }

    void Buffer::BindBufferBase(const unsigned int type, const unsigned int binding) const {
        glBindBufferBase(static_cast<GLenum>(type), static_cast<GLuint>(binding), this->ID);
    }

    void Buffer::BindBufferRange(const unsigned int type, const unsigned int binding, const int offset,
                                 const int size) const {
        glBindBufferRange(static_cast<GLenum>(type), static_cast<GLuint>(binding), this->ID,
                          static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size));
    }

    void Buffer::GenBuffer(const int n) { glGenBuffers(static_cast<GLsizei>(n), static_cast<GLuint*>(&this->ID)); }

    void Buffer::Bind(const unsigned int type) const {
        glBindBuffer(static_cast<GLenum>(type), static_cast<GLuint>(this->ID));
    }

    void Buffer::Unbind(const unsigned int type) const { glBindBuffer(static_cast<GLenum>(type), 0); }

    void Buffer::SetData(const int size, const void* data, const unsigned int usage) const {
        glNamedBufferData(this->ID, static_cast<GLsizeiptr>(size), data, static_cast<GLenum>(usage));
    }

    void Buffer::SetSubData(const int offset, const int size, const void* data) const {
        glNamedBufferSubData(this->ID, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size), data);
    }

    /******************************************************************************************************************/

    FrameBuffer::FrameBuffer() { glGenFramebuffers(1, &this->ID); }
    FrameBuffer::FrameBuffer(const ATTACHMENT& attachment, const bool depth) {
        hasDepth = depth;

        glGenFramebuffers(1, &this->ID);
        Bind();

        SetTextureAttachment(attachment.color.attachment, GL_TEXTURE_2D, attachment.color.internalformat,
                             attachment.color.width, attachment.color.height, attachment.color.format, GL_UNSIGNED_BYTE,
                             nullptr);

        if (depth)
            SetDepthAttachment(attachment.depth.attachment, attachment.depth.internalformat, attachment.color.width,
                               attachment.color.height);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            VK_ERROR("You're doing something wrong");

        Unbind();
    }

    FrameBuffer::~FrameBuffer() {
        glDeleteFramebuffers(1, &this->ID);
        DeleteRenderBuffer();
    }

    void FrameBuffer::ClearBuffer(const unsigned int buffer) const { glClear(buffer); }
    void FrameBuffer::ClearColor(const float r, const float g, const float b, const float a) const {
        glClearColor(r, g, b, a);
    }
    void FrameBuffer::ClearColor(const glm::vec4& color) const { glClearColor(color.r, color.g, color.b, color.a); }

    void FrameBuffer::Bind() const { glBindFramebuffer(GL_FRAMEBUFFER, this->ID); }
    void FrameBuffer::Unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

    void FrameBuffer::CreateRenderBuffer() { glGenRenderbuffers(1, &this->m_RenderBufferID); }
    void FrameBuffer::DeleteRenderBuffer() const { glDeleteRenderbuffers(1, &this->m_RenderBufferID); }

    void FrameBuffer::BindRenderBuffer() const { glBindRenderbuffer(GL_RENDERBUFFER, this->m_RenderBufferID); }
    void FrameBuffer::UnbindRenderBuffer() const { glBindRenderbuffer(GL_RENDERBUFFER, 0); }

    void FrameBuffer::RenderBufferStorage(const int internalformat, const int width, const int height) const {
        glRenderbufferStorage(GL_RENDERBUFFER, internalformat, width, height);
    }
    void FrameBuffer::AttachRenderBuffer(const unsigned int attachment) const {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, this->m_RenderBufferID);
    }

    void FrameBuffer::SetTextureAttachment(const unsigned int attachment, const unsigned int target,
                                           const int internalformat, const int width, const int height,
                                           const unsigned int format, const unsigned int type, const void* data) {
        glCreateTextures(target, 1, &this->m_ColorAttachmentID);
        glBindTexture(target, this->m_ColorAttachmentID);

        glTexImage2D(target, 0, internalformat, width, height, 0, format, type, data);

        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, target, this->m_ColorAttachmentID, 0);
    }

    void FrameBuffer::SetDepthAttachment(const unsigned int attachment, const int internalformat, const int width,
                                         const int height) {
        CreateRenderBuffer();
        BindRenderBuffer();

        RenderBufferStorage(internalformat, width, height);
        AttachRenderBuffer(attachment);
    }
}  // namespace Vakol::Model

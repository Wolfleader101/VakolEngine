#include "GLTexture.hpp"

#include <glad/glad.h>

#include <Controller/Logger.hpp>

namespace Vakol::Model::Assets {
    using Controller::LoadRawTexture;
    using Controller::LoadTexture;

    GLTexture::GLTexture(const int width, const int height, const unsigned int format)
        : Texture(width, height, format) {
        GLenum internal_format = (format == GL_RED) ? GL_R8 : (format == GL_RGB) ? GL_RGB8 : GL_RGBA8;

        glCreateTextures(GL_TEXTURE_2D, 1, &this->m_ID);
        glTextureStorage2D(this->m_ID, 1, internal_format, this->m_width, this->m_height);

        glTextureParameteri(this->m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(this->m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(this->m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(this->m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    void GLTexture::Bind(const unsigned int unit) const { glBindTextureUnit(unit, this->m_ID); }

    void GLTexture::SetData(const void* data) {
        glTextureSubImage2D(this->m_ID, 0, 0, 0, this->m_width, this->m_height, this->m_format, GL_UNSIGNED_BYTE, data);
    }
}  // namespace Vakol::Model::Assets

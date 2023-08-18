#include "Rendering/Platform/OpenGL/Texture.hpp"

#include "Logger/Logger.hpp"

#include <glad/glad.h>

namespace Vakol::Rendering::OpenGL
{
    unsigned int GenerateTexture(const int width, const int height, const int channels, const unsigned char* pixels)
    {
        unsigned int texture;

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        const GLenum internal_format = channels == 1 ? GL_R8 : channels > 3 ? GL_RGBA8 : GL_RGB8;
        const GLenum format = channels == 1 ? GL_RED : channels > 3 ? GL_RGBA : GL_RGB;

        glTexStorage2D(GL_TEXTURE_2D, 1, internal_format, width, height);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, pixels);

        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        delete[] pixels;
        pixels = nullptr;

        glBindTexture(GL_TEXTURE_2D, 0);

        return texture;
    }

    void SetActiveTexture(const int slot)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
    }

    void BindTexture(const unsigned int texture)
    {
        glBindTexture(GL_TEXTURE_2D, texture);
    }

} // namespace Vakol::Rendering::OpenGL
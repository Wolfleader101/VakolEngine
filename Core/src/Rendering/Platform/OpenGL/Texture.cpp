#include "Texture.hpp"

#include "Controller/Logger.hpp"

#include <glad/glad.h>

namespace Vakol::Rendering::OpenGL
{
    unsigned int GenerateTexture(const int width, const int height, const unsigned char* pixels)
    {
        unsigned int texture;

        if (width <= 0 || height <= 0)
            return 0u;

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

        delete[] pixels;
        pixels = nullptr;

        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

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

}
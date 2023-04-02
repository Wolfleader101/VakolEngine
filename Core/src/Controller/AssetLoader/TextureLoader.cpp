#include "TextureLoader.hpp"

#include <IO/FileLoader.hpp>

#include <glad/glad.h>

const unsigned int LoadGLTexture(const std::string&);

namespace Vakol::Controller::AssetLoader {
    const unsigned int LoadTexture(const std::string& path) { return ::LoadGLTexture(path); }
}  // namespace Vakol::Controller::AssetLoader

const unsigned int LoadGLTexture(const std::string& path)
{
    unsigned int id;

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &id);

    int width, height, channels;
    const auto data = LoadImage(path, width, height, channels, true);

    GLenum format = (channels == 1) ? GL_RED : (channels > 3) ? GL_RGBA : GL_RGB;

    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_TEXTURE_2D);

    return id;
}
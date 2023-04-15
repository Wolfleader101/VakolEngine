#include "TextureLoader.hpp"

#include <Controller/AssetLoader/FileLoader.hpp>

#include <glad/glad.h>

const unsigned int LoadGLTexture(const std::string&, const bool);

namespace Vakol::Controller
{
    const unsigned int LoadTexture(const std::string& path) { return ::LoadGLTexture(path, false); }

    const unsigned int LoadRawTexture(const std::string& path) { return ::LoadGLTexture(path, true); }
}

const unsigned int LoadGLTexture(const std::string& path, const bool raw)
{
    unsigned int ID = 0;

    glGenTextures(1, &ID);

    int width = 0, height = 0, channels = 1;
    auto data = (raw) ? LoadImage(path, width, height) : LoadImage(path, width, height, channels, true);

    GLenum internal_format = (channels == 1) ? GL_R8 : (channels > 3) ? GL_RGBA8 : GL_RGB8;
    GLenum data_format = (channels == 1) ? GL_RED : (channels > 3) ? GL_RGBA : GL_RGB;

    glCreateTextures(GL_TEXTURE_2D, 1, &ID);
    glTextureStorage2D(ID, 1, internal_format, width, height);

    glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureParameteri(ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureSubImage2D(ID, 0, 0, 0, width, height, data_format, GL_UNSIGNED_BYTE, data);

    delete[] data;
    data = nullptr;

    return ID;
}
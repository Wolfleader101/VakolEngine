#include "TextureLoader.hpp"

#include <Controller/AssetLoader/FileLoader.hpp>
#include <Controller/Logger.hpp>

#include <glad/glad.h>

unsigned int LoadGLTexture(std::string&, const bool, const bool, const bool);
unsigned int LoadGLTexture(const int, float, const int, const float, const float);

namespace Vakol::Controller
{
	unsigned int LoadTexture(std::string& path, const bool gamma, const bool flip) { return ::LoadGLTexture(path, false, gamma, flip); }

	unsigned int LoadNoiseTexture(const int size, const float scale, const int octaves, const float persistence, const float lacunarity) { return ::LoadGLTexture(size, scale, octaves, persistence, lacunarity); }
    // gamma correction only applies for RGB/RGBA channels
    unsigned int LoadRawTexture(std::string& path) { return ::LoadGLTexture(path, true, false, false); }
}


unsigned int LoadGLTexture(std::string& path, const bool raw, const bool gamma, const bool flip)
{
    unsigned int ID = 0;

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    int width = 0, height = 0, channels = 1;
    auto data = (raw) ? LoadImage(std::move(path), width, height) : LoadImage(std::move(path), width, height, channels, flip);

    if (!data) data = LoadImage("coreAssets/textures/error.png", width, height, channels, flip);

    VK_ASSERT(data, "\n\ndata was nullptr");

    const GLint internal_format = (channels == 1) ? GL_R8 : (channels > 3) ? (gamma) ? GL_SRGB8_ALPHA8 : GL_RGBA8 : (gamma) ? GL_SRGB8 : GL_RGB8;
    const GLenum data_format = (channels == 1) ? GL_RED : (channels > 3) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, data_format, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    delete[] data;
    data = nullptr;

    glBindTexture(GL_TEXTURE_2D, 0); // unbind

    return ID;
}

unsigned int LoadGLTexture(const int size, const float scale, const int octaves, const float persistence, const float lacunarity)
{
    unsigned int ID = 0;

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    auto data = LoadNoiseImage(size, scale, octaves, persistence, lacunarity);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, size, size, 0, GL_RED, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    delete[] data;
    data = nullptr;

    glBindTexture(GL_TEXTURE_2D, 0); // unbind

    return ID;
}
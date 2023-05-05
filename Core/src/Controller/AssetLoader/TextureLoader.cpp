#include "TextureLoader.hpp"

#include <Controller/AssetLoader/FileLoader.hpp>

#include <glad/glad.h>

unsigned int LoadGLTexture(const std::string&, const bool, const bool, const bool);
unsigned int LoadGLTexture(const int, float, const int, const float, const float);

namespace Vakol::Controller
{
	unsigned int LoadTexture(const std::string& path, const bool gamma, const bool flip) { return ::LoadGLTexture(path, false, gamma, flip); }

	unsigned int LoadNoiseTexture(const int size, float scale, const int octaves, const float persistence, const float lacunarity) { return ::LoadGLTexture(size, scale, octaves, persistence, lacunarity); }

    // gamma correction only applies for RGB/RGBA channels
    unsigned int LoadRawTexture(const std::string& path) { return ::LoadGLTexture(path, true, false, false); }
}


unsigned int LoadGLTexture(const std::string& path, const bool raw, const bool gamma, const bool flip)
{
    unsigned int ID = 0;

    glGenTextures(1, &ID);

    int width = 0, height = 0, channels = 1;
    auto data = (raw) ? LoadImage(path, width, height) : LoadImage(path, width, height, channels, flip);

    const GLenum internal_format = (channels == 1) ? GL_R8 : (channels > 3) ? (gamma) ? GL_SRGB8_ALPHA8 : GL_RGBA8 : (gamma) ? GL_SRGB8 : GL_RGB8;
    const GLenum data_format = (channels == 1) ? GL_RED : (channels > 3) ? GL_RGBA : GL_RGB;

    glCreateTextures(GL_TEXTURE_2D, 1, &ID);
    glTextureStorage2D(ID, 1, internal_format, width, height);

    glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureParameteri(ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenerateMipmap(ID);

    glTextureSubImage2D(ID, 0, 0, 0, width, height, data_format, GL_UNSIGNED_BYTE, data);

    delete[] data;
    data = nullptr;

    return ID;
}

unsigned int LoadGLTexture(const int size, float scale, const int octaves, const float persistence, const float lacunarity)
{
    unsigned int ID = 0;

    glGenTextures(1, &ID);
    
    auto data = LoadNoiseImage(size, scale, octaves, persistence, lacunarity);

    glCreateTextures(GL_TEXTURE_2D, 1, &ID);
    glTextureStorage2D(ID, 1, GL_R8, size, size);

    glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureParameteri(ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenerateMipmap(ID);

    glTextureSubImage2D(ID, 0, 0, 0, size, size, GL_RED, GL_UNSIGNED_BYTE, data);

    delete[] data;
    data = nullptr;

    return ID;
}
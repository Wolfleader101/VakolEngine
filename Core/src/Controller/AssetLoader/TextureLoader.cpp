#include "TextureLoader.hpp"

#include <Controller/AssetLoader/FileLoader.hpp>
#include <Controller/Logger.hpp>

#include <glad/glad.h>

unsigned int LoadGLTexture(std::vector<const char*>&&, bool, bool);
unsigned int LoadGLTexture(std::string&, bool, bool, bool);
unsigned int LoadGLTexture(int, bool, bool, const void*);

namespace Vakol::Controller
{
    unsigned int LoadTexture(std::vector<const char*>&& faces, const bool gamma, const bool flip) { return LoadGLTexture(std::move(faces), gamma, flip); }
    unsigned int LoadTexture(const int size, const bool gamma, const bool flip, const void* data) { return LoadGLTexture(size, gamma, flip, data); }
	unsigned int LoadTexture(std::string& path, const bool gamma, const bool flip) { return ::LoadGLTexture(path, false, gamma, flip); }
    // gamma correction only applies for RGB/RGBA channels
    unsigned int LoadRawTexture(std::string& path) { return ::LoadGLTexture(path, true, false, false); }
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int LoadGLTexture(std::vector<const char*>&& faces, const bool gamma, const bool flip)
{
	const auto size = static_cast<int>(faces.size());

    unsigned int ID = 0;

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

    int width = 0, height = 0, channels = 1;

    for (int i = 0; i < size; ++i)
    {
    	auto data = LoadImage(faces.at(i), width, height, channels, flip);

    	//if (!data) data = LoadImage("coreAssets/textures/error.png", width, height, channels, flip);
        VK_ASSERT(data, "\n\ndata was nullptr");

        const GLint internal_format = (channels == 1) ? GL_R8 : (channels > 3) ? (gamma) ? GL_SRGB8_ALPHA8 : GL_RGBA8 : (gamma) ? GL_SRGB8 : GL_RGB8;
        const GLenum data_format = (channels == 1) ? GL_RED : (channels > 3) ? GL_RGBA : GL_RGB;

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internal_format, width, height, 0, data_format, GL_UNSIGNED_BYTE, data);

        delete[] data;
        data = nullptr;

        glBindTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0); // unbind individual faces
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0); // unbind cube map

    return ID;
}

unsigned int LoadGLTexture(const int size, const bool gamma, const bool flip, const void* data)
{
    unsigned int ID = 0;

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    int width = 0, height = 0, channels = 1;
    auto img_data = LoadImage(size, width, height, channels, flip, data);

    if (!img_data) img_data = LoadImage("coreAssets/textures/error.png", width, height, channels, flip);
    VK_ASSERT(img_data, "\n\ndata was nullptr");

    const GLint internal_format = (channels == 1) ? GL_R8 : (channels > 3) ? (gamma) ? GL_SRGB8_ALPHA8 : GL_RGBA8 : (gamma) ? GL_SRGB8 : GL_RGB8;
    const GLenum data_format = (channels == 1) ? GL_RED : (channels > 3) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, data_format, GL_UNSIGNED_BYTE, img_data);

    glGenerateMipmap(GL_TEXTURE_2D);

    delete[] img_data;
	img_data = nullptr;

    glBindTexture(GL_TEXTURE_2D, 0); // unbind

    return ID;
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
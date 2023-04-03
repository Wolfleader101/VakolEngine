#include "TextureLoader.hpp"

#include <Controller/IO/FileLoader.hpp>
#include <Model/Assets/Texture.hpp>

#include <glad/glad.h>

using namespace Vakol::Controller::IO;
using Texture = Vakol::Model::Assets::Texture;

Texture LoadGLTexture(const std::string&);

namespace Vakol::Controller {
    Texture LoadTexture(const std::string& path) { return ::LoadGLTexture(path); }
}  // namespace Vakol::Controller::AssetLoader

Texture LoadGLTexture(const std::string& path)
{
    unsigned int id;

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &id);

    int width, height, channels;
    const unsigned char* data = Vakol::Controller::IO::LoadImage(path, true, width, height, channels);

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

    return Texture{path, "", id, width, height, channels};
}
#pragma once

#include <string>
#include <vector>

namespace Vakol::Rendering::Assets
{
    struct Texture;
}

namespace Vakol::Rendering::OpenGL
{
    unsigned int GenerateTexture(int levels, int width, int height, int channels, const unsigned char* pixels);
    unsigned int GenerateTexture(std::vector<Assets::Texture>&& textures);

    void SetActiveTexture(int slot);

    void BindTexture(unsigned int texture);
    void BindCubemapTexture(unsigned int texture);
} // namespace Vakol::Rendering::OpenGL
#pragma once

#include "Rendering/Assets/Texture.hpp"

#include <string>

namespace Vakol::Rendering::Assets
{
    void ImportTexture(const std::string& path, int& width, int& height, int& channels, unsigned char*& pixels);
    void ImportTexture(const void* data, int length, int& width, int& height, int& channels,
                       unsigned char*& pixels); // memory location of embedded data
} // namespace Vakol::Rendering::Assets
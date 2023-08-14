#pragma once

#include "Rendering/Assets/Texture.hpp"

#include <string>

namespace Vakol::Rendering::Assets::Importer
{
    void ImportTexture(const std::string& path, int& width, int& height, int& channels, unsigned char*& pixels);
    void ImportTexture(const unsigned char*& buffer, int length, int& width, int& height, int& channels, unsigned char*& pixels); // memory location of embedded data
}
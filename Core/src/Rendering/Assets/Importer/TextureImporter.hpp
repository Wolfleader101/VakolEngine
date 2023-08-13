#pragma once

#include "Rendering/Assets/Texture.hpp"

#include <string>

namespace Vakol::Rendering::Assets::Importer
{
    Texture ImportTexture(const std::string& path, unsigned char*& pixels);
    Texture ImportTexture(int size, const void* location); // memory location of embedded data
}
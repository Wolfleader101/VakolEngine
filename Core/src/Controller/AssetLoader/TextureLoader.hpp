#pragma once

#include <string>
#include <vector>

namespace Vakol::Controller
{
    const unsigned int LoadTexture(const std::string& path, const bool gamma, const bool flip);
    const unsigned int LoadRawTexture(const std::string& path);
}
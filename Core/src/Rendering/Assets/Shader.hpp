#pragma once

#include <string>
#include <vector>

namespace Vakol::Rendering::Assets
{
    struct Shader
    {
        std::string path;
        std::vector<std::string> sources;
    };
}
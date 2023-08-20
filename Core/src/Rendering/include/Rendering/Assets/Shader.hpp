#pragma once

#include <string>
#include <vector>

namespace Vakol::Rendering::Assets
{
    struct Shader
    {
        std::string path;

        std::string vertSrc; // vertex
        std::string geomSrc; // geometry
        std::string tscSrc;  // tessellation control
        std::string tseSrc;  // tessellation evaluation
        std::string fragSrc; // fragment
    };
} // namespace Vakol::Rendering::Assets
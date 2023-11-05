#pragma once

#include <string>
#include <vector>

#include <glad/glad.h>

#include "AssetLoader/AssetManager.hpp"
#include "Logger/Logger.hpp"
#include "Rendering/RenderData.hpp"

namespace Vakol::Rendering::Assets
{
    struct Shader
    {
        std::string path; /// The path of the shader

        std::string vertSrc; /// vertex
        std::string geomSrc; /// geometry
        std::string tscSrc;  /// tessellation control
        std::string tseSrc;  /// tessellation evaluation
        std::string fragSrc; /// fragment
    };
} // namespace Vakol::Rendering::Assets
#pragma once

#include "Rendering/Assets/Shader.hpp"

#include <string>

namespace Vakol
{
    Rendering::Assets::Shader ImportShader(const std::string& path, bool& success);
}

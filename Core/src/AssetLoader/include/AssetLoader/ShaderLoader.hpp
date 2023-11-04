#pragma once

#include <string>
#include <vector>

#include "Rendering/Assets/Shader.hpp"

#include "AssetLoader/FileLoader.hpp"

#include "Logger/Logger.hpp"

namespace Vakol
{
    /**
     * \brief
     * \param path the path of the shader program
     * \param success was the shader successfully imported?
     * \return
     */
    Rendering::Assets::Shader ImportShader(const std::string& path, bool& success);
} // namespace Vakol
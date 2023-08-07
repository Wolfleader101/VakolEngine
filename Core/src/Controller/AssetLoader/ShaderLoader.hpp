#pragma once

#include <string>

namespace Vakol::Controller
{
    /**
     * @brief Load a shader from a file.
     *
     * @param path The file path of the shader.
     * @return unsigned int The ID of the loaded shader.
     */
    unsigned int LoadShader(const std::string &path);
} // namespace Vakol::Controller

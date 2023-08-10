#pragma once

#include <string>
#include <vector>

namespace Vakol::Rendering::OpenGL
{
    unsigned int GenerateShaderProgram(std::vector<std::string>&& sources);

    void BindShaderProgram(unsigned int program);
    void UnbindShaderProgram();
}
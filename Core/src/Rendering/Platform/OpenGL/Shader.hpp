#pragma once

#include <string>
#include <vector>
#include <unordered_map>

struct Uniform;

namespace Vakol::Rendering::OpenGL
{
    unsigned int GenerateShaderProgram(std::vector<std::string>&& sources);

    void BindShaderProgram(unsigned int program);
    void UnbindShaderProgram();

    void GetUniforms(unsigned int shader, std::unordered_map<std::string, Uniform>& uniforms);

    void SetMat4(int location, int count, const char* name, bool transpose, const float* value);
}
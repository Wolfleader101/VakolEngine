#pragma once

#include <string>
#include <unordered_map>
#include <vector>

struct Uniform;

namespace Vakol::Rendering::OpenGL
{
    unsigned int GenerateShaderProgram(const std::vector<std::string>& sources);

    void BindShaderProgram(unsigned int program);
    void UnbindShaderProgram();

    void GetUniforms(unsigned int shader, std::unordered_map<std::string, Uniform>& uniforms);

    void SetMat4(int location, int count, const char* name, bool transpose, const float* value);
    void SetMat3(int location, int count, const char* name, bool transpose, const float* value);

    void SetFloat(int location, float value);
    void SetInt(int location, int value);

    void SetVec3(int location, int count, const float* value);
    void SetVec4(int location, int count, const float* value);
} // namespace Vakol::Rendering::OpenGL
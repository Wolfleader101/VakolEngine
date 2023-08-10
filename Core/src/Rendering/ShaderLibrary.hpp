#pragma once

#include <map>
#include <unordered_map>
#include <string>

#include "Math/Math.hpp"

namespace Vakol::Rendering
{
    struct Uniform;

    class ShaderLibrary
    {
    public:
        static void GetShaderUniforms(unsigned int shader);

        static void SetMat4(unsigned int shader, const char* name, bool transpose, const Math::Mat4& value);
    private:
        static std::map<unsigned int, std::unordered_map<std::string, Uniform>> m_uniforms;
    };
}
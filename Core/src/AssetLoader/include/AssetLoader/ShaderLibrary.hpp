#pragma once

#include <map>
#include <string>
#include <unordered_map>

#include "Math/Math.hpp"

namespace Vakol::Rendering
{
    struct Uniform;
} // namespace Vakol::Rendering

namespace Vakol
{
    class ShaderLibrary
    {
      public:
        void AddShader(const std::string& shaderID, unsigned int shader);

        unsigned int GetShader(const std::string& shaderID);

        void SetInt(unsigned int shader, const char* name, int value);
        void SetFloat(unsigned int shader, const char* name, float value);

        void SetVec3(unsigned int shader, const char* name, const Math::Vec3& value);
        void SetVec4(unsigned int shader, const char* name, const Math::Vec4& value);

        void SetMat4(unsigned int shader, const char* name, bool transpose, const Math::Mat4& value);

      private:
        std::unordered_map<std::string, unsigned int> m_shaders;
        std::map<unsigned int, std::unordered_map<std::string, Rendering::Uniform>> m_uniforms;

        void GetShaderUniforms(unsigned int shader);
        Rendering::Uniform& GetUniform(unsigned int shader, const char* name);

        bool UniformExists(unsigned int shader, const char* name);
    };
} // namespace Vakol
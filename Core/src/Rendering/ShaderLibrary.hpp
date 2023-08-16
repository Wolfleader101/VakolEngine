#pragma once

#include <map>
#include <unordered_map>
#include <string>

#include "Math/Math.hpp"

namespace Vakol::Rendering
{
    struct Uniform;
    struct UniformBuffer;

    class ShaderLibrary
    {
    public:
        static void AddShader(const std::string& ID, unsigned int shader);
        static void CreateUniformBuffer(const char* uBufferName, int size, unsigned int binding);

        static unsigned int GetShader(const std::string& ID);

        static void GetShaderUniforms(unsigned int shader);

        static void SetInt(unsigned int shader, const char* name, int value);
        static void SetFloat(unsigned int shader, const char* name, float value);

        static void SetVec3(unsigned int shader, const char* name, const Math::Vec3& value);
        static void SetVec4(unsigned int shader, const char* name, const Math::Vec4& value);

        static void SetMat4(unsigned int shader, const char* name, bool transpose, const Math::Mat4& value);

        static void SetUniformBufferData(const char* name, int size, const void* data);
        static void SetUniformBufferSubData(const char* name, int offset, int size, const void* data);
    private:
        static std::unordered_map<std::string, unsigned int> m_shaders;
        static std::map<unsigned int, std::unordered_map<std::string, Uniform>> m_uniforms;
        static std::unordered_map<std::string, UniformBuffer> m_uniformBuffers;

        static void AddUniformBuffer(const std::string& uBufferName, const UniformBuffer& uBuffer);

        static Uniform GetUniform(unsigned int shader, const char* name);
        static UniformBuffer GetUniformBuffer(const char* name);

        static bool UniformExists(unsigned int shader, const char* name);
        static bool UniformBufferExists(const char* name);
    };
}
#include "ShaderLibrary.hpp"

#include "Platform/OpenGL/Shader.hpp"
#include "RenderData.hpp"
#include "Controller/Logger.hpp"

namespace Vakol::Rendering
{
    std::unordered_map<std::string, unsigned int> ShaderLibrary::m_shaders;
    std::map<unsigned int, std::unordered_map<std::string, Uniform>> ShaderLibrary::m_uniforms;

    void ShaderLibrary::AddShader(const std::string& ID, const unsigned int shader)
    {
        if (m_shaders.find(ID) == m_shaders.end())
            m_shaders[ID] = shader;
    }

    unsigned int ShaderLibrary::GetShader(const std::string& ID)
    {
        if (m_shaders.find(ID) != m_shaders.end())
            return m_shaders.at(ID);

        VK_ERROR("Unable to find shader for ID = {0}", ID);

        return 0u;
    }

    void ShaderLibrary::GetShaderUniforms(const unsigned int shader)
    {
        if (m_uniforms.find(shader) == m_uniforms.end())
            m_uniforms[shader].emplace();

        OpenGL::GetUniforms(shader, m_uniforms.at(shader));
    }

    void ShaderLibrary::SetMat4(const unsigned int shader, const char* name, const bool transpose, const Math::Mat4& value)
    {
        if (UniformExists(shader, name))
        {
            const auto& [location, count] = GetUniform(shader, name);

            OpenGL::SetMat4(location, count, name, transpose, Math::ToArray(value));
        }
    }

    void ShaderLibrary::SetFloat(const unsigned int shader, const char* name, const float value)
    {
        if (UniformExists(shader, name))
        {
            const auto& [location, count] = GetUniform(shader, name);

            OpenGL::SetFloat(location, value);
        }
    }

    void ShaderLibrary::SetVec3(const unsigned int shader, const char* name, const Math::Vec3& value)
    {
        if (UniformExists(shader, name))
        {
            const auto& [location, count] = GetUniform(shader, name);

            OpenGL::SetVec3(location, count, Math::ToArray(value));
        }
    }

    void ShaderLibrary::SetVec4(const unsigned int shader, const char* name, const Math::Vec4& value)
    {
        if (UniformExists(shader, name))
        {
            const auto& [location, count] = GetUniform(shader, name);

            OpenGL::SetVec4(location, count, Math::ToArray(value));
        }
    }

    Uniform ShaderLibrary::GetUniform(const unsigned int shader, const char* name)
    {
        return m_uniforms.at(shader).at(name);
    }

    bool ShaderLibrary::UniformExists(const unsigned int shader, const char* name)
    {
        return m_uniforms.at(shader).find(name) != m_uniforms.at(shader).end();
    }
}

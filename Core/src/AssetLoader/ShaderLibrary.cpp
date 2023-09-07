#include "AssetLoader/ShaderLibrary.hpp"

#include "Rendering/Platform/OpenGL/Shader.hpp"

#include "Logger/Logger.hpp"
#include "Rendering/RenderData.hpp"

namespace Vakol
{
    void ShaderLibrary::AddShader(const std::string& shaderID, const unsigned int shader)
    {
        if (m_shaders.find(shaderID) == m_shaders.end())
        {
            m_shaders[shaderID] = shader;
            GetShaderUniforms(shader);
        }
    }

    unsigned int ShaderLibrary::GetShader(const std::string& shaderID)
    {
        if (m_shaders.find(shaderID) != m_shaders.end())
            return m_shaders.at(shaderID);

        VK_ERROR("Unable to find shader for ID = {0}", shaderID);

        return 0u;
    }

    void ShaderLibrary::GetShaderUniforms(const unsigned int shader)
    {
        if (m_uniforms.find(shader) == m_uniforms.end())
        {
            m_uniforms[shader].emplace();
        }

        Rendering::OpenGL::GetUniforms(shader, m_uniforms.at(shader));

        SetInt(shader, "material.diffuse_map", 1);
        SetInt(shader, "material.specular_map", 2);
        SetInt(shader, "material.ambient_map", 3);
        SetInt(shader, "material.emissive_map", 4);
        SetInt(shader, "material.height_map", 5);
        SetInt(shader, "material.normal_map", 6);
    }

    void ShaderLibrary::SetBool(const unsigned int shader, const char* name, const bool value, const bool binding)
    {
        if (UniformExists(shader, name))
        {
            const auto& [location, count] = GetUniform(shader, name);

            if (binding)
            {
                Rendering::OpenGL::BindShaderProgram(shader);
                Rendering::OpenGL::SetBool(location, value);
                Rendering::OpenGL::UnbindShaderProgram();
            }
            else
                Rendering::OpenGL::SetBool(location, value);
        }
    }

    void ShaderLibrary::SetInt(const unsigned int shader, const char* name, const int value, const bool binding)
    {
        if (UniformExists(shader, name))
        {
            const auto& [location, count] = GetUniform(shader, name);

            if (binding)
            {
                Rendering::OpenGL::BindShaderProgram(shader);
                Rendering::OpenGL::SetInt(location, value);
                Rendering::OpenGL::UnbindShaderProgram();
            }
            else
                Rendering::OpenGL::SetInt(location, value);
        }
    }

    void ShaderLibrary::SetFloat(const unsigned int shader, const char* name, const float value, const bool binding)
    {
        if (UniformExists(shader, name))
        {
            const auto& [location, count] = GetUniform(shader, name);

            if (binding)
            {
                Rendering::OpenGL::BindShaderProgram(shader);
                Rendering::OpenGL::SetFloat(location, value);
                Rendering::OpenGL::UnbindShaderProgram();
            }
            else
                Rendering::OpenGL::SetFloat(location, value);
        }
    }

    void ShaderLibrary::SetVec2(const unsigned int shader, const char* name, const Math::Vec2& value,
                                const bool binding)
    {
        if (UniformExists(shader, name))
        {
            const auto& [location, count] = GetUniform(shader, name);

            if (binding)
            {
                Rendering::OpenGL::BindShaderProgram(shader);
                Rendering::OpenGL::SetVec2(location, count, Math::AsArray(value));
                Rendering::OpenGL::UnbindShaderProgram();
            }
            else
                Rendering::OpenGL::SetVec2(location, count, Math::AsArray(value));
        }
    }

    void ShaderLibrary::SetVec3(const unsigned int shader, const char* name, const Math::Vec3& value,
                                const bool binding)
    {
        if (UniformExists(shader, name))
        {
            const auto& [location, count] = GetUniform(shader, name);

            if (binding)
            {
                Rendering::OpenGL::BindShaderProgram(shader);
                Rendering::OpenGL::SetVec3(location, count, Math::AsArray(value));
                Rendering::OpenGL::UnbindShaderProgram();
            }
            else
                Rendering::OpenGL::SetVec3(location, count, Math::AsArray(value));
        }
    }

    void ShaderLibrary::SetVec4(const unsigned int shader, const char* name, const Math::Vec4& value,
                                const bool binding)
    {
        if (UniformExists(shader, name))
        {
            const auto& [location, count] = GetUniform(shader, name);

            if (binding)
            {
                Rendering::OpenGL::BindShaderProgram(shader);
                Rendering::OpenGL::SetVec4(location, count, Math::AsArray(value));
                Rendering::OpenGL::UnbindShaderProgram();
            }
            else
                Rendering::OpenGL::SetVec4(location, count, Math::AsArray(value));
        }
    }

    void ShaderLibrary::SetMat3(const unsigned int shader, const char* name, const bool transpose,
                                const Math::Mat3& value, const bool binding)
    {
        if (UniformExists(shader, name))
        {
            const auto& [location, count] = GetUniform(shader, name);

            if (binding)
            {
                Rendering::OpenGL::BindShaderProgram(shader);
                Rendering::OpenGL::SetMat3(location, count, name, transpose, Math::AsArray(value));
                Rendering::OpenGL::UnbindShaderProgram();
            }
            else
                Rendering::OpenGL::SetMat3(location, count, name, transpose, Math::AsArray(value));
        }
    }

    void ShaderLibrary::SetMat4(const unsigned int shader, const char* name, const bool transpose,
                                const Math::Mat4& value, const bool binding)
    {
        if (UniformExists(shader, name))
        {
            const auto& [location, count] = GetUniform(shader, name);

            if (binding)
            {
                Rendering::OpenGL::BindShaderProgram(shader);
                Rendering::OpenGL::SetMat4(location, count, name, transpose, Math::AsArray(value));
                Rendering::OpenGL::UnbindShaderProgram();
            }
            else
                Rendering::OpenGL::SetMat4(location, count, name, transpose, Math::AsArray(value));
        }
    }

    Rendering::Uniform& ShaderLibrary::GetUniform(const unsigned int shader, const char* name)
    {
        return m_uniforms.at(shader).at(name);
    }

    bool ShaderLibrary::UniformExists(const unsigned int shader, const char* name)
    {
        return m_uniforms.at(shader).find(name) != m_uniforms.at(shader).end();
    }
} // namespace Vakol
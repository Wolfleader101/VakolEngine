#include "AssetLoader/ShaderProcessing.hpp"

namespace Vakol
{
    void ShaderProcessing::AddShader(const std::string& shaderID, const unsigned int shader)
    {
        if (m_shaders.find(shaderID) == m_shaders.end())
        {
            m_shaders[shaderID] = shader;
            GetShaderUniforms(shader);
        }
    }

    unsigned int ShaderProcessing::GetShader(const std::string& shaderID)
    {
        if (m_shaders.find(shaderID) != m_shaders.end())
            return m_shaders.at(shaderID);

        VK_ERROR("Unable to find shader for ID = {0}", shaderID);

        return 0u;
    }

    void ShaderProcessing::GetShaderUniforms(const unsigned int shader)
    {
        Rendering::OpenGL::GetUniforms(shader, m_uniforms[shader]);

        SetInt(shader, "material.diffuse_map", 1);
        SetInt(shader, "material.specular_map", 2);
        SetInt(shader, "material.ambient_map", 3);
        SetInt(shader, "material.emissive_map", 4);
        SetInt(shader, "material.height_map", 5);
        SetInt(shader, "material.normal_map", 6);
    }

    void ShaderProcessing::SetBool(const unsigned int shader, const char* name, const bool value, const bool binding)
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

    void ShaderProcessing::SetInt(const unsigned int shader, const char* name, const int value, const bool binding)
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

    void ShaderProcessing::SetUInt(const unsigned int shader, const char* name, const unsigned int value,
                                   const bool binding)
    {
        if (UniformExists(shader, name))
        {
            const auto& [location, count] = GetUniform(shader, name);

            if (binding)
            {
                Rendering::OpenGL::BindShaderProgram(shader);
                Rendering::OpenGL::SetUInt(location, value);
                Rendering::OpenGL::UnbindShaderProgram();
            }
            else
                Rendering::OpenGL::SetUInt(location, value);
        }
    }

    void ShaderProcessing::SetFloat(const unsigned int shader, const char* name, const float value, const bool binding)
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

    void ShaderProcessing::SetVec2(const unsigned int shader, const char* name, const Math::Vec2& value,
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

    void ShaderProcessing::SetVec3(const unsigned int shader, const char* name, const Math::Vec3& value,
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

    void ShaderProcessing::SetVec4(const unsigned int shader, const char* name, const Math::Vec4& value,
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

    void ShaderProcessing::SetMat3(const unsigned int shader, const char* name, const bool transpose,
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

    void ShaderProcessing::SetMat4(const unsigned int shader, const char* name, const bool transpose,
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

    Rendering::Uniform& ShaderProcessing::GetUniform(const unsigned int shader, const char* name)
    {
        Rendering::Uniform defaultUniform{}; // Empty Uniform to return if uniform is not found

        std::vector<UniformIdentifier>& uniformIdentifiers = m_uniforms.at(shader);

        for (UniformIdentifier& uniformIdentifier : uniformIdentifiers)
        {
            if (uniformIdentifier.uniformName == name)
            {
                return uniformIdentifier.uniformVariable;
            }
        }

        VK_ERROR("Uniform '" + std::string(name) + "' not found! Returning empty uniform...");

        return defaultUniform;
    }

    bool ShaderProcessing::UniformExists(const unsigned int shader, const char* name)
    {
        const std::vector<UniformIdentifier>& uniformIdentifiers = m_uniforms.at(shader);

        for (const UniformIdentifier& uniformIdentifier : uniformIdentifiers)
        {
            if (uniformIdentifier.uniformName == name)
            {
                VK_INFO("Uniform '" + std::string(name) + "' exists!");

                return true;
            }
        }

        VK_ERROR("Uniform '" + std::string(name) + "' does not exist!");

        return false;
    }

    bool ShaderProcessing::IsEmpty() const
    {
        return m_shaders.empty();
    }

    Rendering::Assets::Shader ShaderProcessing::ImportShader(const std::string& path, bool& success)
    {
        Rendering::Assets::Shader shader;

        std::vector<std::string> paths;

        const auto directory = path.substr(0, path.find_last_of('.'));

        const char* extensions[] = {".vert", ".geom", ".tesc", ".tese", ".frag"};

        for (const auto& extension : extensions)
        {
            if (const auto shaderPath = directory + extension; FileExists(shaderPath))
                paths.emplace_back(shaderPath);
            else
                paths.emplace_back("");
        }

        shader.path = path;
        success = GetShaderSources(std::move(paths), shader);

        return shader;
    }

    bool ShaderProcessing::GetShaderSources(std::vector<std::string>&& paths, Rendering::Assets::Shader& shader)
    {
        std::vector<std::string> sources;

        if (paths[0].empty() || paths[4].empty())
        {
            VK_ERROR("VERTEX AND/OR FRAGMENT SHADER WAS NOT FOUND!");

            return false;
        }

        sources.emplace_back(LoadFile(paths[0]));

        if (paths[1].empty())
        {
            sources.emplace_back("");
        }
        else
            sources.emplace_back(LoadFile(paths[1]));

        if (paths[2].empty())
        {
            sources.emplace_back("");
        }
        else
            sources.emplace_back(LoadFile(paths[2]));

        if (paths[3].empty())
        {
            sources.emplace_back("");
        }
        else
            sources.emplace_back(LoadFile(paths[3]));

        sources.emplace_back(LoadFile(paths[4]));

        shader.vertSrc = std::move(sources[0]);
        shader.geomSrc = std::move(sources[1]);
        shader.tscSrc = std::move(sources[2]);
        shader.tseSrc = std::move(sources[3]);
        shader.fragSrc = std::move(sources[4]);

        return true;
    }
} // namespace Vakol

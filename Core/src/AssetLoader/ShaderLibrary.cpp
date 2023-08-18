#include "AssetLoader/ShaderLibrary.hpp"

#include "Rendering/Platform/OpenGL/Buffer.hpp"
#include "Rendering/Platform/OpenGL/Shader.hpp"

#include "Logger/Logger.hpp"
#include "Rendering/RenderData.hpp"

namespace Vakol
{
    std::unordered_map<std::string, unsigned int> ShaderLibrary::m_shaders;

    std::map<unsigned int, std::unordered_map<std::string, Rendering::Uniform>> ShaderLibrary::m_uniforms;
    std::unordered_map<std::string, Rendering::UniformBuffer> ShaderLibrary::m_uniformBuffers;

    void ShaderLibrary::CreateUniformBuffer(const char* uBufferName, const int size, const unsigned int binding)
    {
        Rendering::UniformBuffer uniform_buffer{};

        Rendering::OpenGL::GenBuffers(uniform_buffer.id);

        Rendering::OpenGL::BindUniformBuffer(uniform_buffer.id);
        Rendering::OpenGL::SetUniformBufferData(uniform_buffer.id, size, nullptr);
        Rendering::OpenGL::UnbindUniformBuffer();

        Rendering::OpenGL::BindUniformBufferRange(binding, uniform_buffer.id, 0, size);

        uniform_buffer.binding = binding;

        AddUniformBuffer(uBufferName, uniform_buffer);
    }

    void ShaderLibrary::AddShader(const std::string& ID, const unsigned int shader)
    {
        if (m_shaders.find(ID) == m_shaders.end())
            m_shaders[ID] = shader;
    }

    void ShaderLibrary::AddUniformBuffer(const std::string& uBufferName, const Rendering::UniformBuffer& uBuffer)
    {
        if (m_uniformBuffers.find(uBufferName) == m_uniformBuffers.end())
            m_uniformBuffers[uBufferName] = uBuffer;
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
        {
            m_uniforms[shader].emplace();
        }

        Rendering::OpenGL::GetUniforms(shader, m_uniforms.at(shader));
    }

    void ShaderLibrary::SetInt(const unsigned int shader, const char* name, const int value)
    {
        if (UniformExists(shader, name))
        {
            const auto& [location, count] = GetUniform(shader, name);

            Rendering::OpenGL::SetInt(location, value);
        }
    }

    void ShaderLibrary::SetFloat(const unsigned int shader, const char* name, const float value)
    {
        if (UniformExists(shader, name))
        {
            const auto& [location, count] = GetUniform(shader, name);

            Rendering::OpenGL::SetFloat(location, value);
        }
    }

    void ShaderLibrary::SetVec3(const unsigned int shader, const char* name, const Math::Vec3& value)
    {
        if (UniformExists(shader, name))
        {
            const auto& [location, count] = GetUniform(shader, name);

            Rendering::OpenGL::SetVec3(location, count, Math::AsArray(value));
        }
    }

    void ShaderLibrary::SetVec4(const unsigned int shader, const char* name, const Math::Vec4& value)
    {
        if (UniformExists(shader, name))
        {
            const auto& [location, count] = GetUniform(shader, name);

            Rendering::OpenGL::SetVec4(location, count, Math::AsArray(value));
        }
    }

    void ShaderLibrary::SetMat4(const unsigned int shader, const char* name, const bool transpose,
                                const Math::Mat4& value)
    {
        if (UniformExists(shader, name))
        {
            const auto& [location, count] = GetUniform(shader, name);

            Rendering::OpenGL::SetMat4(location, count, name, transpose, Math::AsArray(value));
        }
    }

    void ShaderLibrary::SetUniformBufferData(const char* name, const int size, const void* data)
    {
        if (UniformBufferExists(name))
        {
            const auto& [id, binding] = GetUniformBuffer(name);

            Rendering::OpenGL::SetUniformBufferData(id, size, data);
        }
    }

    void ShaderLibrary::SetUniformBufferSubData(const char* name, const int offset, const int size, const void* data)
    {
        if (UniformBufferExists(name))
        {
            const auto& [id, binding] = GetUniformBuffer(name);

            Rendering::OpenGL::SetUniformBufferSubData(id, offset, size, data);
        }
    }

    Rendering::Uniform ShaderLibrary::GetUniform(const unsigned int shader, const char* name)
    {
        return m_uniforms.at(shader).at(name);
    }

    Rendering::UniformBuffer ShaderLibrary::GetUniformBuffer(const char* name)
    {
        return m_uniformBuffers.at(name);
    }

    bool ShaderLibrary::UniformExists(const unsigned int shader, const char* name)
    {
        return m_uniforms.at(shader).find(name) != m_uniforms.at(shader).end();
    }

    bool ShaderLibrary::UniformBufferExists(const char* name)
    {
        return m_uniformBuffers.find(name) != m_uniformBuffers.end();
    }
} // namespace Vakol

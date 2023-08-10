#include "ShaderLibrary.hpp"

#include "Platform/OpenGL/Shader.hpp"
#include "RenderData.hpp"
#include "Controller/Logger.hpp"

namespace Vakol::Rendering
{
    std::map<unsigned int, std::unordered_map<std::string, Uniform>> ShaderLibrary::m_uniforms;

    void ShaderLibrary::GetShaderUniforms(const unsigned int shader)
    {
        if (m_uniforms.find(shader) == m_uniforms.end())
            m_uniforms[shader].emplace();

        OpenGL::GetUniforms(shader, m_uniforms.at(shader));
    }

    void ShaderLibrary::SetMat4(const unsigned int shader, const char* name, const bool transpose, const Math::Mat4& value)
    {
        if (m_uniforms.at(shader).find(name) != m_uniforms.at(shader).end())
        {
            const auto& [location, count] = m_uniforms.at(shader).at(name);

            OpenGL::SetMat4(location, count, name, transpose, Math::ToArray(value));
        }
    }
}

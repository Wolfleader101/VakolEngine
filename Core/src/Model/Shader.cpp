#include "Shader.hpp"

#include <glad/glad.h>
namespace Vakol::Model
{
    Shader::~Shader()
    {
        glDeleteShader(this->m_ID);
    };

    void Shader::Bind() const
    {
        glUseProgram(this->m_ID);
    }

    void Shader::Unbind() const
    {
        glUseProgram(0);
    }

    void Shader::SetBool(const char* name, const bool value) const
    {
        Bind();
        glUniform1i(glGetUniformLocation(this->m_ID, name), static_cast<int>(value));
        Unbind();
    }

    void Shader::SetInt(const char* name, const int value) const
    {
        Bind();
        glUniform1i(glGetUniformLocation(this->m_ID, name), value);
        Unbind();
    }

    void Shader::SetFloat(const char* name, const float value) const
    {
        Bind();
        glUniform1f(glGetUniformLocation(this->m_ID, name), value);
        Unbind();
    }

    void Shader::SetVec2(const char* name, const Math::Vec2& value) const
    {
        Bind();
        glUniform2fv(glGetUniformLocation(this->m_ID, name), 1, Math::ToArray(value));
        Unbind();
    }

    void Shader::SetVec2(const char* name, const float x, const float y) const
    {
        Bind();
        glUniform2f(glGetUniformLocation(this->m_ID, name), x, y);
        Unbind();
    }

    void Shader::SetVec3(const char* name, const Math::Vec3& value) const
    {
        Bind();
        glUniform3fv(glGetUniformLocation(this->m_ID, name), 1, Math::ToArray(value));
        Unbind();
    }

    void Shader::SetVec3(const char* name, const float x, const float y, const float z) const
    {
        Bind();
        glUniform3f(glGetUniformLocation(this->m_ID, name), x, y, z);
        Unbind();
    }

    void Shader::SetVec4(const char* name, const Math::Vec4& value) const
    {
        Bind();
        glUniform4fv(glGetUniformLocation(this->m_ID, name), 1, Math::ToArray(value));
        Unbind();
    }

    void Shader::SetVec4(const char* name, const float x, const float y, const float z, const float w) const
    {
        Bind();
        glUniform4f(glGetUniformLocation(this->m_ID, name), x, y, z, w);
        Unbind();
    }

    void Shader::SetMat3(const char* name, const Math::Mat3& value) const
    {
        Bind();
        glUniformMatrix3fv(glGetUniformLocation(this->m_ID, name), 1, GL_FALSE, Math::ToArray(value));
        Unbind();
    }

    void Shader::SetMat4(const char* name, const Math::Mat4& value) const
    {
        Bind();
        glUniformMatrix4fv(glGetUniformLocation(this->m_ID, name), 1, GL_FALSE, Math::ToArray(value));
        Unbind();
    }

    void Shader::SetMat4v(const char* name, const int count, const void* data) const
    {
        Bind();
        glUniformMatrix4fv(glGetUniformLocation(this->m_ID, name), static_cast<GLsizei>(count), GL_FALSE,
                           static_cast<const float*>(data));
        Unbind();
    }
} // namespace Vakol::Model
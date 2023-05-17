#include "Shader.hpp"

#include <glad/glad.h>

#pragma warning(push)
#pragma warning(disable:4201)
#include <glm/gtc/type_ptr.hpp>
#pragma warning(pop)

namespace Vakol::Model 
{	
	Shader::~Shader() { glDeleteShader(this->m_ID); };

    void Shader::Bind() const { glUseProgram(this->m_ID); }

    void Shader::Unbind() const { glUseProgram(0); }

	void Shader::SetBool(const char* name, const bool value) const { glUniform1i(glGetUniformLocation(this->m_ID, name), static_cast<int>(value)); }

	void Shader::SetInt(const char* name, const int value) const { glUniform1i(glGetUniformLocation(this->m_ID, name), value); }

	void Shader::SetFloat(const char* name, const float value) const { glUniform1f(glGetUniformLocation(this->m_ID, name), value); }

	void Shader::SetVec2(const char* name, const glm::vec2& value) const { glUniform2fv(glGetUniformLocation(this->m_ID, name), 1, glm::value_ptr(value)); }

	void Shader::SetVec2(const char* name, const float x, const float y) const { glUniform2f(glGetUniformLocation(this->m_ID, name), x, y); }

	void Shader::SetVec3(const char* name, const glm::vec3& value) const { glUniform3fv(glGetUniformLocation(this->m_ID, name), 1, glm::value_ptr(value)); }

	void Shader::SetVec3(const char* name, const float x, const float y, const float z) const { glUniform3f(glGetUniformLocation(this->m_ID, name), x, y, z); }

	void Shader::SetVec4(const char* name, const glm::vec4& value) const { glUniform4fv(glGetUniformLocation(this->m_ID, name), 1, glm::value_ptr(value)); }

	void Shader::SetVec4(const char* name, const float x, const float y, const float z, const float w) const { glUniform4f(glGetUniformLocation(this->m_ID, name), x, y, z, w); }

	void Shader::SetMat3(const char* name, const glm::mat3& value) const { glUniformMatrix3fv(glGetUniformLocation(this->m_ID, name), 1, GL_FALSE, glm::value_ptr(value)); }

	void Shader::SetMat4(const char* name, const glm::mat4& value) const { glUniformMatrix4fv(glGetUniformLocation(this->m_ID, name), 1, GL_FALSE, glm::value_ptr(value)); }

	void Shader::SetMat4v(const char* name, const int count, const void* data) const { glUniformMatrix4fv(glGetUniformLocation(this->m_ID, name), static_cast<GLsizei>(count), GL_FALSE, static_cast<const float*>(data)); }
}
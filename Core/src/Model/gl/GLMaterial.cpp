#include "GLMaterial.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>
#include <Controller/Logger.hpp>

namespace Vakol::Model
{
	void GLMaterial::Bind() const
	{
		this->m_shader->Bind();
	}

	void GLMaterial::Unbind() const
	{
		this->m_shader->Unbind();
	}

	void GLMaterial::SetBool(const std::string& name, const bool value) const
	{
		glUniform1i(glGetUniformLocation(this->m_shader->GetID(), name.c_str()), (int)value);
	}

	void GLMaterial::SetInt(const std::string& name, const int value) const
	{
		glUniform1i(glGetUniformLocation(this->m_shader->GetID(), name.c_str()), value);
	}

	void GLMaterial::SetFloat(const std::string& name, const float value) const
	{
		glUniform1f(glGetUniformLocation(this->m_shader->GetID(), name.c_str()), value);
	}

	void GLMaterial::SetVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(this->m_shader->GetID(), name.c_str()), 1, &value[0]);
	}

	void GLMaterial::SetVec2(const std::string& name, const float x, const float y) const
	{
		glUniform2f(glGetUniformLocation(this->m_shader->GetID(), name.c_str()), x, y);
	}

	void GLMaterial::SetVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(this->m_shader->GetID(), name.c_str()), 1, &value[0]);
	}

	void GLMaterial::SetVec3(const std::string& name, const float x, const float y, const float z) const
	{
		glUniform3f(glGetUniformLocation(this->m_shader->GetID(), name.c_str()), x, y, z);
	}

	void GLMaterial::SetVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(this->m_shader->GetID(), name.c_str()), 1, &value[0]);
	}

	void GLMaterial::SetMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(this->m_shader->GetID(), name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}

	void GLMaterial::SetMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(this->m_shader->GetID(), name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}
}
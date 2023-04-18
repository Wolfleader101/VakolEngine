#include "GLShader.hpp"

#include <glad/glad.h>

#include <Controller/AssetLoader/ShaderLoader.hpp>
#include <Controller/Logger.hpp>

#include <glm/gtc/type_ptr.hpp>

using Vakol::Controller::LoadShader;

namespace Vakol::Model::Assets {
	GLShader::GLShader(const std::string& path) : Shader(LoadShader(path)) {};

	void GLShader::Bind() const { glUseProgram(this->m_ID); }

	void GLShader::Unbind() const
	{
		glUseProgram(0);
		glDeleteShader(this->m_ID);
	}

	void GLShader::SetBool(const std::string& name, const bool value) const
	{
		glUniform1i(glGetUniformLocation(this->m_ID, name.c_str()), (int)value);
	}

	void GLShader::SetInt(const std::string& name, const int value) const
	{
		glUniform1i(glGetUniformLocation(this->m_ID, name.c_str()), value);
	}

	void GLShader::SetFloat(const std::string& name, const float value) const
	{
		glUniform1f(glGetUniformLocation(this->m_ID, name.c_str()), value);
	}

	void GLShader::SetVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(this->m_ID, name.c_str()), 1, &value[0]);
	}

	void GLShader::SetVec2(const std::string& name, const float x, const float y) const
	{
		glUniform2f(glGetUniformLocation(this->m_ID, name.c_str()), x, y);
	}

	void GLShader::SetVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(this->m_ID, name.c_str()), 1, &value[0]);
	}

	void GLShader::SetVec3(const std::string& name, const float x, const float y, const float z) const
	{
		glUniform3f(glGetUniformLocation(this->m_ID, name.c_str()), x, y, z);
	}

	void GLShader::SetVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(this->m_ID, name.c_str()), 1, &value[0]);
	}

	void GLShader::SetMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(this->m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}

	void GLShader::SetMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(this->m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}

}
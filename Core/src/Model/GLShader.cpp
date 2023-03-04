#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

#include "GLShader.hpp"

namespace Vakol::Model
{
	void GLShader::Bind() const
	{
        glUseProgram(this->id);
	}

    void GLShader::Unbind() const
    {
        glUseProgram(0);
    }

	unsigned int GLShader::GetID() const
	{
		return this->id;
	}

	void GLShader::SetBool(const std::string& name, const bool value)
	{
		glUniform1i(glGetUniformLocation(this->id, name.c_str()), (int)value);
	}

	void GLShader::SetInt(const std::string& name, const int value)
	{
		glUniform1i(glGetUniformLocation(this->id, name.c_str()), value);
	}

	void GLShader::SetFloat(const std::string& name, const float value)
	{
		glUniform1f(glGetUniformLocation(this->id, name.c_str()), value);
	}

	void GLShader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		glUniform2fv(glGetUniformLocation(this->id, name.c_str()), 1, &value[0]);
	}

	void GLShader::SetFloat2(const std::string& name, const float x, const float y)
	{
		glUniform2f(glGetUniformLocation(this->id, name.c_str()), x, y);
	}

	void GLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		glUniform3fv(glGetUniformLocation(this->id, name.c_str()), 1, &value[0]);
	}

	void GLShader::SetFloat3(const std::string& name, const float x, const float y, const float z)
	{
		glUniform3f(glGetUniformLocation(this->id, name.c_str()), x, y, z);
	}

	void GLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		glUniform4fv(glGetUniformLocation(this->id, name.c_str()), 1, &value[0]);
	}

	void GLShader::SetMat3(const std::string& name, const glm::mat3& mat)
	{
		glUniformMatrix3fv(glGetUniformLocation(this->id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}

	void GLShader::SetMat4(const std::string& name, const glm::mat4& mat)
	{
		glUniformMatrix4fv(glGetUniformLocation(this->id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}
}
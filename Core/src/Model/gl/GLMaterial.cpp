#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

#include "GLMaterial.hpp"

namespace Vakol::Model
{
	GLMaterial::GLMaterial(const GLShader& shader)
	{
		this->shader = shader;
		this->texture = GLTexture::GetTexture("assets/smile.png");
	}

	GLMaterial::~GLMaterial()
	{
		//this->shader.~GLShader();
		//
		//const auto textureCount = this->textures.size();

		//for (auto i = 0; i < textureCount; ++i)
		//	this->textures[i].~GLTexture();
	}

	const unsigned int GLMaterial::GetID() const { return shader.GetID(); }

	void GLMaterial::Bind(const unsigned int type) const
	{
		if (type == GL_SHADER)
			this->shader.Bind();
		else
			glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(this->texture));
	}

	void GLMaterial::Unbind() const
	{
		//this->shader.Unbind();

		//const auto textureCount = this->textures.size();

		//for (auto i = 0; i < textureCount; ++i)
		//	this->textures[i].Unbind();
	}

	void GLMaterial::SetBool(const std::string& name, const bool value)
	{
		glUniform1i(glGetUniformLocation(this->shader.GetID(), name.c_str()), (int)value);
	}

	void GLMaterial::SetInt(const std::string& name, const int value)
	{
		glUniform1i(glGetUniformLocation(this->shader.GetID(), name.c_str()), value);
	}

	void GLMaterial::SetFloat(const std::string& name, const float value)
	{
		glUniform1f(glGetUniformLocation(this->shader.GetID(), name.c_str()), value);
	}

	void GLMaterial::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		glUniform2fv(glGetUniformLocation(this->shader.GetID(), name.c_str()), 1, &value[0]);
	}

	void GLMaterial::SetFloat2(const std::string& name, const float x, const float y)
	{
		glUniform2f(glGetUniformLocation(this->shader.GetID(), name.c_str()), x, y);
	}

	void GLMaterial::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		glUniform3fv(glGetUniformLocation(this->shader.GetID(), name.c_str()), 1, &value[0]);
	}

	void GLMaterial::SetFloat3(const std::string& name, const float x, const float y, const float z)
	{
		glUniform3f(glGetUniformLocation(this->shader.GetID(), name.c_str()), x, y, z);
	}

	void GLMaterial::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		glUniform4fv(glGetUniformLocation(this->shader.GetID(), name.c_str()), 1, &value[0]);
	}

	void GLMaterial::SetMat3(const std::string& name, const glm::mat3& mat)
	{
		glUniformMatrix3fv(glGetUniformLocation(this->shader.GetID(), name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}

	void GLMaterial::SetMat4(const std::string& name, const glm::mat4& mat)
	{
		glUniformMatrix4fv(glGetUniformLocation(this->shader.GetID(), name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}
}
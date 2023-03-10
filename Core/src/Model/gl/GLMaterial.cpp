#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

#include "GLMaterial.hpp"

namespace Vakol::Model
{
	GLMaterial::GLMaterial(const GLShader& shader)
	{
		this->shader = shader;
		this->textures["default"] = GLTexture::GetTexture("assets/textures/white.png");
	}

	GLMaterial::~GLMaterial()
	{
		//this->shader.~GLShader();
		//
		//const auto textureCount = this->textures.size();

		//for (auto i = 0; i < textureCount; ++i)
		//	this->textures[i].~GLTexture();
	}

	void GLMaterial::AddTexture(const std::string& path)
	{
		const std::string name = GetName(path);

		if (textures.find(name) == textures.end())
			textures.insert({name, GLTexture::GetTexture(path)});
	}

	void GLMaterial::ReplaceTexture(const std::string& src, const std::string& dst)
	{
		const std::string src_name = GetName(src);
		const std::string dst_name = GetName(dst);

		if (textures.find(src_name) != textures.end())
		{
			textures.erase(src_name);
			AddTexture(dst_name);
		}
	}

	const unsigned int GLMaterial::GetID() const { return shader.GetID(); }
	const std::string GLMaterial::GetName(const std::string& str) const 
	{ 
		return str.substr(str.find_last_of('/'),
						  str.find_last_of('.'));
	}

	void GLMaterial::Bind(const unsigned int type) const
	{
		auto size = textures.size();

		if (type == GL_SHADER)
			this->shader.Bind();
		else
		{
			for (auto i = 0; i < size; i++)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(textures.begin()->second));
			}
		}
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
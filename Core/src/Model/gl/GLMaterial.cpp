#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>
#include <Controller/Logger.hpp>

#include <iostream>

#include "GLMaterial.hpp"

namespace Vakol::Model
{
	//GLMaterial::GLMaterial(const GLShader& shader, const MaterialInfo& data)
	//{
	//	this->shader = shader;

	//	this->_AMBIENT = data._AMBIENT;
	//	this->_DIFFUSE = data._DIFFUSE;
	//	this->_SPECULAR = data._SPECULAR;
	//	this->_EMISSIVE = data._EMISSIVE;

	//	this->_SHININESS = data._SHININESS;

	//	this->textures = data.textures;
	//}

	//GLMaterial::~GLMaterial()
	//{
	//	//this->shader.~GLShader();
	//	//
	//	//const auto textureCount = this->textures.size();

	//	//for (auto i = 0; i < textureCount; ++i)
	//	//	this->textures[i].~GLTexture();
	//}

	//const unsigned int GLMaterial::GetID() const { return shader.GetID(); }
	//
	//const unsigned int GLMaterial::GetTextureCount() const { return static_cast<unsigned int>(textures.size()); }


	//const std::string GLMaterial::GetName(const std::string& str) const 
	//{ 
	//	return str.substr(str.find_last_of('/'),
	//					  str.find_last_of('.'));
	//}

	//void GLMaterial::Bind(const unsigned int type) const
	//{
	//	auto size = static_cast<unsigned int>(textures.size());

	//	if (type == GL_SHADER)
	//		this->shader.Bind();
	//	else if (type == GL_TEXTURE_2D || type == GL_TEXTURE_CUBE_MAP)
	//	{
	//		this->SetInt("matieral.specular", 0);
	//		this->SetInt("material.diffuse", 1);

	//		this->SetFloat("material.shininess", _SHININESS);

	//		this->SetVec3("light.ambient",  _AMBIENT);
	//		this->SetVec3("light.diffuse",  _DIFFUSE);
	//		this->SetVec3("light.specular", _SPECULAR);

	//		for (unsigned int i = 0; i < size; i++)
	//		{
	//			glActiveTexture(GL_TEXTURE0 + i);
	//			glBindTexture(type, textures[i].id);
	//		}
	//	}
	//	else
	//	{
	//		VK_ERROR("ERROR:: UNKNOWN DRAW TYPE\n");
	//		return;
	//	}
	//}

	//void GLMaterial::Unbind() const
	//{
	//	//this->shader.Unbind();

	//	//const auto textureCount = this->textures.size();

	//	//for (auto i = 0; i < textureCount; ++i)
	//	//	this->textures[i].Unbind();
	//}

	//void GLMaterial::SetBool(const std::string& name, const bool value) const
	//{
	//	glUniform1i(glGetUniformLocation(this->shader.GetID(), name.c_str()), (int)value);
	//}

	//void GLMaterial::SetInt(const std::string& name, const int value) const
	//{
	//	glUniform1i(glGetUniformLocation(this->shader.GetID(), name.c_str()), value);
	//}

	//void GLMaterial::SetFloat(const std::string& name, const float value) const
	//{
	//	glUniform1f(glGetUniformLocation(this->shader.GetID(), name.c_str()), value);
	//}

	//void GLMaterial::SetVec2(const std::string& name, const glm::vec2& value) const
	//{
	//	glUniform2fv(glGetUniformLocation(this->shader.GetID(), name.c_str()), 1, &value[0]);
	//}

	//void GLMaterial::SetVec2(const std::string& name, const float x, const float y) const
	//{
	//	glUniform2f(glGetUniformLocation(this->shader.GetID(), name.c_str()), x, y);
	//}

	//void GLMaterial::SetVec3(const std::string& name, const glm::vec3& value) const
	//{
	//	glUniform3fv(glGetUniformLocation(this->shader.GetID(), name.c_str()), 1, &value[0]);
	//}

	//void GLMaterial::SetVec3(const std::string& name, const float x, const float y, const float z) const
	//{
	//	glUniform3f(glGetUniformLocation(this->shader.GetID(), name.c_str()), x, y, z);
	//}

	//void GLMaterial::SetVec4(const std::string& name, const glm::vec4& value) const
	//{
	//	glUniform4fv(glGetUniformLocation(this->shader.GetID(), name.c_str()), 1, &value[0]);
	//}

	//void GLMaterial::SetMat3(const std::string& name, const glm::mat3& mat) const
	//{
	//	glUniformMatrix3fv(glGetUniformLocation(this->shader.GetID(), name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	//}

	//void GLMaterial::SetMat4(const std::string& name, const glm::mat4& mat) const
	//{
	//	glUniformMatrix4fv(glGetUniformLocation(this->shader.GetID(), name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	//}
}
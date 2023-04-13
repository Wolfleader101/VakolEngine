#pragma once

#include <glm/common.hpp>
#include <glm/glm.hpp>

#include <memory>

#include "Texture.hpp"
#include <Model/gl/GLShader.hpp>

using Vakol::Model::GLShader;

namespace Vakol::Model::Assets 
{
	class Material
	{
	public:
		Material(const std::string& path) : m_shader(std::make_shared<GLShader>(path)) {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	
		// virtual const unsigned int GetID() const = 0;
		// virtual const unsigned int GetTextureCount() const = 0;

        // virtual std::vector<Texture> textures() = 0;

	public:
		virtual void SetBool(const std::string& name, const bool value) const = 0;
		virtual void SetInt(const std::string& name, const int value) const = 0;
		virtual void SetFloat(const std::string& name, const float value) const = 0;

		virtual void SetVec2(const std::string& name, const glm::vec2& value) const = 0;
		virtual void SetVec2(const std::string& name, const float x, const float y) const = 0;

		virtual void SetVec3(const std::string& name, const glm::vec3& value) const = 0;
		virtual void SetVec3(const std::string& name, const float x, const float y, const float z) const = 0;

		virtual void SetVec4(const std::string& name, const glm::vec4& value) const = 0;

		virtual void SetMat3(const std::string& name, const glm::mat3& value) const = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) const = 0;

    protected:
        std::shared_ptr<Shader> m_shader;

		//std::vector<Texture> m_textures;
	};
}
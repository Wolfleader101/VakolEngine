#pragma once

#include <vector>

#include "../Material.hpp"

#include "GLShader.hpp"
#include "GLTexture.hpp"

namespace Vakol::Model
{

	class GLMaterial : public Material
	{
	public:
        GLMaterial() = default;
		GLMaterial(const GLShader& shader, const MaterialInfo& data);
		~GLMaterial() override;

		void Bind(const unsigned int type) const override;
		void Unbind() const override;

		const unsigned int GetID() const override;

	public:
		void SetBool(const std::string& name, const bool value) const override;
		void SetInt(const std::string& name, const int value) const override;
		void SetFloat(const std::string& name, const float value) const override;

		void SetVec2(const std::string& name, const glm::vec2& value) const override;
		void SetVec2(const std::string& name, const float x, const float y) const override;

		void SetVec3(const std::string& name, const glm::vec3& value) const override;
		void SetVec3(const std::string& name, const float x, const float y, const float z) const override;

		void SetVec4(const std::string& name, const glm::vec4& value) const override;

		void SetMat3(const std::string& name, const glm::mat3& value) const override;
		void SetMat4(const std::string& name, const glm::mat4& value) const override;

	private:
		const std::string GetName(const std::string& str) const;
	private:
		GLShader shader;
	private:
		glm::vec3 _AMBIENT = glm::vec3(0.0f);
		glm::vec3 _DIFFUSE = glm::vec3(0.0f);
		glm::vec3 _SPECULAR = glm::vec3(0.0f);
		glm::vec3 _EMISSIVE = glm::vec3(0.0f);

		float _SHININESS = 0.5f;
		std::vector<Texture> textures;
	};
}

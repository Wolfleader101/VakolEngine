#pragma once

#include <vector>

#include <Model/Assets/Material.hpp>

#include "GLShader.hpp"
#include "GLTexture.hpp"

namespace Vakol::Model
{
	using Vakol::Model::Assets::Texture;
	
	class GLMaterial : public Vakol::Model::Assets::Material
	{
	public:
		GLMaterial(const std::string& path) : Material(path) {};

		void Bind() const override;
		void Unbind() const override;

		const unsigned int GetID() const override { return this->m_shader->GetID(); };
		const unsigned int GetTextureCount() const override { return static_cast<unsigned int>(this->m_textures.size()); }

        std::vector<Texture> textures() override { return this->m_textures; }

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
	};
}

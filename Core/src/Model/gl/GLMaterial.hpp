#pragma once

#include <vector>

#include <Model/Assets/Material.hpp>

#include "GLShader.hpp"
#include "GLTexture.hpp"

namespace Vakol::Model
{
	using MaterialInfo = Vakol::Model::Assets::MaterialInfo;

	/*class GLMaterial : public Vakol::Model::Assets::Material
	{
	public:
        GLMaterial() = default;
		GLMaterial(const GLShader& shader, const MaterialInfo& data);
		~GLMaterial() override;

		void Bind(const unsigned int type) const override;
		void Unbind() const override;

		const unsigned int GetID() const override;
		const unsigned int GetTextureCount() const;

	public:
		void SetBool(const std::string& name, const bool value) const;
		void SetInt(const std::string& name, const int value) const;
		void SetFloat(const std::string& name, const float value) const;

		void SetVec2(const std::string& name, const glm::vec2& value) const;
		void SetVec2(const std::string& name, const float x, const float y) const;

		void SetVec3(const std::string& name, const glm::vec3& value) const;
		void SetVec3(const std::string& name, const float x, const float y, const float z) const;

		void SetVec4(const std::string& name, const glm::vec4& value) const;

		void SetMat3(const std::string& name, const glm::mat3& value) const;
		void SetMat4(const std::string& name, const glm::mat4& value) const;

	private:
		const std::string GetName(const std::string& str) const;
	private:
		GLShader shader;
	private:
	};*/
}

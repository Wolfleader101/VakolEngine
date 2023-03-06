#pragma once

#include <memory>
#include <vector>

#include "Material.hpp"

#include "GLShader.hpp"
#include "GLTexture.hpp"

namespace Vakol::Model
{
	class GLMaterial : public Material
	{
	public:
		GLMaterial(const std::shared_ptr<GLShader>& shader);
		~GLMaterial() override;

		void Bind() const override;
		void Unbind() const override;

		const unsigned int GetID() const override;

		void AddTexture(const std::shared_ptr<GLTexture>& texture);

		void SetBool(const std::string& name, const bool value) override;
		void SetInt(const std::string& name, const int value) override;
		void SetFloat(const std::string& name, const float value) override;

		void SetFloat2(const std::string& name, const glm::vec2& value) override;
		void SetFloat2(const std::string& name, const float x, const float y) override;

		void SetFloat3(const std::string& name, const glm::vec3& value) override;
		void SetFloat3(const std::string& name, const float x, const float y, const float z) override;

		void SetFloat4(const std::string& name, const glm::vec4& value) override;

		void SetMat3(const std::string& name, const glm::mat3& value) override;
		void SetMat4(const std::string& name, const glm::mat4& value) override;
	private:
		std::vector<std::shared_ptr<GLTexture>> textures;
		std::shared_ptr<GLShader> shader = nullptr;
	};
}

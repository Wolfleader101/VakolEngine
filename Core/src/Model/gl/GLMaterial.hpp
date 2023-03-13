#pragma once

#include <memory>
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
		GLMaterial(const GLShader& shader);
		~GLMaterial() override;

		void Bind(const unsigned int type) const override;
		void Unbind() const override;

		const unsigned int GetID() const override;

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

		unsigned int texture;
	private:
		GLShader shader;
	};
}
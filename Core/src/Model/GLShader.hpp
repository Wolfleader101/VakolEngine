#pragma once

#include "Shader.hpp"

namespace Vakol::Model
{
	class GLShader : public Shader
	{
		~GLShader() override;

        void SetBool(const std::string& name, const bool value) override;
        void SetInt(const std::string& name, const int value) override;
        void SetFloat(const std::string& name, const float value) override;

        void SetFloat2(const std::string& name, const glm::vec2&) override;
        void SetFloat2(const std::string& name, const float x, const float y) override;

        void SetFloat3(const std::string& name, const glm::vec3&) override;
        void SetFloat3(const std::string& name, const float x, const float y, const float z) override;

        void SetFloat4(const std::string& name, const glm::vec4&) override;

        void SetMat3(const std::string& name, const glm::mat3&) override;
        void SetMat4(const std::string& name, const glm::mat4&) override;
	};
}
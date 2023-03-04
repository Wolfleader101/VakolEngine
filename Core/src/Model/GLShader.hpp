#pragma once

#include "Shader.hpp"

namespace Vakol::Model
{
	class GLShader : public Shader
	{
    public:
        GLShader(const std::string& vertex, const std::string& fragment);
        GLShader(const std::string& program);

		~GLShader() override;

        void Bind() const override;
        void Unbind() const override;

        unsigned int GetID() const;

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
        unsigned int id;
	};
}
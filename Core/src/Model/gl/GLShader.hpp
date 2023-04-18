#pragma once

#include <Model/Assets/Shader.hpp>

#include <memory>

namespace Vakol::Model::Assets 
{
    class GLShader : public Shader 
    {
    public:
        GLShader(const std::string& path);
        GLShader();

        void Bind() const override;
        void Unbind() const override;

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
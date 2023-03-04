#pragma once

#include <string>

#include "Math/Math.hpp"

namespace Vakol::Model 
{
    class Shader 
    {
    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetBool(const std::string& name, const bool value) = 0;
        virtual void SetInt(const std::string& name, const int value) = 0;
        virtual void SetFloat(const std::string& name, const float value) = 0;

        virtual void SetFloat2(const std::string& name, const glm::vec2&) = 0;
        virtual void SetFloat2(const std::string& name, const float x, const float y) = 0;

        virtual void SetFloat3(const std::string& name, const glm::vec3&) = 0;
        virtual void SetFloat3(const std::string& name, const float x, const float y, const float z) = 0;

        virtual void SetFloat4(const std::string& name, const glm::vec4&) = 0;

        virtual void SetMat3(const std::string& name, const glm::mat3&) = 0;
        virtual void SetMat4(const std::string& name, const glm::mat4&) = 0;
    };
}
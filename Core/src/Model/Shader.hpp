#pragma once

#include <string>

namespace Vakol::Model 
{
    class Shader 
    {
    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        // Shader(const std::string& vertex, const std::string& fragment) const;

        // void Use() const;
        // void Disable() const;

        const unsigned int ID() const { return this->id; }

    public:
        // virtual void SetBool(const std::string& name, const bool value) = 0;
        // virtual void SetInt(const std::string& name, const int value) = 0;
        // void SetFloat(const std::string& name, const float value) const;

        // void SetFloat2(const std::string& name, const glm::vec2&) const;
        // void SetFloat2(const std::string& name, const float x, const float y) const;

        // void SetFloat3(const std::string& name, const glm::vec3&) const;
        // void SetFloat3(const std::string& name, const float x, const float y, const float z) const;

        // void SetFloat4(const std::string& name, const glm::vec4&) const;

        // void SetMat3(const std::string& name, const glm::mat3&) const;
        // void SetMat4(const std::string& name, const glm::mat4&) const;
    private:
        unsigned int id;
    };
}
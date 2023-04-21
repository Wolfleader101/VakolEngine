#pragma once

#include <string>

#include <glm/glm.hpp>

namespace Vakol::Model::Assets 
{
    class Shader 
    {
    public:
        Shader(const unsigned int id) : m_ID(id) {};
        
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual const unsigned int GetID() const = 0;

    public:
        virtual void SetBool(const std::string& name, const bool value) const = 0;

        virtual void SetInt(const std::string& name, const int value) const = 0;

        virtual void SetFloat(const std::string& name, const float value) const = 0;

        virtual void SetVec2v(const std::string& name, const glm::vec2& value) const = 0;
        virtual void SetVec2(const std::string& name, const float x, const float y) const = 0;

        virtual void SetVec3v(const std::string& name, const glm::vec3& value) const = 0;
        virtual void SetVec3(const std::string& name, const float x, const float y, const float z) const = 0;

        virtual void SetVec4v(const std::string& name, const glm::vec4& value) const = 0;
        virtual void SetVec4(const std::string& name, const float x, const float y, const float z, const float w) const = 0;

        virtual void SetMat3(const std::string& name, const glm::mat3& value) const = 0;
        virtual void SetMat4(const std::string& name, const glm::mat4& value) const = 0;

    protected:
        unsigned int m_ID = 0;
    };
}
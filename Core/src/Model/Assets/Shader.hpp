#pragma once

#include <string>

#include <glm/mat4x4.hpp>

namespace Vakol::Model::Assets
{
    class Shader {
       public:
        Shader(const unsigned int id) : id(id){};
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetBool(const std::string& name, const bool value) const = 0;
        virtual void SetInt(const std::string& name, const int value) const = 0;
        virtual void SetFloat(const std::string& name, const float value) const = 0;
        virtual void SetMat4(const std::string& name, const glm::mat4& value) const = 0;

        const virtual unsigned int GetID() const = 0;

       protected:
        unsigned int id;
    };
}
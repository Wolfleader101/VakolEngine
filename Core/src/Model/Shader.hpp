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

        virtual const unsigned int GetID() const = 0;

        static std::string ReadFile(const std::string& path);
    };
}

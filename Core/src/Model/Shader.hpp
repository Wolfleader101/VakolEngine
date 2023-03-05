#pragma once

namespace Vakol::Model 
{
    class Shader 
    {
    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
    };
}
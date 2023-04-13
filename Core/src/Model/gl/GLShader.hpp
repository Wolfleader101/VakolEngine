#pragma once

#include <Model/Assets/Shader.hpp>

namespace Vakol::Model
{
    class GLShader : public Assets::Shader 
    {
       public:
        GLShader(const std::string& path);

        void Bind() const override;
        void Unbind() const override;

        const unsigned int GetID() const override;
    };
}
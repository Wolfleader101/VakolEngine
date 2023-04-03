#pragma once

#include <Model/Assets/Shader.hpp>

namespace Vakol::Model
{
    class GLShader : public Assets::Shader {
       public:
        GLShader(const std::string& path);

        void Bind() const override;
        void Unbind() const override;

        void SetBool(const std::string& name, const bool value) const override;
        void SetInt(const std::string& name, const int value) const override;
        void SetFloat(const std::string& name, const float value) const override;
        void SetMat4(const std::string& name, const glm::mat4& value) const override;

        const unsigned int GetID() const override;
    };
}
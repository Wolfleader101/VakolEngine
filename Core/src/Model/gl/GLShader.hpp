#pragma once

#include <Model/Assets/Shader.hpp>

namespace Vakol::Model::Asset {
    class GLShader : public Asset::Shader {
       public:
        GLShader(const std::string& path);

        void Bind() const override;
        void Unbind() const override;
    };
}  // namespace Vakol::Model
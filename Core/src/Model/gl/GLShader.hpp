#pragma once

#include <Model/Assets/Shader.hpp>

namespace Vakol::Model::Assets {
    class GLShader : public Shader {
       public:
        GLShader(const std::string& path);

        void Bind() const override;
        void Unbind() const override;
    };
}  // namespace Vakol::Model
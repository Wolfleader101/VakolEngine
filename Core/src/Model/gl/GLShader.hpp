#pragma once

#include "../Shader.hpp"

namespace Vakol::Model
{
	class GLShader : public Shader
	{
    public:
        GLShader() = default;
        GLShader(const std::string& vertexPath, const std::string& fragmentPath);

		~GLShader() override;

        void Bind() const override;
        void Unbind() const override;

        const unsigned int GetID() const override;

    private:
        unsigned int id = 0;
        std::string name = "null";
    private:
        void CompileShader(const std::string& vertex, const std::string& fragment);
        void CheckCompileErrors(const unsigned int shader, const std::string& type);
	};
}
#pragma once

#include <string>
//#include <map>

#include "Shader.hpp"

namespace Vakol::Model
{
	class GLShader : public Shader
	{
    public:
        GLShader(const std::string& vertex, const std::string& fragment);
        GLShader(const std::string& program);

		~GLShader() override;

        void Bind() const override;
        void Unbind() const override;

        unsigned int GetID() const;

    private:
        unsigned int id = 0;
        std::string name = "null";
    private:
        std::string ReadFile(const std::string& name);
        void CreateProgram(const std::string& vertex, const std::string& fragment);

        void CompileShader(const std::string& vertex, const std::string& fragment);
        void CheckCompileErrors(const unsigned int shader, const std::string& type);
	};

    //class ShaderLibrary
    //{
    //public:
    //    static void AddShader(const std::string& name, const std::shared_ptr<GLShader>& shader);

    //    static std::shared_ptr<GLShader> GetShader(const std::string& name);

    //    static bool Contains(const std::string& name);

    //private:
    //    static std::map<std::string, std::shared_ptr<GLShader>> shaders;
    //};
}
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Controller/Logger.hpp>

#include <Controller/AssetLoader/ShaderLoader.hpp>

#include "GLShader.hpp"
#include <glm/gtc/type_ptr.hpp>

using Vakol::Controller::LoadShader;

namespace Vakol::Model
{
    GLShader::GLShader(const std::string& path) : Shader(LoadShader(path)) {};

    void GLShader::Bind() const { glUseProgram(this->id); }

    void GLShader::Unbind() const 
    {
        glUseProgram(0);
        glDeleteShader(this->id);
    }

    const unsigned int GLShader::GetID() const { return this->id; }
}
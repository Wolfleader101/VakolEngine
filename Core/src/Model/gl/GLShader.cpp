#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Controller/Logger.hpp>

#include <Controller/AssetLoader/ShaderLoader.hpp>

#include "GLShader.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace Vakol::Model
{
    GLShader::GLShader(const std::string& path) : Shader(id) { this->id = Vakol::Controller::LoadShader(path); }

    void GLShader::Bind() const { glUseProgram(this->id); }

    void GLShader::Unbind() const {
        glUseProgram(0);
        glDeleteShader(this->id);
    }

    const unsigned int GLShader::GetID() const { return this->id; }

    void GLShader::SetBool(const std::string& name, const bool value) const {
        glUniform1i(glGetUniformLocation(this->id, name.c_str()), static_cast<int>(value));
    }

    void GLShader::SetInt(const std::string& name, const int value) const {
        glUniform1i(glGetUniformLocation(this->id, name.c_str()), value);
    }

    void GLShader::SetFloat(const std::string& name, const float value) const {
        glUniform1f(glGetUniformLocation(this->id, name.c_str()), value);
    }

    void GLShader::SetMat4(const std::string& name, const glm::mat4& value) const {
        glUniformMatrix4fv(glGetUniformLocation(this->id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }
}
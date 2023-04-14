#include "GLShader.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <Controller/AssetLoader/ShaderLoader.hpp>
#include <Controller/Logger.hpp>
#include <glm/gtc/type_ptr.hpp>

using Vakol::Controller::LoadShader;

namespace Vakol::Model::Assets {
    GLShader::GLShader(const std::string& path) : Shader(LoadShader(path)){};

    void GLShader::Bind() const { glUseProgram(this->id); }

    void GLShader::Unbind() const {
        glUseProgram(0);
        glDeleteShader(this->id);
    }

}  // namespace Vakol::Model
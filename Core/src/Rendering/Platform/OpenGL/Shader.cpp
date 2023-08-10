#include "Shader.hpp"

#include <glad/glad.h>

#include "Controller/Logger.hpp"
#include "Rendering/RenderData.hpp"

std::string ToString(unsigned int shader);

namespace Vakol::Rendering::OpenGL
{
    unsigned int GenerateShader(const char* source, unsigned int type);
    int CheckCompileErrors(unsigned int shader, unsigned int type);

	unsigned int GenerateShaderProgram(std::vector<std::string>&& sources)
    {
        const auto vertex = GenerateShader(sources[0].c_str(), GL_VERTEX_SHADER);

        unsigned int geometry = 0u;
        unsigned int tessControl = 0u;
        unsigned int tessEval = 0u;

        if (!sources[1].empty())
            geometry = GenerateShader(sources[1].c_str(), GL_GEOMETRY_SHADER);

        if (!sources[2].empty())
            tessControl = GenerateShader(sources[2].c_str(), GL_TESS_CONTROL_SHADER);

        if (!sources[3].empty())
            tessEval = GenerateShader(sources[3].c_str(), GL_TESS_EVALUATION_SHADER);

        const auto fragment = GenerateShader(sources[4].c_str(), GL_FRAGMENT_SHADER);

        // Program Setup
        auto AttachShader = [](const unsigned int shaderProgram, const unsigned int shader) -> void { glAttachShader(shaderProgram, shader); };
        auto DeleteShader = [](const unsigned int shader) -> void { glDeleteShader(shader); };
        auto LinkProgram =  [](const unsigned int shaderProgram) -> void { glLinkProgram(shaderProgram); };

        /*CREATE PROGRAM*/
        const auto program = glCreateProgram();

        /*ATTACH SHADERS*/
        AttachShader(program, vertex);

        if (geometry != 0)
        {
            AttachShader(program, geometry);
        }

	    if (tessControl != 0)
	    {
            AttachShader(program, tessControl);
	    }

        if (tessEval != 0)
        {
	        AttachShader(program, tessEval);
        }

        AttachShader(program, fragment);

        /*LINK PROGRAM*/
        LinkProgram(program);

        /*DELETE SHADERS*/
        DeleteShader(vertex);

        if (geometry != 0)
        {
            DeleteShader(geometry);
        }

	    if (tessControl != 0)
	    {
            DeleteShader(tessControl);
	    }

	    if (tessEval != 0)
        {
            DeleteShader(tessEval);
        }

        DeleteShader(fragment);

        return program;
    }

    unsigned int GenerateShader(const char* source, const unsigned int type)
	{
	    const auto shader = glCreateShader(type);

        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        CheckCompileErrors(shader, type);

        return shader;
	}

    int CheckCompileErrors(const unsigned int shader, const unsigned int type)
    {
        int success;
        char info_log[1024];

        if (type != GL_PROGRAM)
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, nullptr, info_log);
                VK_ERROR("ERROR::SHADER_COMPILATION:: TYPE = {0}\n{1}", ToString(type), info_log);
            }

            return success;
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);

            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, nullptr, info_log);
                VK_ERROR("ERROR::PROGRAM_LINKING:: TYPE = {0}\n{1}", ToString(type), info_log);
            }

            return success;
        }
    }

    void BindShaderProgram(const unsigned int program)
    {
        glUseProgram(program);
    }

    void UnbindShaderProgram()
    {
        glUseProgram(0);
    }

    void GetUniforms(const unsigned int shader, std::unordered_map<std::string, Uniform>& uniforms)
    {
        GLint uniformCount = 0;

        glGetProgramiv(shader, GL_ACTIVE_UNIFORMS, &uniformCount);

        if (uniformCount != 0)
        {
            GLint max_name_len = 0;
            GLsizei length = 0;
            GLsizei count = 0;
            GLenum  type = GL_NONE;

            glGetProgramiv(shader, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_len);

            const auto uniformName = std::make_unique<char[]>(max_name_len);

            for (GLint i = 0; i < uniformCount; ++i)
            {
                glGetActiveUniform(shader, i, max_name_len, &length, &count, &type, uniformName.get());

                Uniform info = {};
                info.location = glGetUniformLocation(shader, uniformName.get());
                info.count = count;

                uniforms.emplace(std::make_pair(std::string(uniformName.get(), length), info));
            }
        }
    }

    void SetMat4(const int location, const int count, const char* name, const bool transpose, const float* value)
    {
        glUniformMatrix4fv(location, count, transpose, value);
    }

}

std::string ToString(const unsigned int shader)
{
    switch (shader)
    {
    case GL_VERTEX_SHADER:
        return "VERTEX_SHADER";
    case GL_GEOMETRY_SHADER:
        return "GEOMETRY_SHADER";
    case GL_TESS_CONTROL_SHADER:
        return "TESSELLATION_CONTROL_SHADER";
    case GL_TESS_EVALUATION_SHADER:
        return "TESSELLATION_EVALUATION_SHADER";
    case GL_FRAGMENT_SHADER:
        return "FRAGMENT_SHADER";
    default:
        break;
    }

    return "IF THIS IS DISPLAYED, I'VE DONE SOMETHING WRONG!";
}
#include "Rendering/Assets/Shader.hpp"

std::string ToString(unsigned int shader);

namespace Vakol::Rendering::OpenGL
{
    unsigned int GenerateShader(const char* source, unsigned int type);
    int CheckCompileErrors(unsigned int shader, unsigned int type);

    unsigned int GenerateShaderProgram(const std::string& vertSrc, const std::string& geomSrc,
                                       const std::string& tscSrc, const std::string& tseSrc, const std::string& fragSrc)
    {
        const auto vertex = GenerateShader(vertSrc.c_str(), GL_VERTEX_SHADER);

        unsigned int geometry = 0u;
        unsigned int tessControl = 0u;
        unsigned int tessEval = 0u;

        if (!geomSrc.empty())
            geometry = GenerateShader(geomSrc.c_str(), GL_GEOMETRY_SHADER);

        if (!tscSrc.empty())
            tessControl = GenerateShader(tscSrc.c_str(), GL_TESS_CONTROL_SHADER);

        if (!tseSrc.empty())
            tessEval = GenerateShader(tseSrc.c_str(), GL_TESS_EVALUATION_SHADER);

        const auto fragment = GenerateShader(fragSrc.c_str(), GL_FRAGMENT_SHADER);

        /*CREATE PROGRAM*/
        const auto program = glCreateProgram();

        /*ATTACH SHADERS*/
        glAttachShader(program, vertex);

        if (geometry != 0)
        {
            glAttachShader(program, geometry);
        }

        if (tessControl != 0)
        {
            glAttachShader(program, tessControl);
        }

        if (tessEval != 0)
        {
            glAttachShader(program, tessEval);
        }

        glAttachShader(program, fragment);

        /*LINK PROGRAM*/
        glLinkProgram(program);

        /*DELETE SHADERS*/
        glDeleteShader(vertex);

        if (geometry != 0)
        {
            glDeleteShader(geometry);
        }

        if (tessControl != 0)
        {
            glDeleteShader(tessControl);
        }

        if (tessEval != 0)
        {
            glDeleteShader(tessEval);
        }

        glDeleteShader(fragment);

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

    void GetUniforms(const unsigned int shader, std::vector<ShaderProcessing::UniformIdentifier>& uniforms)
    {
        GLint uniformCount = 0;

        glGetProgramiv(shader, GL_ACTIVE_UNIFORMS, &uniformCount);

        if (uniformCount != 0)
        {
            GLint max_name_len = 0;
            GLsizei length = 0;
            GLsizei count = 0;
            GLenum type = GL_NONE;

            glGetProgramiv(shader, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_len);

            const auto uniformName = std::make_unique<char[]>(max_name_len);

            for (GLint i = 0; i < uniformCount; ++i)
            {
                glGetActiveUniform(shader, i, max_name_len, &length, &count, &type, uniformName.get());

                Uniform info = {};
                info.location = glGetUniformLocation(shader, uniformName.get());
                info.count = count;

                ShaderProcessing::UniformIdentifier uniformIdentifier;
                uniformIdentifier.uniformName = std::string(uniformName.get(), length);
                uniformIdentifier.uniformVariable = info;

                uniforms.emplace_back(uniformIdentifier);
            }
        }
    }

    void SetBool(const int location, const bool value)
    {
        glUniform1i(location, value);
    }

    void SetFloat(const int location, const float value)
    {
        glUniform1f(location, value);
    }

    void SetInt(const int location, const int value)
    {
        glUniform1i(location, value);
    }

    void SetUInt(const int location, const unsigned int value)
    {
        glUniform1ui(location, value);
    }

    void SetVec2(const int location, const int count, const float* value)
    {
        glUniform2fv(location, count, value);
    }

    void SetVec3(const int location, const int count, const float* value)
    {
        glUniform3fv(location, count, value);
    }

    void SetVec4(const int location, const int count, const float* value)
    {
        glUniform4fv(location, count, value);
    }

    void SetMat3(const int location, const int count, const char* name, const bool transpose, const float* value)
    {
        glUniformMatrix3fv(location, count, transpose, value);
    }

    void SetMat4(const int location, const int count, const char* name, const bool transpose, const float* value)
    {
        glUniformMatrix4fv(location, count, transpose, value);
    }
} // namespace Vakol::Rendering::OpenGL

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

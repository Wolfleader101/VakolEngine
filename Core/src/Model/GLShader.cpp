#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>

#include "GLShader.hpp"

namespace Vakol::Model
{
	std::string GLShader::ReadFile(const std::string& name)
	{
		const std::string filepath = "../Core/assets/" + name;

		std::string result;

		std::ifstream in(filepath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII

		if (in)
		{
			in.seekg(0, std::ios::end);

			size_t size = in.tellg();

			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
			}
			else
				std::cout << "Could not read file: " << filepath << std::endl;
		}
		else
			std::cout << "Could not open file: " << filepath << std::endl;

		return result;
	}

	GLShader::GLShader(const std::string& vertex, const std::string& fragment)
	{
		std::string vsCode = ReadFile(vertex);
		std::string fsCode = ReadFile(fragment);

		CompileShader(vsCode, fsCode);
	}

	GLShader::GLShader(const std::string& program)
	{
		std::string sCode = ReadFile(program);
	}

	void GLShader::CompileShader(const std::string& vertex, const std::string& fragment)
	{
		GLuint vert, frag;

		const auto v_code = std::make_unique<const char*>(vertex.c_str());
		const auto f_code = std::make_unique<const char*>(fragment.c_str());

		vert = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vert, 1, v_code.get(), NULL);
		glCompileShader(vert);

		CheckCompileErrors(vert, "VERTEX");

		frag = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(frag, 1, f_code.get(), NULL);
		glCompileShader(frag);

		CheckCompileErrors(frag, "FRAGMENT");

		this->id = glCreateProgram();
		glAttachShader(this->id, vert);
		glAttachShader(this->id, frag);
		glLinkProgram(this->id);

		CheckCompileErrors(this->id, "PROGRAM");

		glDeleteShader(vert);
		glDeleteShader(frag);

		//CreateProgram(vertex, fragment);
	}

	void GLShader::CreateProgram(const std::string& vertex, const std::string& fragment)
	{

	}

	void GLShader::CheckCompileErrors(const unsigned int shader, const std::string& type)
	{
		int success;

		char infoLog[1024];

		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "\nERROR::SHADER_COMPILATION_ERROR of type: " << type << '\n' << infoLog << '\n' << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);

			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << '\n' << infoLog << '\n' << std::endl;
			}
		}
	}

	GLShader::~GLShader()
	{
		glDeleteProgram(this->id);
	}

	void GLShader::Bind() const
	{
        glUseProgram(this->id);
	}

    void GLShader::Unbind() const
    {
        glUseProgram(0);
    }

	unsigned int GLShader::GetID() const
	{
		return this->id;
	}

	void GLShader::SetBool(const std::string& name, const bool value)
	{
		glUniform1i(glGetUniformLocation(this->id, name.c_str()), (int)value);
	}

	void GLShader::SetInt(const std::string& name, const int value)
	{
		glUniform1i(glGetUniformLocation(this->id, name.c_str()), value);
	}

	void GLShader::SetFloat(const std::string& name, const float value)
	{
		glUniform1f(glGetUniformLocation(this->id, name.c_str()), value);
	}

	void GLShader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		glUniform2fv(glGetUniformLocation(this->id, name.c_str()), 1, &value[0]);
	}

	void GLShader::SetFloat2(const std::string& name, const float x, const float y)
	{
		glUniform2f(glGetUniformLocation(this->id, name.c_str()), x, y);
	}

	void GLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		glUniform3fv(glGetUniformLocation(this->id, name.c_str()), 1, &value[0]);
	}

	void GLShader::SetFloat3(const std::string& name, const float x, const float y, const float z)
	{
		glUniform3f(glGetUniformLocation(this->id, name.c_str()), x, y, z);
	}

	void GLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		glUniform4fv(glGetUniformLocation(this->id, name.c_str()), 1, &value[0]);
	}

	void GLShader::SetMat3(const std::string& name, const glm::mat3& mat)
	{
		glUniformMatrix3fv(glGetUniformLocation(this->id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}

	void GLShader::SetMat4(const std::string& name, const glm::mat4& mat)
	{
		glUniformMatrix4fv(glGetUniformLocation(this->id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}
}
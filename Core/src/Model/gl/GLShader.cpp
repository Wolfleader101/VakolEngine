#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <Controller/Logger.hpp>

#include "GLShader.hpp"

namespace Vakol::Model
{
	std::string GLShader::ReadFile(const std::string& path)
	{
		std::string result;

		std::ifstream in(path, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII

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
				VK_ERROR("Could not read file: {0}", path);
		}
		else
			VK_ERROR("Could not open file: {0}", path);

		return result;
	}

	GLShader::GLShader(const std::string& vertex, const std::string& fragment)
	{
		std::string vsCode = ReadFile(vertex);
		std::string fsCode = ReadFile(fragment);

		CompileShader(vsCode, fsCode);
	}

	void GLShader::CompileShader(const std::string& vertex, const std::string& fragment)
	{
		GLuint vert, frag;

		const auto v_code = vertex.c_str();
		const auto f_code = fragment.c_str();

		vert = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vert, 1, &v_code, NULL);
		glCompileShader(vert);

		CheckCompileErrors(vert, "VERTEX");

		frag = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(frag, 1, &f_code, NULL);
		glCompileShader(frag);

		CheckCompileErrors(frag, "FRAGMENT");

		this->id = glCreateProgram();
		glAttachShader(this->id, vert);
		glAttachShader(this->id, frag);
		glLinkProgram(this->id);

		CheckCompileErrors(this->id, "PROGRAM");

		glDeleteShader(vert);
		glDeleteShader(frag);

		delete v_code;
		delete f_code;
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
				VK_ERROR("\nERROR::SHADER_COMPILATION_ERROR of type: {0}\n{1}\n", type, infoLog);
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);

			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				VK_ERROR("ERROR::PROGRAM_LINKING_ERROR of type: {0}\n{1}\n ", type, infoLog);
			}
		}
	}

	GLShader::~GLShader()
	{
		//glDeleteProgram(this->id);
	}

	void GLShader::Bind() const
	{
        glUseProgram(this->id);
	}

    void GLShader::Unbind() const
    {
        glUseProgram(0);
    }

	const unsigned int GLShader::GetID() const
	{
		return this->id;
	}

	/*void ShaderLibrary::AddShader(const std::string& name, const std::shared_ptr<GLShader>& shader)
	{
		assert(!Contains(name));

		shaders[name] = shader;
	}

	std::shared_ptr<GLShader> ShaderLibrary::GetShader(const std::string& name)
	{
		assert(Contains(name));

		return shaders.at(name);
	}

	bool ShaderLibrary::Contains(const std::string& name)
	{
		return shaders.find(name) != shaders.end();
	}*/
}
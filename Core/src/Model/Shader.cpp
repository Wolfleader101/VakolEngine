#include "Shader.hpp"

#include "GLShader.hpp"

namespace Vakol::Model
{
	std::shared_ptr<Shader> Shader::Create(const std::string& vertex, const std::string& fragment)
	{
		return std::make_shared<GLShader>(vertex, fragment);
	}

	std::shared_ptr<Shader> Shader::Create(const std::string& program)
	{
		return std::make_shared<GLShader>(program);
	}
}
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GLTexture.hpp"

namespace Vakol::Model
{
	GLTexture::GLTexture(const std::string& name)
	{
		// TODO: ADD IO LOADER
	}

	GLTexture::~GLTexture()
	{
		glDeleteTextures(1, &this->id);
	}

	void GLTexture::Bind() const
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->id);
	}

	void GLTexture::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	unsigned int GLTexture::GetID() const
	{
		return this->id;
	}
}
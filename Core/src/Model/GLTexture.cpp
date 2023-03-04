#define STBI_IMAGE_IMPLEMENTATION

#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GLTexture.hpp"

#include <stb_image.h>

namespace Vakol::Model
{
	GLTexture::GLTexture(const std::string& name)
	{
		std::
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
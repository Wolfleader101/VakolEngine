#define STBI_IMAGE_IMPLEMENTATION

#include <string>
#include <vector>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

#include <iostream>

#include "GLTexture.hpp"

namespace Vakol::Model
{
	std::unique_ptr<unsigned char*> GLTexture::GetTextureFromFile(const std::string& name, int& width, int& height, int& colorDepth) const
	{
		const std::string path = "./Core/assets" + name;

		stbi_set_flip_vertically_on_load(true);

		return std::make_unique<unsigned char*>(stbi_load(name.c_str(), &width, &height, &colorDepth, 0));
	}

	GLTexture::GLTexture(const std::string& name)
	{
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &this->id);

		int width, height, colorDepth;
		const auto data = GetTextureFromFile(name, width, height, colorDepth);

		if (data.get()[0])
		{
			GLenum format = (colorDepth > 3) ? GL_RGBA : GL_RGB;

			glBindTexture(GL_TEXTURE_2D, this->id);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data.get()[0]);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data.get()[0]);
		}
		else
		{
			std::cout << "Texture File: " << name << " Failed to Load!" << std::endl;
			stbi_image_free(data.get()[0]);
		}
	}

	GLTexture::~GLTexture()
	{
		glDeleteTextures(1, &this->id);
	}

	void GLTexture::Bind(const unsigned int unit) const
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, this->id);
	}

	void GLTexture::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	const unsigned int GLTexture::GetID() const
	{
		return this->id;
	}
}
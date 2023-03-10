#define STB_IMAGE_IMPLEMENTATION

#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

#include <Controller/Logger.hpp>

#include "GLTexture.hpp"

namespace Vakol::Model
{
	unsigned int GLTexture::GetTexture(const std::string& path)
	{
		unsigned int textureID;

		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &textureID);

		stbi_set_flip_vertically_on_load(true);

		int width, height, colorDepth;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &colorDepth, 0);

		if (data)
		{
			GLenum format = (colorDepth > 3) ? GL_RGBA : GL_RGB;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			stbi_image_free(data);
		}
		else
		{
			VK_ERROR("Failed to load file {0}", path);
			stbi_image_free(data);

			textureID = GetTexture("default.png");
		}

		glDisable(GL_TEXTURE_2D);

		return textureID;
	}
}
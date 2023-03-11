#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GLTexture.hpp"

namespace Vakol::Model
{
	unsigned int GLTexture::GetTextureCubemap(const std::vector<std::string>& faces, const bool flip)
	{
		unsigned int textureID;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		int width, height, colorDepth;
		
		for (auto i = 0; i < faces.size(); i++)
		{
			unsigned char* data = Texture::GetImage(faces[i], flip, width, height, colorDepth);

			GLenum format = (colorDepth > 3) ? GL_RGBA : GL_RGB;

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

			Texture::FreeImage(data);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);

		return textureID;
	}

	unsigned int GLTexture::GetTexture(const std::string& path, const bool flip)
	{
		unsigned int textureID;

		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &textureID);

		int width, height, colorDepth;
		unsigned char* data = Texture::GetImage(path, flip, width, height, colorDepth);

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

		Texture::FreeImage(data);

		glDisable(GL_TEXTURE_2D);

		return textureID;
	}
}
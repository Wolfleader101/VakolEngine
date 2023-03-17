#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GLTexture.hpp"

using Vakol::Model::GetImage;
using Vakol::Model::FreeImage;

namespace Vakol::Model
{
	unsigned int GetTextureCubemap(const std::vector<std::string>& faces, const bool flip)
	{
		unsigned int textureID;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		int width, height, colorDepth;
		
		for (auto i = 0; i < faces.size(); i++)
		{
			unsigned char* data = GetImage(faces[i], flip, width, height, colorDepth);

			GLenum format = (colorDepth > 3) ? GL_RGBA : GL_RGB;

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

			FreeImage(data);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);

		return textureID;
	}

	unsigned int GetTexture(const std::string& path, const bool flip)
	{
		unsigned int textureID;

		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &textureID);

		int width, height, colorDepth;
		unsigned char* data = GetImage(path, flip, width, height, colorDepth);

		GLenum format = (colorDepth == 1) ? GL_RED : (colorDepth > 3) ? GL_RGBA : GL_RGB;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		FreeImage(data);

		glDisable(GL_TEXTURE_2D);

		return textureID;
	}
}
#define STB_IMAGE_IMPLEMENTATION

#include "Texture.hpp"

#include <Controller/Logger.hpp>

#include <stb_image.h>

namespace Vakol::Model
{
	unsigned char* GetImage(const std::string& path, const bool flip, int& width, int& height, int& colorDepth)
	{
		stbi_set_flip_vertically_on_load(flip);

		unsigned char* image = stbi_load(path.c_str(), &width, &height, &colorDepth, 0);

		if (!image)
		{
			VK_ERROR("Failed to load file {0}", path);
			stbi_image_free(image);

			return stbi_load("assets/textures/Error/DEBUG_ErrorTex_Var1_64.png", &width, &height, &colorDepth, 0);
		}

		return image;
	}

	void FreeImage(unsigned char* image)
	{
		stbi_image_free(image);
	}
}
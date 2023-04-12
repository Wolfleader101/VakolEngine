#pragma once

#include <vector>

#include <Model/Assets/Texture.hpp>

namespace Vakol::Model
{
	struct GLTexture : public Assets::Texture
	{
		
	};

	unsigned int GetTextureCubemap(const std::vector<std::string>& faces, const bool flip);
	unsigned int GetTexture(const std::string& path, const bool flip);
}

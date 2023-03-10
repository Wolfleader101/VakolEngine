#pragma once

#include <vector>

#include "../Texture.hpp"

namespace Vakol::Model
{
	class GLTexture
	{
	public:
		static unsigned int GetTextureCubemap(const std::vector<std::string>& faces);
		static unsigned int GetTexture(const std::string& path);
	};
}

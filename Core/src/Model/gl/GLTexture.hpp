#pragma once

#include "../Texture.hpp"

namespace Vakol::Model
{
	class GLTexture
	{
	public:
		static unsigned int GetTexture(const std::string& path);
	};
}

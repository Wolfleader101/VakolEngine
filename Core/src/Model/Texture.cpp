#include "Texture.hpp"

#include "GLTexture.hpp"

namespace Vakol::Model
{
	std::shared_ptr<Texture> Texture::Create(const std::string& name)
	{
		return std::make_shared<GLTexture>(name);
	}
}
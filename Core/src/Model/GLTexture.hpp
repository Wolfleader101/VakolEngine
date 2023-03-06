#pragma once

#include "Texture.hpp"

namespace Vakol::Model
{
	class GLTexture : public Texture
	{
	public:
		GLTexture(const std::string& name);

		~GLTexture() override;

		void Bind(const unsigned int unit) const override;

		void Unbind() const override;

		const unsigned int GetID() const override;

	private:
		unsigned int id = 0;
	private:
		std::unique_ptr<unsigned char*> GetTextureFromFile(const std::string& name, int& width, int& height, int& colorDepth) const;
	};
}

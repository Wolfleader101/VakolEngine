#pragma once

#include "Texture.hpp"

namespace Vakol::Model
{
	class GLTexture : public Texture
	{
	public:
		GLTexture(const std::string& name);

		~GLTexture() override;

		void Bind() override;

		void Unbind() override;

		unsigned int GetID() override;

	private:
		unsigned int id;
	};
}

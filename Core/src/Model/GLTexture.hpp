#pragma once

#include "Texture.hpp"

namespace Vakol::Model
{
	class GLTexture : public Texture
	{
	public:
		GLTexture(const std::string& name);

		~GLTexture() override;

		void Bind() const override;

		void Unbind() const override;

		unsigned int GetID() const override;

	private:
		unsigned int id;
	};
}

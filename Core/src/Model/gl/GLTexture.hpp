#pragma once

#include <Model/Assets/Texture.hpp>

namespace Vakol::Model
{
	class GLTexture : public Assets::Texture
	{
	public:
		GLTexture(const int width, const int height, const unsigned int format);
		GLTexture(const std::string& path, const bool raw = false);

		~GLTexture();

		void Bind(const unsigned int unit = 0) const;

		void SetData(const void* data);

		const unsigned int id() const { return m_ID; }
	private:
		unsigned int m_ID = 0;
		unsigned int m_format = 0;

		int m_width = 0;
		int m_height = 0;
	};
}
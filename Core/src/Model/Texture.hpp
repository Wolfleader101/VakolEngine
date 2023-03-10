#pragma once

#include <string>

namespace Vakol::Model 
{
	class Texture 
	{
	public:
		virtual ~Texture() = default;

		virtual void Bind(const unsigned int) const = 0;

		virtual void Unbind() const = 0;

		virtual const unsigned int GetID() const = 0;

		static unsigned char* GetImage(const std::string& path, int& width, int& height, int& colorDepth);

		static void FreeImage(unsigned char* image);
	};
}

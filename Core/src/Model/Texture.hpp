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

	public:
		static unsigned char* GetImage(const std::string& path, const bool flip, int& width, int& height, int& colorDepth);

		static void FreeImage(unsigned char* image);

	protected:
		unsigned int id;
		std::string type;
		std::string path;
	};
}

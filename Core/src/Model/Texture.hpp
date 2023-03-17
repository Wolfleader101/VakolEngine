#pragma once

#include <string>

namespace Vakol::Model 
{
	struct Texture 
	{
		unsigned int id;
		std::string type;
		std::string path;
	};

	unsigned char* GetImage(const std::string& path, const bool flip, int& width, int& height, int& colorDepth);
	void FreeImage(unsigned char* image);
}

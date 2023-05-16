#pragma once

#include <string>
#include <vector>

namespace Vakol::Controller
{
	unsigned int LoadTexture(std::vector<const char*>&& faces, bool gamma, bool flip);
	unsigned int LoadTexture(int size, bool gamma, bool flip, const void* data);
	unsigned int LoadTexture(std::string& path, bool gamma, bool flip);
	unsigned int LoadRawTexture(std::string& path);
}
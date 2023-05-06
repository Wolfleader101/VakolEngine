#pragma once

#include <string>
#include <vector>

namespace Vakol::Controller
{
	unsigned int LoadTexture(std::string& path, const bool gamma, const bool flip);
	unsigned int LoadRawTexture(std::string& path);
}
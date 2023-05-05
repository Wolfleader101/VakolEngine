#pragma once

#include <string>
#include <vector>

namespace Vakol::Controller
{
	unsigned int LoadTexture(std::string& path, const bool gamma, const bool flip);
	unsigned int LoadNoiseTexture(const int size, float scale, const int octaves, const float persistence, const float lacunarity);
	unsigned int LoadRawTexture(std::string& path);
}
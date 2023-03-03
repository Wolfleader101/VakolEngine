#pragma once

#include <memory>

typedef unsigned char byte;
typedef unsigned int uint;

struct Texture
{
	Texture() = default;

	int width = 0;
	int height = 0;
	int colorDepth = 0;

private:
	std::unique_ptr<byte*> data;
	uint id;
};
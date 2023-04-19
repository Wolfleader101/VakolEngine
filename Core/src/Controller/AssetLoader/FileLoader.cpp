#define STB_IMAGE_IMPLEMENTATION

#include "FileLoader.hpp"

#include <Controller/Logger.hpp>

#include <fstream>
#include <filesystem>

#include <stb_image.h>

#include <glm/gtc/noise.hpp>

std::string LoadFile(const std::string& path)
{
	std::string result;

	std::ifstream in(path, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII

	if (in)
	{
		in.seekg(0, std::ios::end);

		size_t size = in.tellg();

		if (size != -1)
		{
			result.resize(size);
			in.seekg(0, std::ios::beg);
			in.read(result.data(), size);
		}
		else
			VK_ERROR("Could not read file '{0}'", path);
	}
	else
		VK_ERROR("Could not open file '{0}'", path);

	return result;
}

unsigned char* LoadImage(const std::string& path, int& width, int& height)
{
	unsigned char* result = nullptr;
	size_t size = 0;

	std::ifstream in(path, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII

	if (in)
	{
		in.seekg(0, std::ios::end);

		size = in.tellg();
		result = new unsigned char[size];

		if (size != -1)
		{
			in.seekg(0, std::ios::beg);
			in.read(reinterpret_cast<char*>(result), size);
		}
		else
			VK_ERROR("Could not read file '{0}'", path);
	}
	else
		VK_ERROR("Could not open file '{0}'", path);


	if (size != -1 || size != 0)
		width = height = static_cast<int>(sqrt(size));

	return result;
}

unsigned char* LoadImage(const std::string& path, int& width, int& height, int& channels, const bool flip = true)
{
	// OpenGL be like: Nah we want textures upside down.
    stbi_set_flip_vertically_on_load(flip);
    
    const auto data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    if (!data)
    {
        VK_ERROR("ERROR::FileLoader::LoadImage(): Failed to load image at path {0}", path);
        stbi_image_free(data);
    }

    return data;
}

// unsigned char* Image::LoadNoiseMap(const int size, float scale, float frequency)
// {
// 	auto data = new unsigned char[size * size];

// 	float xFactor = 1.0f / (size - 1);
// 	float yFactor = 1.0f / (size - 1);

// 	for( int row = 0; row < size; row++ ) 
// 	{
// 		for( int col = 0 ; col < size; col++ ) 
// 		{
// 			float x = xFactor * col;
// 			float y = yFactor * row;

// 			float sum = 0.0f;

// 			glm::vec2 p(x * frequency, y * frequency);
			
// 			float val = glm::perlin(p) / scale;
// 			sum += val;

// 			float result = (sum + 1.0f)/ 2.0f;

// 			// Store in texture buffer
// 			data[(row * size + col)] = (unsigned char) ( result * 255.0f );
// 		}
// 	}

// 	return data;
// }

bool FileExists(const std::string& file)
{
	std::filesystem::file_status s = std::filesystem::file_status{};

	return std::filesystem::status_known(s) ? std::filesystem::exists(s) : std::filesystem::exists(file);
}

std::string GetFileSuffix(const std::string& path, const char ch)
{
	if (path.find_last_of(ch) > 1024) 
		return std::string();

	return path.substr(path.find_last_of(ch) + 1);
}

std::string GetFileExtension(const std::string& path)
{
	return path.substr(path.find_last_of('.'));
}
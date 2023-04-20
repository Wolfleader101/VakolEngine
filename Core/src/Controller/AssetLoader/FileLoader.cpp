#define STB_IMAGE_IMPLEMENTATION

#include "FileLoader.hpp"

#include <Controller/Logger.hpp>

#include <fstream>
#include <filesystem>

#include <stb_image.h>

#include <glm/gtc/noise.hpp>

float inverse_lerp(const float a, const float b, const float t);

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

unsigned char* LoadNoiseImage(const int size, float scale, const int octaves, const float persistence, const float lacunarity)
{
	auto data = new unsigned char[size * size];

	srand(static_cast<unsigned int>(time(NULL)));

	std::vector<glm::vec2> offsets;
	glm::vec2 offset = glm::vec2(0.0f);

	offsets.reserve(octaves);

	for (int i = 0; i < octaves; ++i)
	{
		float off_x = rand() + offset.x;
		float off_y = rand() + offset.y;

		offsets.push_back(glm::vec2(off_x, off_y));
	}

	if (scale <= 0.0f)
		scale = 1e-5f;

	float max_noise_height = FLT_MIN;
	float min_noise_height = FLT_MAX;

	float half_size = (float) size / 2.0f;

	for (int y = 0; y < size; ++y)
	{
		for (int x = 0; x < size; ++x)
		{
			float amplitude = 1.0f;
			float frequency = 1.0f;
			float height = 0.0f;

			for (int i = 0; i < octaves; ++i)
			{
				float sample_x = (x - half_size) / scale * frequency + offsets.at(i).x;
				float sample_y = (y - half_size) / scale * frequency + offsets.at(i).y;

				float perlin = glm::perlin(glm::vec2(sample_x, sample_y)) * 2 - 1;
				height += perlin * amplitude;

				amplitude *= persistence;
				frequency *= lacunarity;
			}

			if (height > max_noise_height)
				max_noise_height = height;
			else if (height < min_noise_height)
				min_noise_height = height;

			data[y * size + x] = static_cast<unsigned char>(inverse_lerp(min_noise_height, max_noise_height, height) * 255.0f);
		}
	}

	return data;
}

float inverse_lerp(const float a, const float b, const float t)
{
	return (t - a) / (b - a);
}

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
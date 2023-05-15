#define STB_IMAGE_IMPLEMENTATION

#include "FileLoader.hpp"

#include <Controller/Logger.hpp>

#include <fstream>
#include <iostream>
#include <filesystem>

#include <stb_image.h>

#include <Model/Assets/md2.hpp>

#define DEBUG_VERBOSE 1

int LoadMD2File(const char* path, md2_model_t& mdl)
{
	if (std::ifstream in(path, std::ios::in | std::ios::binary); in)
	{
		in.seekg(0, std::ios::end);

		if (const auto size = in.tellg(); size != -1)
		{
			VK_TRACE("MODEL FILE SIZE: {0} BYTES", size);

			in.seekg(0, std::ios::beg); // start file pos at the beginning of the file
			in.read(reinterpret_cast<char*>(&mdl.header), sizeof(md2_header_t)); // Read the first 68 bytes of file data into the model header
			VK_TRACE(in.tellg());

#if DEBUG_VERBOSE
			std::cout << std::endl;
			VK_TRACE("MESH INFO:");
			std::cout << std::endl;
			VK_TRACE("IDENTITY: {0}", mdl.header.identity);
			VK_TRACE("VERSION:  {0}", mdl.header.version);
			std::cout << std::endl;
			VK_TRACE("SKIN WIDTH: {0}", mdl.header.skin_width);
			VK_TRACE("SKIN HEIGHT: {0}", mdl.header.skin_height);
			VK_TRACE("ANIMATION FRAME SIZE: {0}", mdl.header.frame_size);
			std::cout << std::endl;
			VK_TRACE("NUMBER OF SKINS: {0}", mdl.header.num_skins);
			VK_TRACE("NUMBER OF VERTICES: {0}", mdl.header.num_vertices);
			VK_TRACE("NUMBER OF UVS: {0}", mdl.header.num_uvs);
			VK_TRACE("NUMBER OF TRIANGLES: {0}", mdl.header.num_tris);
			VK_TRACE("NUMBER OF OpenGL COMMANDS: {0}", mdl.header.num_gl_cmds);
			VK_TRACE("NUMBER OF ANIMATION FRAMES: {0}", mdl.header.num_frames);
			std::cout << std::endl;
			VK_TRACE("END INFO");
			std::cout << std::endl;
#endif

			/* Reserve Memory */
			mdl.skins.reserve(mdl.header.num_skins);
			mdl.uvs.reserve(sizeof(md2_uv_t) * mdl.header.num_uvs);
			mdl.triangles.reserve(sizeof(md2_triangle_t) * mdl.header.num_tris);
			mdl.frames.reserve(sizeof(md2_frame_t) * mdl.header.num_frames);
			mdl.gl_cmds.reserve(sizeof(int) * mdl.header.num_gl_cmds);
		}
		else
			VK_ERROR("Could not read file '{0}'", path);
	}
	else
		VK_ERROR("Could not open file '{0}'", path);

	return EXIT_SUCCESS;
}

std::string LoadFile(const std::string& path)
{
	std::string result;

	if (std::ifstream in(path, std::ios::in | std::ios::binary); in)
	{
		in.seekg(0, std::ios::end);

		if (const auto size = in.tellg(); size != -1)
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

unsigned char* LoadImage(const int size, int& width, int& height, int& nrChannels, const bool flip, const void* data)
{
	// OpenGL be like: Nah we want textures upside down.
	stbi_set_flip_vertically_on_load(flip);

	const auto img_data = stbi_load_from_memory(static_cast<const stbi_uc* const>(data), size, &width, &height, &nrChannels, 0);

	if (!img_data)
	{
		VK_ERROR("ERROR::FileLoader::LoadImage(): Failed to load embedded image");
		stbi_image_free(img_data);
	}

	return img_data;
}

unsigned char* LoadImage(std::string&& path, int& width, int& height)
{
	unsigned char* result = nullptr;

	if (std::ifstream in(path, std::ios::in | std::ios::binary); in)
	{
		in.seekg(0, std::ios::end);

		const auto size = in.tellg();

		if (size != -1 || size != 0)
			width = height = static_cast<int>(sqrt(static_cast<int>(size)));

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

	return result;
}

unsigned char* LoadImage(std::string&& path, int& width, int& height, int& channels, const bool flip = true)
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

bool FileExists(const std::string& file)
{
	const auto s = std::filesystem::file_status{};

	return status_known(s) ? exists(s) : std::filesystem::exists(file);
}

std::string GetFileSuffix(const std::string& path, const char ch)
{
	if (path.find_last_of(ch) > 1024) 
		return {};

	return path.substr(path.find_last_of(ch) + 1);
}

std::string GetFileExtension(const std::string& path)
{
	return path.substr(path.find_last_of('.'));
}
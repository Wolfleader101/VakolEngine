#define STB_IMAGE_IMPLEMENTATION

#include "FileLoader.hpp"

#include <stb_image.h>

#include <Controller/Logger.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>

std::string LoadFile(const std::string &path)
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
            VK_CRITICAL("Could not read file '{0}'", path);
    }
    else
        VK_CRITICAL("Could not open file '{0}'", path);

    return result;
}

unsigned char *LoadImage(const int size, int &width, int &height, int &nrChannels, const bool flip, const void *data)
{
    // OpenGL be like: Nah we want textures upside down.
    stbi_set_flip_vertically_on_load(flip);

    const auto img_data =
        stbi_load_from_memory(static_cast<const stbi_uc *const>(data), size, &width, &height, &nrChannels, 0);

    if (!img_data)
    {
        VK_CRITICAL("FileLoader::LoadImage(): Failed to load embedded image");
        stbi_image_free(img_data);
    }

    return img_data;
}

unsigned char *LoadImage(std::string &&path, int &width, int &height)
{
    unsigned char *result = nullptr;

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
            in.read(reinterpret_cast<char *>(result), size);
        }
        else
            VK_CRITICAL("Could not read file '{0}'", path);
    }
    else
        VK_CRITICAL("Could not open file '{0}'", path);

    return result;
}

unsigned char *LoadImage(const char *path, int &width, int &height, int &channels, const bool flip = true)
{
    // OpenGL be like: Nah we want textures upside down.
    stbi_set_flip_vertically_on_load(flip);

    const auto data = stbi_load(path, &width, &height, &channels, 0);

    if (!data)
    {
        VK_CRITICAL("FileLoader::LoadImage(): Failed to load image at path {0}", path);
        stbi_image_free(data);
    }

    return data;
}

unsigned char *LoadImage(std::string &&path, int &width, int &height, int &channels, const bool flip = true)
{
    // OpenGL be like: Nah we want textures upside down.
    stbi_set_flip_vertically_on_load(flip);

    const auto data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    if (!data)
    {
        VK_CRITICAL("FileLoader::LoadImage(): Failed to load image at path {0}", path);
        stbi_image_free(data);
    }

    return data;
}

bool FileExists(const std::string &file)
{
    const auto s = std::filesystem::file_status{};

    return status_known(s) ? exists(s) : std::filesystem::exists(file);
}

std::string GetFileSuffix(const std::string &path, const char ch)
{
    if (path.find_last_of(ch) > 1024)
        return {};

    return path.substr(path.find_last_of(ch) + 1);
}

std::string GetFileExtension(const std::string &path)
{
    return path.substr(path.find_last_of('.'));
}
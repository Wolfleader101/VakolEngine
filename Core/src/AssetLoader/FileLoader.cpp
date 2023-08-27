#include "AssetLoader/FileLoader.hpp"

#include <filesystem>
#include <fstream>

#include "Logger/Logger.hpp"

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
            VK_CRITICAL("Could not read file '{0}'", path);
    }
    else
        VK_CRITICAL("Could not open file '{0}'", path);

    return result;
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

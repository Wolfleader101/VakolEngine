#define STB_IMAGE_IMPLEMENTATION

#include "FileLoader.hpp"

#include <Logger.hpp>

#include <fstream>
#include <filesystem>

#include <stb_image.h>

namespace Vakol::Controller::IO 
{

    std::string LoadFile(const std::string& path) {
        std::string result;

        std::ifstream in(path, std::ios::in | std::ios::binary);  // ifstream closes itself due to RAII

        if (in) {
            in.seekg(0, std::ios::end);

            size_t size = in.tellg();

            if (size != -1) {
                result.resize(size);
                in.seekg(0, std::ios::beg);
                in.read(result.data(), size);
            } else
                VK_ERROR("Could not read file '{0}'", path);
        } else
            VK_ERROR("Could not open file '{0}'", path);

        return result;
    }

    char * LoadImage(const std::string& path, int& width, int& height, int& channels, const bool flip = true) {
        // OpenGL be like: Nah we want textures upside down.
        stbi_set_flip_vertically_on_load(flip);

        const auto data = stbi_load(path.c_str(), &width, &height, &channels, 0);

        if (!data) {
            VK_ERROR("ERROR::ImageLoader::LoadImage(): {0}", "Failed to load image!");
            stbi_image_free(data);
        }

        char* image(reinterpret_cast<char*>(data));

        stbi_image_free(data);

        return image;
    }

    bool FileExists(const std::string& file) {
        std::filesystem::file_status s = std::filesystem::file_status{};

        return std::filesystem::status_known(s) ? std::filesystem::exists(s) : std::filesystem::exists(file);
    }

    std::string GetFileSuffix(const std::string& path, const char ch) {
        if (path.find_last_of(ch) > 1024) return std::string();

        return path.substr(path.find_last_of(ch) + 1);
    }

    std::string GetFileExtension(const std::string& path) { return path.substr(path.find_last_of('.')); }

}  // namespace Vakol::Controller::IO

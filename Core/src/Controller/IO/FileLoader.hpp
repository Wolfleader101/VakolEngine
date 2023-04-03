#pragma once
#define STB_IMAGE_IMPLEMENTATION

#include <string>

namespace Vakol::Controller::IO 
{
    std::string LoadFile(const std::string& path);
    unsigned char* LoadImage(const std::string& path, int& width, int& height, int& nrChannels, const bool flip);

    void FreeImage(unsigned char* image);

    bool FileExists(const std::string& file);

    std::string GetFileSuffix(const std::string& path, const char ch);
    std::string GetFileExtension(const std::string& path);

}  // namespace Vakol::Controller::IO
#pragma once

#include <memory>
#include <string>

namespace Vakol::Rendering::Assets
{

    enum VK_TEXTURE_TYPE
    {
        VK_TEXTURE_NONE,
        VK_TEXTURE_DIFFUSE,
        VK_TEXTURE_SPECULAR,
        VK_TEXTURE_AMBIENT,
        VK_TEXTURE_EMISSION,
        VK_TEXTURE_HEIGHT,
        VK_TEXTURE_NORMAL
    };

    inline std::string ToString(const VK_TEXTURE_TYPE type)
    {
        switch (type)
        {
        case VK_TEXTURE_NONE:
            return "NONE";
        case VK_TEXTURE_DIFFUSE:
            return "DIFFUSE";
        case VK_TEXTURE_SPECULAR:
            return "SPECULAR";
        case VK_TEXTURE_AMBIENT:
            return "AMBIENT";
        case VK_TEXTURE_EMISSION:
            return "EMISSION";
        case VK_TEXTURE_HEIGHT:
            return "HEIGHT";
        case VK_TEXTURE_NORMAL:
            return "NORMAL";
        }

        return "UNKNOWN";
    }

    struct Texture
    {
        std::string path = "undefined";

        unsigned int ID = 0;

        int width = 0;
        int height = 0;

        int channels = 0; // bytes-per-pixel (number of color channels in an image) R = 1, RG = 2, RGB = 3, RGBA = 4

        bool embedded = false;

        VK_TEXTURE_TYPE type = VK_TEXTURE_NONE;
    };
} // namespace Vakol::Rendering::Assets
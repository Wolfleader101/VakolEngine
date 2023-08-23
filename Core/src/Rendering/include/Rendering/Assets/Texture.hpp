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

    /**
     * \brief convert numeric format of texture type to string form
     * \param type the type of texture (in numeric format)
     * \return the string-represented format of texture type
     */
    inline std::string ToString(const unsigned int type)
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

    /**
     * \brief convert string format of texture type to numeric form
     * \param type the type of texture (in string format)
     * \return the numeric-represented format of texture type
     */
    inline VK_TEXTURE_TYPE ToTextureType(const std::string& type)
    {
        if (type == "DIFFUSE")
            return VK_TEXTURE_DIFFUSE;

        if (type == "SPECULAR")
            return VK_TEXTURE_SPECULAR;

        if (type == "AMBIENT")
            return VK_TEXTURE_AMBIENT;

        if (type == "EMISSION" || type == "EMISSIVE")
            return VK_TEXTURE_EMISSION;

        if (type == "HEIGHT")
            return VK_TEXTURE_HEIGHT;

        if (type == "NORMAL")
            return VK_TEXTURE_NORMAL;

        return VK_TEXTURE_NONE;
    }

    struct Texture
    {
        std::string path;

        unsigned int ID = 0;

        int width = 0;
        int height = 0;

        int channels = 0; // bytes-per-pixel (number of color channels in an image) R = 1, RG = 2, RGB = 3, RGBA = 4

        bool embedded = false; // is the texture embedded in memory?

        unsigned int type = VK_TEXTURE_NONE;
    };
} // namespace Vakol::Rendering::Assets
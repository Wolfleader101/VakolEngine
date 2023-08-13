#include "TextureImporter.hpp"

#include "Controller/Logger.hpp"

#include <stb_image.h>

namespace Vakol::Rendering::Assets::Importer
{
    Texture ImportTexture(const std::string& path, unsigned char*& pixels)
    {
        Texture texture;

        pixels = stbi_load(path.c_str(), &texture.width, &texture.height, &texture.bpp, 0);

        if (!pixels)
        {
            VK_ERROR("Unable to Import Texture at path: {0}", path);

            stbi_image_free(pixels);
            pixels = nullptr;
        }

        return texture;
    }

}
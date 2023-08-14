#include "TextureImporter.hpp"

#include "Controller/AssetLoader/FileLoader.hpp"
#include "Controller/Logger.hpp"

#include <stb_image.h>

namespace Vakol::Rendering::Assets::Importer
{
    void ImportTexture(const std::string& path, int& width, int& height, int& channels, unsigned char*& pixels)
    {
        if (!FileExists(path))
        {
            VK_ERROR("Could not find texture file at path: {0}", path);

            return;
        }

        pixels = stbi_load(path.c_str(), &width, &height, &channels, 0);

        if (!pixels)
        {
            VK_ERROR("Unable to Import Texture at path: {0}", path);

            stbi_image_free(pixels);
            pixels = nullptr;
        }
    }

    void ImportTexture(const unsigned char*& buffer, const int length, int& width, int& height, int& channels, unsigned char*& pixels)
    {
        pixels = stbi_load_from_memory(buffer, length, &width, &height, &channels, 0);

        if (!pixels)
        {
            VK_ERROR("Unabled to import embedded texture");

            stbi_image_free(pixels);
            pixels = nullptr;
        }
    }
}